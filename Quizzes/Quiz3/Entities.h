#ifndef ENTITIES_H
#define ENTITIES_H

#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

// Enum for the classes
enum ClassType {
    WARRIOR = 1,
    ASSASSIN = 2,
    MAGE = 3
};

// Unit class for both player and enemy units
class Unit {
private:
    string name;
    ClassType classType;
    int maxHp;
    int currentHp;
    int pow;
    int vit;
    int agi;
    int dex;

public:
    // creates the unit and its attributes
    Unit(string unitName, ClassType type, int hp, int power, int vitality, int agility, int dexterity) {
        name = unitName;
        classType = type;
        maxHp = hp;
        currentHp = hp;
        pow = power;
        vit = vitality;
        agi = agility;
        dex = dexterity;
    }

    // Destroys the unit 
    ~Unit() {}

    string getName() { return name; }
    ClassType getClassType() { return classType; }
    int getCurrentHp() { return currentHp; }
    int getMaxHp() { return maxHp; }
    int getPow() { return pow; }
    int getVit() { return vit; }
    int getAgi() { return agi; }
    int getDex() { return dex; }

    string getClassName() {
        if (classType == WARRIOR) return "Warrior";
        if (classType == ASSASSIN) return "Assassin";
        if (classType == MAGE) return "Mage";
        return "Unknown";
    }

    // Checks if unit is still alive
    bool isAlive() {
        return currentHp > 0;
    }

    // Calculate hit rate using DEX / AGI x 100
    int CalcHitRate(Unit* defender) {
        if (defender->getAgi() <= 0) return 80;

        int hitRate = (dex * 100) / defender->getAgi();

        if (hitRate < 20) hitRate = 20;
        if (hitRate > 80) hitRate = 80;

        return hitRate;
    }

    // Check if this unit is strong against defender's class using switch-case
    bool isStrongAgainst(ClassType defenderClass) {
        switch (classType) {
        case WARRIOR: return (defenderClass == ASSASSIN);
        case ASSASSIN: return (defenderClass == MAGE);
        case MAGE: return (defenderClass == WARRIOR);
        default: return false;
        }
    }

    // Calculate total damage dealt using POW - VIT x bonusDamage
    int calculateDamage(Unit* defender) {
        int baseDamage = pow - defender->getVit();

        if (isStrongAgainst(defender->getClassType())) {
            baseDamage = (baseDamage * 3) / 2;
        }

        if (baseDamage < 1) {
            baseDamage = 1;
        }

        return baseDamage;
    }

    void takeDamage(int damage) {
        currentHp -= damage;
        if (currentHp < 0) {
            currentHp = 0;
        }
    }

    void healPercentage(double percent) {
        int healAmount = maxHp * percent;
        currentHp += healAmount;

        if (currentHp > maxHp) {
            currentHp = maxHp;
        }

        cout << name << " healed for " << healAmount << " HP! (" << currentHp << "/" << maxHp << " HP)\n";
    }

    // Stat rewards given to the player once defeating an opponent 
    void applyVictoryBonus(ClassType defeatedClass) {
        switch (defeatedClass) {
        case WARRIOR:
            pow += 3;
            vit += 3;
            cout << "Victory Bonus: POW +3, VIT +3!\n";
            break;
        case ASSASSIN:
            agi += 3;
            dex += 3;
            cout << "Victory Bonus: AGI +3, DEX +3!\n";
            break;
        case MAGE:
            pow += 5;
            cout << "Victory Bonus: POW +5!\n";
            break;
        }
    }

    void displayStats() {
        cout << "===========================\n";
        cout << " Name   : " << name << endl;
        cout << " Class  : " << getClassName() << endl;
        cout << " HP     : " << currentHp << " / " << maxHp << endl;
        cout << " POW    : " << pow << " | VIT : " << vit << endl;
        cout << " AGI    : " << agi << " | DEX : " << dex << endl;
        cout << "===========================\n";
    }
};

// Spawns enemy objects that get harder to defeat over time
inline Unit* spawnEnemy(int stage) {
    int randomNum = (rand() % 3) + 1;
    ClassType randomClass;

    if (randomNum == 1) randomClass = WARRIOR;
    else if (randomNum == 2) randomClass = ASSASSIN;
    else randomClass = MAGE;

    string enemyName = "";
    int hp = 0, pow = 0, vit = 0, agi = 0, dex = 0;
    int growth = (stage - 1) * 3;

    switch (randomClass) {
    case WARRIOR:
        enemyName = "Orc Warrior";
        hp = 80 + (stage * 15);
        pow = 12 + growth;
        vit = 8 + growth;
        agi = 6 + (stage * 2);
        dex = 8 + (stage * 2);
        break;
    case ASSASSIN:
        enemyName = "Shadow Assassin";
        hp = 65 + (stage * 12);
        pow = 10 + growth;
        vit = 5 + growth;
        agi = 12 + (stage * 3);
        dex = 12 + (stage * 3);
        break;
    case MAGE:
        enemyName = "Dark Mage";
        hp = 60 + (stage * 10);
        pow = 16 + (stage * 4);
        vit = 4 + growth;
        agi = 8 + (stage * 2);
        dex = 10 + (stage * 2);
        break;
    }

    return new Unit(enemyName, randomClass, hp, pow, vit, agi, dex);
}

#endif
