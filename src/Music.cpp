#include "Music.h"

#include <cctype>
#include <cmath>
#include <fstream>


double NoteNameToHz(const std::string& name)
{
    if (name.empty()) return 0.0;
    if (name == "R" || name == "r" || name == "-" || name == ".")
        return 0.0;

    size_t i = 0;
    char letter = (char)std::toupper((unsigned char)name[i++]);
    if (letter < 'A' || letter > 'G') return 0.0;

    // Semitones above C for each letter name
    static const int semitoneFromC[] = { 9, 11, 0, 2, 4, 5, 7 }; // A B C D E F G
    int semitone = semitoneFromC[letter - 'A'];

    if (i < name.size() && (name[i] == '#' || name[i] == 's' || name[i] == 'S'))
    {
        semitone += 1;
        ++i;
    }

    else if (i < name.size() && (name[i] == 'b' || name[i] == 'f' || name[i] == 'F'))
    {
        semitone -= 1;
        ++i;
    }

    int octave = 4; // default if none given
    if (i < name.size())
    {
        bool neg = false;
        if (name[i] == '-') { neg = true; ++i; }
        if (i < name.size() && std::isdigit((unsigned char)name[i]))
        {
            octave = 0;
            while (i < name.size() && std::isdigit((unsigned char)name[i]))
            {
                octave = octave * 10 + (name[i] - '0');
                ++i;
            }
            if (neg) octave = -octave;
        }
    }

    int midi = (octave + 1) * 12 + semitone;
    return 440.0 * std::pow(2.0, (midi - 69) / 12.0);
}

MusicTrack& MusicTrack::note(double hz, double beats)
{
    notes.push_back(Note{ hz, beats });
    return *this;
}

MusicTrack& MusicTrack::rest(double beats)
{
    return note(0.0, beats);
}

MusicTrack& MusicTrack::noteName(const std::string& name, double beats)
{
    return note(NoteNameToHz(name), beats);
}

static double trackSeconds(const MusicTrack& track)
{
    double secPerBeat = 60.0 / track.BPM();
    double total = 0.0;
    for (const Note& n : track.Notes())
        total += n.beats * secPerBeat;
    return total;
}

int MusicSystem::repeatsForDuration(const MusicTrack& track, double targetSeconds)
{
    double one = trackSeconds(track);
    if (one <= 0.0) return 1;
    int reps = (int)std::ceil(targetSeconds / one);
    return reps < 1 ? 1 : reps;
}

void MusicSystem::bakeTrack(const MusicTrack& track, const std::string& path, int repeats, int sampleRate, bool force)
{
    if (!force)
    {
        std::ifstream check(path, std::ios::binary);
        if (check.good()) return; // already baked from a previous run
    }

    std::vector<Note> loop;
    loop.reserve(track.Notes().size() * (size_t)repeats);
    for (int i = 0; i < repeats; ++i)
        loop.insert(loop.end(), track.Notes().begin(), track.Notes().end());

    CreateMusicFile(path.c_str(), track.BPM(), 60.0, sampleRate,
        loop.data(), (int)loop.size(), musicVolume);
}

void MusicSystem::bakeSfx(const std::vector<Note>& notes, const std::string& path,
    double bpm, int sampleRate, bool force)
{
    if (!force)
    {
        std::ifstream check(path, std::ios::binary);
        if (check.good()) return;
    }

    CreateMusicFile(path.c_str(), bpm, 60.0, sampleRate,
        notes.data(), (int)notes.size(), sfxVolume);
}

void MusicSystem::buildMusic(bool force)
{
    const double TARGET_SECONDS = 180.0; // ~3 minutes per loop file before it repeats

    MusicTrack main = MainTheme();
    MusicTrack tavern = TavernTheme();
    MusicTrack village = VillageTheme();
    MusicTrack dungeon = DungeonAmbience();
    MusicTrack battle = BattleTheme();

    bakeTrack(main, "quest_main.wav", repeatsForDuration(main, TARGET_SECONDS), 44100, force);
    bakeTrack(tavern, "quest_tavern.wav", repeatsForDuration(tavern, TARGET_SECONDS), 44100, force);
    bakeTrack(village, "quest_village.wav", repeatsForDuration(village, TARGET_SECONDS), 44100, force);
    bakeTrack(dungeon, "quest_dungeon.wav", repeatsForDuration(dungeon, TARGET_SECONDS), 44100, force);
    bakeTrack(battle, "quest_battle.wav", repeatsForDuration(battle, TARGET_SECONDS), 44100, force);

    bakeSfx(HitSfx(), "quest_sfx_hit.wav", 60.0, 44100, force);
    bakeSfx(PickupSfx(), "quest_sfx_pickup.wav", 60.0, 44100, force);
    bakeSfx(LevelUpSfx(), "quest_sfx_levelup.wav", 60.0, 44100, force);
    bakeSfx(DeathSfx(), "quest_sfx_death.wav", 60.0, 44100, force);
}

