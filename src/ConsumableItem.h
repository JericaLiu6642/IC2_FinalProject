#pragma once
#ifndef CONSUMITEM_H
#define CONSUMITEM_H

#include <string>
#include "BaseMonster.h"
#include "Item.h"

class NovicePlayer;

using namespace std;

class ConsumableItem : public Item
{
public:
	ConsumableItem(int, string, string, string, int, char);
};
#endif
