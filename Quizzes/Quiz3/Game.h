#ifndef GAME_H
#define GAME_H

#include "Entities.h"

// Player creation
inline Unit* createPlayer() {
    string playerName;
    int classChoice = 0;

    cout << "==================\n";
    cout << "CHARACTER CREATION\n";
    cout << "==================\n\n";

    cout << "Enter player name: ";
    getline(cin, playerName);

    if (playerName == "") {
        playerName = "Brave Adventurer";
    }

    cout << "\nSelect Class:\n";
    cout << "1. Warrior  (High HP & VIT)\n";
    cout << "2. Assassin (High AGI & DEX)\n";
    cout << "3. Mage     (High POW)\n";
    cout << "Choice (1-3): ";
    cin >> classChoice;

    while (classChoice < 1 || classChoice > 3) {
        cout << "Invalid choice! Pick 1, 2, or 3: ";
        cin >> classChoice;
    }

    cin.ignore();

    system("pause");
    system("cls");

    cout << "You have been warned dear adventurer\n";
    cout << "Tread lightly\n\n";

    system("pause");
    system("cls");

    ClassType selectedClass;
    switch (classChoice) {
    case 1: selectedClass = WARRIOR; break;
    case 2: selectedClass = ASSASSIN; break;
    case 3: selectedClass = MAGE; break;
    }

    if (selectedClass == WARRIOR) {
        return new Unit(playerName, WARRIOR, 100, 15, 10, 8, 10);
    }
    else if (selectedClass == ASSASSIN) {
        return new Unit(playerName, ASSASSIN, 80, 12, 6, 15, 15);
    }
    else if (selectedClass == MAGE) {
        return new Unit(playerName, MAGE, 70, 20, 5, 10, 12);
    }

    return nullptr;
}

// Battle sequence logic
inline bool startBattle(Unit* player, Unit* enemy, int stage) {
    cout << "==============\n";
    cout << "STAGE " << stage << " BATTLE\n";
    cout << "==============\n";

    cout << "\n[YOUR UNIT]\n";
    player->displayStats();

    cout << "\n[ENEMY ENCOUNTERED]\n";
    enemy->displayStats();

    system("pause");
    system("cls");

    int turnCount = 1;

    while (player->isAlive() && enemy->isAlive()) {
        cout << "\n--- Turn " << turnCount << " ---\n";

        Unit* first = player;
        Unit* second = enemy;

        // Unit with higher AGI attacks first. If equal AGI, program prioritizes player
        if (enemy->getAgi() > player->getAgi()) {
            first = enemy;
            second = player;
        }

        // First unit attacks
        int hitPercent = first->CalcHitRate(second);
        int roll = (rand() % 100) + 1;

        cout << first->getName() << " attacks " << second->getName()
            << " (Hit Chance: " << hitPercent << "%)... ";

        if (roll <= hitPercent) {
            int damageDealt = first->calculateDamage(second);
            second->takeDamage(damageDealt);
            cout << "HIT! Dealt " << damageDealt << " damage.\n";

            if (first->isStrongAgainst(second->getClassType())) {
                cout << "  -> Class Advantage! Super effective (+50% bonus damage)!\n";
            }
        }
        else {
            cout << "MISSED!\n";
        }

        // Second unit attacks back if still alive
        if (second->isAlive()) {
            hitPercent = second->CalcHitRate(first);
            roll = (rand() % 100) + 1;

            cout << second->getName() << " attacks " << first->getName()
                << " (Hit Chance: " << hitPercent << "%)... ";

            if (roll <= hitPercent) {
                int damageDealt = second->calculateDamage(first);
                first->takeDamage(damageDealt);
                cout << "HIT! Dealt " << damageDealt << " damage.\n";

                if (second->isStrongAgainst(first->getClassType())) {
                    cout << "  -> Class Advantage! Super effective (+50% bonus damage)!\n";
                }
            }
            else {
                cout << "MISSED!\n";
            }
        }

        cout << "\n[Status Update]\n";
        cout << player->getName() << " HP: " << player->getCurrentHp() << "/" << player->getMaxHp() << endl;
        cout << enemy->getName() << " HP: " << enemy->getCurrentHp() << "/" << enemy->getMaxHp() << endl;

        turnCount++;
        system("pause");
        system("cls");
    }

    return player->isAlive();
}

// Runs the game loop
inline void runGame() {
    cout << "=====================\n";
    cout << "WELCOME TO THE ARENA \n";
    cout << "=====================\n";

    system("pause");
    system("cls");

    cout << "Welcome dear adventurer, may we get started?\n";
    cout << "Create your fighter!\n\n";

    system("pause");
    system("cls");

    Unit* player = createPlayer();
    int stage = 1;

    while (true) {
        Unit* enemy = spawnEnemy(stage);

        bool won = startBattle(player, enemy, stage);

        if (won) {
            cout << "==============================\n";
            cout << "VICTORY! You defeated " << enemy->getName() << "!\n";
            cout << "==============================\n";

            player->applyVictoryBonus(enemy->getClassType());
            player->healPercentage(0.30);

            delete enemy;

            stage++;
            system("pause");
            system("cls");
        }
        else {
            cout << "===============\n";
            cout << "    DEFEATED!   \n";
            cout << "===============\n";
            cout << player->getName() << " fell in battle.\n";
            cout << "You progressed up to Stage: " << stage << endl;

            delete enemy;

            break;
        }
    }

    delete player;

    cout << "\nJourney's over adventurer, step into the light at once.\n";
}

#endif
