#include "Item.h"
#include"NovicePlayer.h"
#include"BaseMonster.h"

using namespace std;

Item::Item(int l, string n, string e, string d, int w, char t)
	:level_required(l), name(n), effect(e), description(d), weight(w), type(t)
{

}

void Item::use(NovicePlayer* const n)
{
	n->sethp(n->getMaxHP());
}

void Item::use(BaseMonster*)
{

}