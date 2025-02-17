#include<iostream>
#include"BaseMonster.h"

using namespace std;

int BaseMonster::count = 0;

int BaseMonster::getInstanceCount()
{
	return count;
}


BaseMonster::BaseMonster(string n, int a, int d, int e, int m, int mh, int mm)
	:name(n), attack(a), defense(d), exp(e), money(m), max_hp(mh), max_mp(mm)
{
	setHP(max_hp);
	setMP(max_mp);
	count++;
}

BaseMonster::~BaseMonster()
{
	count--;
}

void BaseMonster::setHP(int a)
{
	if (a >= max_hp) {
		hp = max_hp;
	}
	else {
		hp = a;
	}
}

int BaseMonster::getHP() const
{
	return hp;
}

void BaseMonster::setMP(int a)
{
	if (a >= max_mp) {
		mp = max_mp;
	}
	else {
		mp = a;
	}
}

int BaseMonster::getMP() const
{
	return mp;
}