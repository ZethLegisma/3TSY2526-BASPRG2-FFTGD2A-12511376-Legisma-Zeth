#include <iostream>
#include <vector>
#include "Unit.h"
#include "Action.h"

using namespace std;

int main() {
    vector<Unit*> pTeam;
    pTeam.push_back(new Unit("E.T.C", 120, 15, 30, 5, 25, 40, true));
    pTeam[0]->addAction(new BasicAttack());
    pTeam[0]->addAction(new MultiTargetSkill("Face Melt"));

    pTeam.push_back(new Unit("Illidan", 100, 15, 35, 3, 30, 45, true));
    pTeam[1]->addAction(new BasicAttack());
    pTeam[1]->addAction(new SingleTargetSkill("The Hunt"));

    pTeam.push_back(new Unit("LiLi", 90, 20, 20, 4, 20, 35, true));
    pTeam[2]->addAction(new BasicAttack());
    pTeam[2]->addAction(new HealSkill("Jug of Life"));

    vector<Unit*> eTeam;
    eTeam.push_back(new Unit("Johanna", 130, 15, 28, 6, 15, 30, false));
    eTeam[0]->addAction(new BasicAttack());
    eTeam[0]->addAction(new MultiTargetSkill("Blessed Shield"));

    eTeam.push_back(new Unit("Valla", 95, 15, 33, 2, 35, 40, false));
    eTeam[1]->addAction(new BasicAttack());
    eTeam[1]->addAction(new SingleTargetSkill("Puncturing Arrow"));

    eTeam.push_back(new Unit("Kharazim", 100, 20, 22, 4, 28, 38, false));
    eTeam[2]->addAction(new BasicAttack());
    eTeam[2]->addAction(new HealSkill("Divine Palm"));

    Combat game(pTeam, eTeam);
    game.run();

    return 0;
}