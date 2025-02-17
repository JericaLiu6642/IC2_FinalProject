#include <iostream>
#include "LifePotion.h"
using namespace std;

LifePotion::LifePotion()
	:ConsumableItem(0, "Life_Potion", "Fill your hp to max", 
		"Extend the lifespan of those who drank it.", 2, 'c')
{

}

void LifePotion::use(NovicePlayer* const n) 
{
	n->sethp(n->getMaxHP());
}
