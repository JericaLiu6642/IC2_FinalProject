#include <iostream>
#include <cmath>
#include <sstream>
#include "KnightPlayer.h"

using namespace std;

//constructor
KnightPlayer::KnightPlayer() 
{
	setLevel(1);
	setname("anonymous");
}

KnightPlayer::KnightPlayer(int a) 
{
	setLevel(a);
	setname("anonymous");
}

KnightPlayer::KnightPlayer(int a, string s) 
{
	setLevel(a);
	setname(s);
}

KnightPlayer::KnightPlayer(const KnightPlayer& p) 
{
	setLevel(p.getLevel());
	setname(p.getname());
}

//setter
void KnightPlayer::setLevel(int a)
{
	level = a;
	maxhp = 150 + 25 * a;
	maxmp = 70 + 10 * a;
	attack = 40 + 12 * a;
	defense = 40 + 12 * a;
	lvupexp = ceil(pow(10, log2(level + 1)));
	sethp(150 + 25 * a);
	setmp(70 + 10 * a);
	setexp(ceil(pow(10, log2(a))));
	setmoney(getmoney() + (ceil(pow(10, log2(level)))) * 10);
}

//special
void KnightPlayer::specialSkill(NovicePlayer* p)
{
	int L = getLevel();
	if (getmp() - L * 5 >= 0) {
		p->sethp(p->gethp() + L * 10 * level);
		setmp(getmp() - L * 5);
	}
	else {
		cout << "Current mp is too low to perform magic!" << endl;
	}
}



NovicePlayer* KnightPlayer::unserialize(string s)
{
	stringstream ss(s);
	string temp;

	getline(ss, temp, '$');
	string name = temp;
	temp.clear();
	getline(ss, temp, '$');
	int hp = stoi(temp);
	temp.clear();
	getline(ss, temp, '$');
	int mp = stoi(temp);
	temp.clear();
	getline(ss, temp, '$');
	int exp = stoi(temp);
	temp.clear();
	getline(ss, temp, '$');
	int money = stoi(temp);
	temp.clear();
	getline(ss, temp, '$');
	int level = stoi(temp);
	temp.clear();

	NovicePlayer *point = new KnightPlayer(level, name);
	point->sethp(hp);
	point->setmp(mp);
	point->setexp(exp);
	point->setmoney(money);
	return point;
}