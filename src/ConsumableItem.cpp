#include <iostream>
#include "ConsumableItem.h"
#include "NovicePlayer.h"

using namespace std;

ConsumableItem::ConsumableItem(int l, string n, string e, string d, int w, char t)
	:Item(l, n, e, d, w, t)
{
}
