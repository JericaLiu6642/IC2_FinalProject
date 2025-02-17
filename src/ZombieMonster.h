#pragma once
#ifndef ZOMBIE_H
#define ZOMBIE_H
#include <string>
#include "BaseMonster.h"
using namespace std;

class ZombieMonster :public BaseMonster
{
public:
	ZombieMonster();
	ZombieMonster(string, int, int, int, int, int, int);
	virtual string serialize();
	static BaseMonster* unserialize(string);
};
#endif
