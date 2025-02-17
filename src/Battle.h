#pragma once
#ifndef BATTLE_H
#define BATTLE_H

#include <string>
#include <vector>
#include "BaseMonster.h"
#include "NovicePlayer.h"
// #include "GoblinMonster.h"
// #include "ZombieMonster.h"
// #include "JWMonster.h"
// #include "OrcPlayer.h"
// #include "KnightPlayer.h"
// #include "MagicianPlayer.h"

using namespace std;

struct Character
{
	char type;
	bool alive;
	void * instance;
};

class Battle
{
private:
	Character* ActionList;
	int turn;

public:
	Battle(vector<NovicePlayer*>, vector<BaseMonster*>);
	~Battle();
	int result(int,int);
	void showPlayerinfo(int);
	void showMoninfo(int);
	int playerWin;
	void print(int);

};


#endif // !BATTLE_H
