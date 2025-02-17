#pragma once
#ifndef JW_H
#define JW_H
#include <string>
#include "BaseMonster.h"
using namespace std;

class JWMonster :public BaseMonster
{
public:
	JWMonster();
	JWMonster(string, int, int, int, int, int, int);
	virtual string serialize();
	static BaseMonster* unserialize(string);
};
#endif
