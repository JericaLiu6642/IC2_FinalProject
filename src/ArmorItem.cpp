#include <iostream>
#include"ArmorItem.h"
using namespace std;

ArmorItem::ArmorItem(int l, string n, string e, string d, int w, char t, int i)
	:Item(l, n, e, d, w, t), defense_increment(i)
{

}

string ArmorItem::getName()
{
	return name;
}