#pragma once
#ifndef BASEMONSTER_H
#define BASEMONSTER_H

#include <string>
#include <iomanip>
using namespace std;

class BaseMonster
{
public:
	BaseMonster(string, int, int, int, int, int, int);
	~BaseMonster();

	const string name; // Name of the monster
	const int attack; // Attack of the monster
	int defense; // Defense of the monster 
	const int exp; // Experience earned by players after beating this monster
	const int money; // Amount of money dropped after beating this monster
	const int max_hp; // The monster��s maximum HP
	const int max_mp; // The monster��s maximum MP

	void setHP(int);
	int getHP() const;
	void setMP(int);
	int getMP() const;

	static int getInstanceCount();

	virtual string serialize() = 0;

	void print()
	{
		cout << "-------------------------" << endl;
		cout << setw(10) << "name:" << name << endl;
		cout << setw(10) << "hp:" << getHP() << endl;
		cout << setw(10) << "mp:" << getMP() << endl;
		cout << setw(10) << "exp:" << exp << endl;
		cout << setw(10) << "money:" << money << endl;
		cout << setw(10) << "attack:" << attack << endl;
		cout << setw(10) << "defense:" << defense << endl;
		cout << setw(10) << "maxhp:" << max_hp << endl;
		cout << setw(10) << "maxmp:" << max_mp << endl;
		cout << "-------------------------" << endl;
	}

	void printInb() {
		cout << "-------------------------" << endl;
		cout << setw(10) << "name:" << name << endl;
		cout << setw(10) << "hp:" << hp << endl;
		cout << setw(10) << "attack:" << attack << endl;
		cout << setw(10) << "defense:" << defense << endl;
		cout << "-------------------------" << endl;
	}

private:
	int hp; // Current HP of this monster, range: [0, max_hp]
	int mp; // Current MP of this monster, range: [0, max_mp]
	static int count; // Number of instances of monster series classes
	// Don��t forget to increase/decrease it within proper places
};


#endif // !
