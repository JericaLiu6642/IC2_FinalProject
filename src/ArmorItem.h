#pragma once
#ifndef ARMORITEM_H
#define ARMORITEM_H

#include <string>
#include "Item.h"
using namespace std;

class ArmorItem : public Item
{
public:
	const int defense_increment;
	ArmorItem(int, string, string, string, int, char, int);
	string getName();
};
#endif
