#pragma once
#ifndef VENOM_H
#define VENOM_H
#include <string>
#include "ConsumableItem.h"
#include "BaseMonster.h"
using namespace std;

class Venom : public ConsumableItem
{
public:
	Venom();
	virtual void use(BaseMonster*);
};
#endif

