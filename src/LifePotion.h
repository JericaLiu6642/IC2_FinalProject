#pragma once
#ifndef LIFEPOTION_H
#define LIFEPOTION_H
#include <string>
#include "ConsumableItem.h"
#include "NovicePlayer.h"
using namespace std;

class LifePotion : public ConsumableItem
{
public:
	LifePotion();
	virtual void use(NovicePlayer* const);
};
#endif

