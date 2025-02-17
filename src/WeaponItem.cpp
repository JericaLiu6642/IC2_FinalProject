#include <iostream>
#include"WeaponItem.h"

using namespace std;

WeaponItem::WeaponItem(int l, string n, string e, string d, int w, char t, int i)
	:Item(l, n, e, d, w, t), attack_increment(i)
{

}

string WeaponItem::getName()
{
	return name;
}