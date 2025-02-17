#pragma once
#ifndef GOBLIN_H
#define GOBLIN_H
#include <string>
#include "BaseMonster.h"

using namespace std;

class GoblinMonster :public BaseMonster
{
public:
	GoblinMonster();
	GoblinMonster(string, int, int, int, int, int, int);
	virtual string serialize();
	static BaseMonster* unserialize(string);
};
#endif