void MusicSystem::playFile(const std::string& path)
{
    currentTrack = path;
    if (isMusicOn)
        PlayMusicFile(path.c_str(), nullptr, nullptr);
}

void MusicSystem::playMainTheme() { playFile("quest_main.wav"); }
void MusicSystem::playTavern() { playFile("quest_tavern.wav"); }
void MusicSystem::playVillage() { playFile("quest_village.wav"); }
void MusicSystem::playDungeon() { playFile("quest_dungeon.wav"); }
void MusicSystem::playBattle() { playFile("quest_battle.wav"); }

void MusicSystem::stopMusic()
{
    StopMusic();
}

void MusicSystem::toggleMusic()
{
    isMusicOn = !isMusicOn;
    if (!isMusicOn)
        StopMusic();

    else if (!currentTrack.empty())
        PlayMusicFile(currentTrack.c_str(), nullptr, nullptr);
}

void MusicSystem::playHit() { PlaySFX("quest_sfx_hit.wav", sfxVolume); }
void MusicSystem::playPickup() { PlaySFX("quest_sfx_pickup.wav", sfxVolume); }
void MusicSystem::playLevelUp() { PlaySFX("quest_sfx_levelup.wav", sfxVolume); }
void MusicSystem::playDeath() { PlaySFX("quest_sfx_death.wav", sfxVolume); }

MusicTrack MusicSystem::MainTheme()
{
    MusicTrack t(108.0); 

    const std::pair<const char*, double> seq[] =
    {
        {"D4",0.5},{"F#4",0.5},{"A4",0.5},{"D5",1.0},
        {"A5",0.75},{"F#5",0.5},{"D5",0.75},
        {"R",0.5},

        {"A4",0.5},{"B4",0.5},{"C#5",0.5},{"D5",1.0},
        {"E5",0.75},{"F#5",0.75},{"G5",0.5},{"A5",1.5},

        {"F#5",0.5},{"A5",0.5},{"D6",1.0},
        {"C#6",0.75},{"B5",0.75},{"A5",1.0},

        {"D5",0.5},{"F#5",0.5},{"A5",0.5},{"D6",1.0},
        {"A5",0.75},{"F#5",0.75},{"D5",1.5},

        {"G4",0.5},{"B4",0.5},{"D5",0.5},{"G5",1.0},
        {"F#5",0.5},{"E5",0.5},{"D5",1.0},

        {"A4",0.25},{"D5",0.25},{"F#5",0.25},{"A5",0.5},
        {"D6",1.0},{"R",0.5},

        {"D5",2.0},{"R",1.0},
    };

    for (auto& n : seq) t.noteName(n.first, n.second);
    return t;
}

MusicTrack MusicSystem::TavernTheme()
{
    MusicTrack t(132.0);
    const std::pair<const char*, double> seq[] =
    {
        {"D4",0.5},{"F#4",0.5},{"A4",0.5},{"D5",0.5},
        {"C5",0.5},{"A4",0.5},{"F#4",0.5},{"D4",1.0},
        {"E4",0.5},{"G4",0.5},{"B4",0.5},{"E5",0.5},
        {"D5",0.5},{"B4",0.5},{"G4",0.5},{"E4",1.0},

        {"A4",0.25},{"D5",0.25},{"F#5",0.5},{"E5",0.5},{"D5",0.5},
        {"R",0.25},{"A4",0.25},{"D5",0.25},{"F#5",0.25},
        {"G5",0.5},{"F#5",0.5},{"E5",0.5},{"D5",0.5},

        {"B4",0.5},{"D5",0.5},{"G5",0.5},{"B5",0.5},
        {"A5",0.5},{"F#5",0.5},{"D5",0.5},{"A4",1.0},
        {"R",0.5},

        {"D4",0.5},{"F#4",0.5},{"A4",0.5},{"D5",0.5},
        {"C5",0.5},{"A4",0.5},{"F#4",0.5},{"D4",1.0},
    };
    for (auto& n : seq) t.noteName(n.first, n.second);
    return t;
}

