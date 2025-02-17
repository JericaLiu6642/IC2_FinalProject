#include <iostream>
#include "Wand.h"
using namespace std;

Wand::Wand()
	:WeaponItem(8, "Elder_Wand", "attack + 100",
		"It was said to have been the most powerful wand ever to exist.",
		2, 'w', 100)
{
}