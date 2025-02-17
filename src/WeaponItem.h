#pragma once
#ifndef WEAPONITEM_H
#define WEAPONITEM_H

#include <string>
#include "Item.h"
using namespace std;

class WeaponItem : public Item
{
public:
	const int attack_increment;
	WeaponItem(int, string, string, string, int, char, int);
	string getName();

};
#endif