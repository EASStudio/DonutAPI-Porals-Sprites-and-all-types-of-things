#pragma once

#include "Entity.h"

namespace EntitySystems
{
    struct Position
    {
        float x = 172.0f;
        float y = 182.0f;
        float lookY = 0.0f;
        float angle = 80.0f;
    };

    struct Health
    {
        int maxHp = 100;
        int currentHp = 100;
    };

    struct Attack
    {
        int value = 10;
    };

    struct Defense
    {
        int value = 5;
    };

    struct Mana 
    {
        int value = 5;
    };

    struct Weapon
    {
        int attackBonus = 0;
        int manaBonus = 0;
        int itemID = 0;
    };

    struct Armor
    {
        int defenseBonus = 0;
        int hpBonus = 0;
        int itemID = 0;
    };

    struct Potion 
    {
        int manaBonus;
        int defenseBonus = 0;
        int attackBonus = 0;
        int hpBonus = 0;
        int positionBonus = 0; // Random teleportation 
        int itemID = 0;
    };

    struct Food 
    {
        int hpBonus = 0;
        int manaBonus = 0;
        int defenseBonus = 0;
    };

    struct PlayerTag {};

    struct PlayerClass
    {
        enum Type { WARRIOR, MAGE, ROGUE };
        Type type = WARRIOR;
    };

    struct SkillTree
    {
        int smithingLevel = 0;
        int twoHandedLevel = 0;
        int illusionLevel = 0;
        int restoration = 0;
        int lockPicking = 0;
        int pickPocket = 0;
    };

    // Mobs / NPCs

    struct AIState
    {
        enum State { IDLE, CHASE, ATTACK, FLEE, WONDER };
        State state = WONDER;
        float attackCooldown = 0.0f;

        bool hostile = false;          // true = chases/attacks the player; false = wanders, flees if cornered

        float wanderTargetX = 0.0f;    
        float wanderTargetY = 0.0f;
        float wanderTimer = 0.0f;
    };

    struct MobSprite
    {
        int textureIndex = 29;  // Default to horseTex
        float scale = 1.0f;
    };

    struct Direction
    {
        float dirX = 1.0f;
        float dirY = 0.0f;
        float planeX = 0.0f;   // Camera plane for FOV
        float planeY = 0.66f;  // 66 degree FOV
    };
}