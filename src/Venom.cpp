#include <iostream>
#include <cmath>
#include "Venom.h"
#include "BaseMonster.h"
using namespace std;

Venom::Venom()
	:ConsumableItem(5, "Nagini's_venom", "Enemy's hp decrease through time ", 
		"An extremely potent poison that prevented the wounds of the victim from healing.", 2, 'c')
{

}

void Venom::use(BaseMonster* m)
{
	m->setHP(m->getHP() - 20 );
}
