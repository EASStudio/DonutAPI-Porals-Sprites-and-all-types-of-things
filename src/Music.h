#pragma once

#include "DonutAPI.h"

#include <string>
#include <vector>

using Note = ::Music;

double NoteNameToHz(const std::string& name);

class MusicTrack
{
public:
    explicit MusicTrack(double bpm = 90.0) : bpm(bpm) {}

    MusicTrack& note(double hz, double beats);
    MusicTrack& rest(double beats);
    MusicTrack& noteName(const std::string& name, double beats);

    double BPM() const { return bpm; }
    const std::vector<Note>& Notes() const { return notes; }

private:
    double bpm;
    std::vector<Note> notes;
};

class MusicSystem
{
public:
    void buildMusic(bool force = false);

    // Background music
    void playMainTheme();
    void playTavern();
    void playVillage();
    void playDungeon();
    void playBattle();
    void stopMusic();
    void toggleMusic();           
    bool musicOn() const { return isMusicOn; }

    // One-shot SFX
    void playHit();
    void playPickup();
    void playLevelUp();
    void playDeath();

    float musicVolume = 0.55f;
    float sfxVolume = 0.50f;

private:
    void bakeTrack(const MusicTrack& track, const std::string& path, int repeats,int sampleRate, bool force);
    void bakeSfx(const std::vector<Note>& notes, const std::string& path, double bpm,int sampleRate, bool force);
    void playFile(const std::string& path);

    static int repeatsForDuration(const MusicTrack& track, double targetSeconds);

    bool isMusicOn = true;
    std::string currentTrack;

    // Themes and Sfx
    static MusicTrack MainTheme();
    static MusicTrack TavernTheme();
    static MusicTrack VillageTheme();
    static MusicTrack DungeonAmbience();
    static MusicTrack BattleTheme();

    static std::vector<Note> HitSfx();
    static std::vector<Note> PickupSfx();
    static std::vector<Note> LevelUpSfx();
    static std::vector<Note> DeathSfx();
};