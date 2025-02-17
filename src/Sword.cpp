#include <iostream>
#include "Sword.h"
using namespace std;

Sword::Sword()
	:WeaponItem(3, "Sword_of_Gryffindor", "attack + 30", 
		"Fashioned from pure silver, it is inset with rubies, the stone that represents Gryffindor.",
		3, 'w', 30)
{
}