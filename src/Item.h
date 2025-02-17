#pragma once
#ifndef ITEM_H
#define ITEM_H

#include<string>
#include<vector>
// #include"BaseMonster.h"
// #include"NovicePlayer.h"
// #include"GoblinMonster.h"
// #include"ZombieMonster.h"
// #include"JWMonster.h"
// #include"OrcPlayer.h"
// #include"KnightPlayer.h"
// #include"MagicianPlayer.h"
// #include"Battle.h"
using namespace std;

class NovicePlayer;
class BaseMonster;

class Item
{
public: 
	Item(int, string, string, string, int, char);
	const int level_required;
	const string name;
	const string effect;
	const string description;
	const int weight;
	const char type;
	void use(NovicePlayer* const);
	void use(BaseMonster*);
};

#endif // !ITEM_H
