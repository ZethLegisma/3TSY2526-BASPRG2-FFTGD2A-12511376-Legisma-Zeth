#ifndef ACTION_H
#define ACTION_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include "Unit.h"

using namespace std;

inline void cls() {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    system("clear");
#endif
}

inline mt19937& getRNG() {
    static mt19937 rng(999);
    return rng;
}

class Action {
protected:
    string title;
    int cost;

public:
    Action(string t, int c) : title(t), cost(c) {}
    virtual ~Action() {}

    string getName() { return title; }
    int getMpCost() { return cost; }

    virtual void execute(Unit* user, vector<Unit*>& targets, vector<Unit*>& allies) = 0;
};

class BasicAttack : public Action {
public:
    BasicAttack() : Action("Basic Attack", 0) {}

    void execute(Unit* user, vector<Unit*>& targets, vector<Unit*>& allies) override {
        vector<Unit*> alive;
        for (auto t : targets) if (t->isAlive()) alive.push_back(t);
        if (alive.empty()) return;

        Unit* t = nullptr;
        if (user->getIsPlayer()) {
            cout << "\nPick target:\n";
            int idx = 1;
            for (auto u : alive) {
                cout << idx++ << ". " << u->getName() << " (HP: " << u->getHp() << ")\n";
            }
            int sel = 0;
            while (true) {
                cin >> sel;
                if (!cin.fail() && sel >= 1 && sel <= (int)alive.size()) break;
                cout << "bad choice, try again: ";
                cin.clear(); cin.ignore(100, '\n');
            }
            t = alive[sel - 1];
        }
        else {
            uniform_int_distribution<int> dist(0, alive.size() - 1);
            t = alive[dist(getRNG())];
        }

        cls();
        cout << "\n===================================\n";
        cout << user->getName() << " hits " << t->getName() << "!\n\n";

        int hit = (user->getDex() * 100) / t->getAgi();
        if (hit < 20) hit = 20;
        if (hit > 80) hit = 80;

        uniform_int_distribution<int> roll(1, 100);
        if (roll(getRNG()) > hit) {
            cout << "Missed!\n\n";
            cout << "===================================\n";
            return;
        }

        int var = user->getPow() * 20 / 100;
        uniform_int_distribution<int> pdist(user->getPow(), user->getPow() + var);
        int dmg = pdist(getRNG()) - t->getVit();
        if (dmg < 1) dmg = 1;

        if (roll(getRNG()) <= 20) {
            dmg = (dmg * 120) / 100;
            cout << "CRIT!\n\n";
        }

        t->modifyHp(-dmg);
        cout << t->getName() << " took " << dmg << " dmg.\n\n";
        cout << "===================================\n";
    }
};

class SingleTargetSkill : public Action {
public:
    SingleTargetSkill(string name) : Action(name, 5) {}

    void execute(Unit* user, vector<Unit*>& targets, vector<Unit*>& allies) override {
        if (user->getMp() < cost) {
            cout << "No mp\n";
            return;
        }
        user->modifyMp(-cost);

        vector<Unit*> alive;
        for (auto t : targets) if (t->isAlive()) alive.push_back(t);
        if (alive.empty()) return;

        Unit* target = nullptr;
        if (user->getIsPlayer()) {
            cout << "\nPick target for " << title << ":\n";
            int idx = 1;
            for (auto u : alive) {
                cout << idx++ << ". " << u->getName() << " (HP: " << u->getHp() << ")\n";
            }
            int sel = 0;
            while (true) {
                cin >> sel;
                if (!cin.fail() && sel >= 1 && sel <= (int)alive.size()) break;
                cout << "bad choice: ";
                cin.clear(); cin.ignore(100, '\n');
            }
            target = alive[sel - 1];
        }
        else {
            int lowest = 999999;
            for (auto t : alive) {
                if (t->getHp() < lowest) {
                    lowest = t->getHp();
                    target = t;
                }
            }
        }

        cls();
        cout << "\n===================================\n";
        cout << user->getName() << " casts " << title << " on " << target->getName() << "\n\n";

        int var = user->getPow() * 20 / 100;
        uniform_int_distribution<int> pdist(user->getPow(), user->getPow() + var);
        int dmg = ((pdist(getRNG()) * 220) / 100) - target->getVit();
        if (dmg < 1) dmg = 1;

        target->modifyHp(-dmg);
        cout << target->getName() << " took " << dmg << " massive dmg!\n\n";
        cout << "===================================\n";
    }
};

class MultiTargetSkill : public Action {
public:
    MultiTargetSkill(string name) : Action(name, 4) {}

    void execute(Unit* user, vector<Unit*>& targets, vector<Unit*>& allies) override {
        if (user->getMp() < cost) {
            cout << "no mp!\n";
            return;
        }
        user->modifyMp(-cost);

        cls();
        cout << "\n===================================\n";
        cout << user->getName() << " uses " << title << " on everyone!\n\n";

        for (auto t : targets) {
            if (!t->isAlive()) continue;
            int var = user->getPow() * 20 / 100;
            uniform_int_distribution<int> pdist(user->getPow(), user->getPow() + var);
            int dmg = ((pdist(getRNG()) * 90) / 100) - t->getVit();
            if (dmg < 1) dmg = 1;
            t->modifyHp(-dmg);
            cout << t->getName() << " takes " << dmg << " dmg.\n\n";
        }
        cout << "===================================\n";
    }
};