MusicTrack MusicSystem::VillageTheme()
{
    MusicTrack t(88.0);
    const std::pair<const char*, double> seq[] =
    {
        {"G3",1.0},{"Bb3",1.0},{"D4",2.0},
        {"C4",1.0},{"Eb4",1.0},{"G4",2.0},

        {"F4",0.75},{"G4",0.75},{"A4",0.75},{"Bb4",0.75},
        {"C5",1.5},{"Bb4",0.5},{"A4",1.0},
        {"G4",1.0},{"F4",1.0},{"Eb4",2.0},

        {"D4",1.0},{"F4",1.0},{"Bb4",1.0},{"A4",1.0},
        {"G4",0.75},{"F4",0.75},{"Eb4",0.75},{"D4",0.75},
        {"C4",2.0},

        {"R",1.0},
    };
    for (auto& n : seq) t.noteName(n.first, n.second);
    return t;
}

MusicTrack MusicSystem::DungeonAmbience()
{
    MusicTrack t(45.0);
    const std::pair<const char*, double> seq[] =
    {
        {"C2",6.0},{"Eb2",4.0},{"R",2.0},
        {"G2",3.0},{"Bb2",3.0},{"C3",4.0},
        {"Eb3",2.0},{"D3",2.0},{"C3",2.0},

        {"Bb2",4.0},{"G2",3.0},{"F2",3.0},
        {"Eb2",4.0},{"R",2.0},

        {"C4",1.0},{"Eb4",1.0},{"G4",1.0},{"R",1.0},
        {"Bb3",2.0},{"G3",2.0},

        {"R",4.0},
    };
    for (auto& n : seq) t.noteName(n.first, n.second);
    return t;
}

MusicTrack MusicSystem::BattleTheme()
{
    MusicTrack t(148.0);
    const std::pair<const char*, double> seq[] =
    {
        {"D3",0.25},{"F3",0.25},{"A3",0.25},{"D4",0.25},
        {"C4",0.25},{"A3",0.25},{"F3",0.25},{"D3",0.25},

        {"Eb3",0.125},{"Eb3",0.125},{"G3",0.25},{"Bb3",0.25},
        {"D4",0.5},{"C4",0.25},{"Bb3",0.25},

        {"F3",0.25},{"G3",0.25},{"Ab3",0.25},{"Bb3",0.25},
        {"C4",0.25},{"D4",0.25},{"Eb4",0.25},{"F4",0.25},

        {"D4",0.125},{"F4",0.125},{"A4",0.125},{"D5",0.5},
        {"C5",0.25},{"Bb4",0.25},{"A4",0.5},

        {"R",0.25},
    };
    for (auto& n : seq) t.noteName(n.first, n.second);
    return t;
}

std::vector<Note> MusicSystem::HitSfx()
{
    return 
    {
        Note{ NoteNameToHz("D3"), 0.05 },
        Note{ NoteNameToHz("A2"), 0.08 },
    };
}

std::vector<Note> MusicSystem::PickupSfx()
{
    return 
    {
        Note{ NoteNameToHz("A5"), 0.06 },
        Note{ NoteNameToHz("D6"), 0.10 },
    };
}

std::vector<Note> MusicSystem::LevelUpSfx()
{
    return 
    {
        Note{ NoteNameToHz("D4"), 0.08 },
        Note{ NoteNameToHz("F4"), 0.08 },
        Note{ NoteNameToHz("A4"), 0.08 },
        Note{ NoteNameToHz("D5"), 0.20 },
    };
}

std::vector<Note> MusicSystem::DeathSfx()
{
    return 
    {
        Note{ NoteNameToHz("A3"), 0.30 },
        Note{ NoteNameToHz("G3"), 0.30 },
        Note{ NoteNameToHz("F3"), 0.30 },
        Note{ NoteNameToHz("D3"), 0.60 },
    };
}