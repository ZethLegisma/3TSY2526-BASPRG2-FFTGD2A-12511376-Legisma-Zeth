#ifndef UNIT_H
#define UNIT_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Action;

class Unit {
private:
    string name;
    int maxHp, hp;
    int maxMp, mp;
    int pwr, vit, agi, dex;
    bool isPlr;
    vector<Action*> acts;

public:
    Unit(string n, int h, int m, int p, int v, int a, int d, bool plr) {
        name = n; maxHp = h; hp = h; maxMp = m; mp = m;
        pwr = p; vit = v; agi = a; dex = d; isPlr = plr;
    }

    ~Unit() {
        for (size_t i = 0; i < acts.size(); i++) delete acts[i];
        acts.clear();
    }

    string getName() { return name; }
    int getHp() { return hp; }
    int getMaxHp() { return maxHp; }
    int getMp() { return mp; }
    int getMaxMp() { return maxMp; }
    int getPow() { return pwr; }
    int getVit() { return vit; }
    int getAgi() { return agi; }
    int getDex() { return dex; }
    bool getIsPlayer() { return isPlr; }

    bool isAlive() { return hp > 0; }

    void modifyHp(int amt) {
        hp += amt;
        if (hp > maxHp) hp = maxHp;
        if (hp < 0) hp = 0;
    }

    void modifyMp(int amt) {
        mp += amt;
        if (mp > maxMp) mp = maxMp;
        if (mp < 0) mp = 0;
    }

    void addAction(Action* ac) { acts.push_back(ac); }
    vector<Action*>& getActions() { return acts; }

    void displayInfo() {
        cout << name << " -> HP: " << hp << "/" << maxHp << " | MP: " << mp << "/" << maxMp << "\n";
    }
};

#endif