class HealSkill : public Action {
public:
    HealSkill(string name) : Action(name, 3) {}

    void execute(Unit* user, vector<Unit*>& targets, vector<Unit*>& allies) override {
        if (user->getMp() < cost) {
            cout << "out of mp\n";
            return;
        }
        user->modifyMp(-cost);

        Unit* target = nullptr;
        if (user->getIsPlayer()) {
            vector<Unit*> aliveAllies;
            for (auto a : allies) if (a->isAlive()) aliveAllies.push_back(a);
            if (aliveAllies.empty()) return;

            cout << "\nPick ally to heal:\n";
            int idx = 1;
            for (auto u : aliveAllies) {
                cout << idx++ << ". " << u->getName() << " (HP: " << u->getHp() << "/" << u->getMaxHp() << ")\n";
            }
            int sel = 0;
            while (true) {
                cin >> sel;
                if (!cin.fail() && sel >= 1 && sel <= (int)aliveAllies.size()) break;
                cout << "try again: ";
                cin.clear(); cin.ignore(100, '\n');
            }
            target = aliveAllies[sel - 1];
        }
        else {
            int lowest = 999999;
            for (auto a : allies) {
                if (a->isAlive() && a->getHp() < lowest) {
                    lowest = a->getHp();
                    target = a;
                }
            }
        }

        if (!target) return;

        cls();
        cout << "\n===================================\n";
        cout << user->getName() << " heals " << target->getName() << " using " << title << "!\n\n";
        int healAmt = (target->getMaxHp() * 30) / 100;
        target->modifyHp(healAmt);
        cout << target->getName() << " healed for " << healAmt << " HP!\n\n";
        cout << "===================================\n";
    }
};

class Combat {
private:
    vector<Unit*> pTeam, eTeam;

    bool checkWipe(vector<Unit*>& t) {
        for (auto u : t) if (u->isAlive()) return false;
        return true;
    }

    void showStatus() {
        cout << "\n========== BATTLEFIELD ==========\n\n";
        cout << "Players:\n\n";
        for (auto p : pTeam) p->displayInfo();
        cout << "\nEnemies:\n\n";
        for (auto e : eTeam) e->displayInfo();
        cout << "\n===================================\n\n";
    }

public:
    Combat(vector<Unit*> p, vector<Unit*> e) : pTeam(p), eTeam(e) {}

    ~Combat() {
        for (auto u : pTeam) delete u;
        for (auto u : eTeam) delete u;
        pTeam.clear(); eTeam.clear();
    }

    void run() {
        cls();
        cout << "Commence the battle!\n\n";

        while (!checkWipe(pTeam) && !checkWipe(eTeam)) {
            showStatus();

            vector<Unit*> q;
            for (auto u : pTeam) if (u->isAlive()) q.push_back(u);
            for (auto u : eTeam) if (u->isAlive()) q.push_back(u);

            sort(q.begin(), q.end(), [](Unit* a, Unit* b) {
                return a->getAgi() > b->getAgi();
                });

            for (size_t i = 0; i < q.size(); i++) {
                Unit* curr = q[i];
                if (!curr->isAlive()) continue;
                if (checkWipe(pTeam) || checkWipe(eTeam)) break;

                cout << "\nTurn: " << curr->getName() << "\n\n";

                vector<Unit*>& targets = curr->getIsPlayer() ? eTeam : pTeam;
                vector<Unit*>& allies = curr->getIsPlayer() ? pTeam : eTeam;

                Action* chosen = nullptr;
                if (curr->getIsPlayer()) {
                    auto& acts = curr->getActions();
                    cout << "Actions:\n\n";
                    for (size_t j = 0; j < acts.size(); j++) {
                        cout << j + 1 << ". " << acts[j]->getName() << " (MP: " << acts[j]->getMpCost() << ")\n\n";
                    }

                    int choice = 0;
                    while (true) {
                        cout << "choice -> ";
                        cin >> choice;
                        if (!cin.fail() && choice >= 1 && choice <= (int)acts.size()) {
                            chosen = acts[choice - 1];
                            if (curr->getMp() < chosen->getMpCost()) {
                                cout << "\nnot enough mp adventurer, pick another: \n\n";
                                continue;
                            }
                            break;
                        }
                        cout << "\nbad input, try again: \n\n";
                        cin.clear(); cin.ignore(100, '\n');
                    }
                }
                else {
                    auto& acts = curr->getActions();
                    uniform_int_distribution<int> roll(1, 100);
                    chosen = acts[0];
                    if (roll(getRNG()) <= 50 && acts.size() > 1) {
                        vector<Action*> usable;
                        for (size_t s = 1; s < acts.size(); s++) {
                            if (curr->getMp() >= acts[s]->getMpCost()) usable.push_back(acts[s]);
                        }
                        if (!usable.empty()) {
                            uniform_int_distribution<int> pick(0, usable.size() - 1);
                            chosen = usable[pick(getRNG())];
                        }
                    }
                }

                chosen->execute(curr, targets, allies);

                system("pause");
                cls();
            }
        }

        cls();
        cout << "\n==========================================================\n\n";
        if (checkWipe(eTeam)) {
            cout << "YOU WIN!\n\n";
            cout << "No..., it wasn't supposed to go this way...\n\n";
        }
        else {
            cout << "YOU LOSE...\n\n";
            cout << "How unfortunate, you were not made for this adventurer...\n\n";
        }
        cout << "=============================================================\n\n";
    }
};

#endif