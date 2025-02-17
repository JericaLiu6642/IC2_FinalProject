#include "NovicePlayer.h"
#include "Sword.h"
#include "Wand.h"
#include "Patronum.h"
#include "Cloak.h"
#include "Venom.h"
#include "LifePotion.h"

using namespace std;

//constructor
NovicePlayer::NovicePlayer()
{
	setLevel(1);
	setname("anonymous");
}

NovicePlayer::NovicePlayer(int a)
{
	setLevel(a);
	setname("anonymous");
}

NovicePlayer::NovicePlayer(int a, string s)
{
	setLevel(a);
	setname(s);
}

NovicePlayer::NovicePlayer(const NovicePlayer& p)
{
	setLevel(p.getLevel());
	setname(p.getname());
}

//destructor
NovicePlayer::~NovicePlayer() {
	for (Item* item : backpack) {
		delete item;
	}

	delete weapon;
	delete armor;
}

//getter & setter
void NovicePlayer::setname(string s)
{
	name = s;
}

string NovicePlayer::getname(void) const
{
	return name;
}

void NovicePlayer::setLevel(int a)
{
	level = a;
	maxhp = 190 + 25 * a;
	maxmp = 50 + 25 * a;
	attack = 60 + 15 * a;
	defense = 60 + 15 * a;
	lvupexp = ceil(pow(10, log2(level + 1)));
	hp = 190 + 25 * a;
	mp = 50 + 25 * a;
	exp = ceil(pow(10, log2(level)));
	money = 0 + ceil(pow(10, log2(level + 1))) * 10;

	if (weapon != NULL) {
		attack += weapon->attack_increment;
	}
	if (armor != NULL) {
		defense += armor->defense_increment;
	}
}

int NovicePlayer::getLevel(void) const
{
	return level;
}

int NovicePlayer::getAttack()
{
	return  60 + 15 * getLevel();
}

int NovicePlayer::getDefense()
{
	return  60 + 15 * getLevel();
}

void NovicePlayer::sethp(int a)
{
	if (a >= getMaxHP()) {
		hp = getMaxHP();
	}
	else {
		hp = a;
	}
}

int NovicePlayer::gethp(void) const
{
	return hp;
}

void NovicePlayer::setmp(int a)
{
	if (a >= getMaxMP()) {
		mp = getMaxMP();
	}
	else {
		mp = a;
	}
}

int NovicePlayer::getmp(void) const
{
	return mp;
}

void NovicePlayer::setexp(int a)
{
	if (a < getLvupExp()) {
		exp = a;
	}
	else {
		int temp = a - getLvupExp();
		setLevel(getLevel() + 1);
		setexp(temp);
	}
}

int NovicePlayer::getexp(void) const
{
	return exp;
}

void NovicePlayer::setmoney(int a)
{
	money = a;
}

int NovicePlayer::getmoney(void) const
{
	return money;
}

int NovicePlayer::getAttack_all(void) const
{
	return attack;
}


int NovicePlayer::getDefense_all(void) const
{
	return defense;
}

int NovicePlayer::getMaxHP(void) const
{
	return maxhp;
}

int NovicePlayer::getMaxMP(void) const
{
	return maxmp;
}

int NovicePlayer::getLvupExp(void) const
{
	return lvupexp;
}

void NovicePlayer::specialSkill(NovicePlayer*)
{
}

void NovicePlayer::specialSkill(BaseMonster*)
{
}

void NovicePlayer::serialize()
{
	string outfile = "Player.txt";
	ofstream output(outfile);
	string s;
	s.clear();	
	s += name + "$";
	s += to_string(gethp()) + "$";
	s += to_string(getmp()) + "$";
	s += to_string(getexp()) + "$";
	s += to_string(getmoney()) + "$";
	s += to_string(getLevel()) + "$";
	s += getWeaponName() + "$";
	s += getArmorName() + "$";
	s += getBackpack();

	output << s;
}

NovicePlayer* NovicePlayer::unserialize(string s)
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
	getline(ss, temp, '$');
	string w = temp;
	temp.clear();
	getline(ss, temp, '$');
	string a = temp;
	temp.clear();
	getline(ss, temp, '$');
	int numItem = stoi(temp);
	temp.clear();

	NovicePlayer *point = new NovicePlayer(level, name);
	point->sethp(hp);
	point->setmp(mp);
	point->setexp(exp);
	point->setmoney(money);
	
	//身上的裝備讀檔復原
	if (w == "Sword_of_Gryffindor") {
		Sword *sword = new Sword;
		point->equipWeapon_s(sword);
	}
	else if (w == "Elder_Wand") {
		Wand *wand = new Wand;
		point->equipWeapon_s(wand);
	}

	if (a == "Cloak_of_Invisibility") {
		Cloak *cloak = new Cloak;
		point->equipArmor_s(cloak);
	}
	else if (a == "Patronum") {
		Patronum *patronum = new Patronum;
		point->equipArmor_s(patronum);
	}

	//背包的東西讀檔復原
	for (int i = 0; i < numItem; i++) {
		getline(ss, temp, '$');
		string n = temp;
		temp.clear();
		
		if (n == "Sword_of_Gryffindor") {
			Sword *sword = new Sword;
			point->putItem(sword);
		}
		else if (n == "Elder_Wand") {
			Wand *wand = new Wand;
			point->putItem(wand);
		}
		else if (n == "Cloak_of_Invisibility") {
			Cloak *cloak = new Cloak;
			point->putItem(cloak);
		}
		else if (n == "Patronum") {
			Patronum *patronum = new Patronum;
			point->putItem(patronum);
		}
		else if (n == "Life_Potion") {
			LifePotion *life = new LifePotion;
			point->putItem(life);
		}
		else if (n == "Nagini's_venom") {
			Venom *venom = new Venom;
			point->putItem(venom);
		}
	}

	return point;
}

string NovicePlayer::getWeaponName()
{
	if (weapon != NULL) {
		return weapon->name;
	}
	else {
		return "0";
	}
}

string NovicePlayer::getArmorName()
{
	if (armor != NULL) {
		return armor->name;
	}
	else {
		return "0";
	}
}

string NovicePlayer::getBackpack()
{
	string s;
	if (backpack.empty()) {
		return "0$";
	}

	s += to_string(backpack.size()) + "$"; //確定之後讀檔背包的空間->決定迴圈數

	for (int i = 0; i < backpack.size(); i++) {
		s += backpack[i]->name + "$";
	}
	return s;
}

void NovicePlayer::setWeapon(WeaponItem* w)
{
	weapon = w;
}

void NovicePlayer::setArmor(ArmorItem* a) 
{
	armor = a;
}

bool NovicePlayer::equipWeapon(WeaponItem* w)
{
	if (getLevel() >= w->level_required) {
		if (weapon != NULL) {
			putItem(weapon);
		}
		setWeapon(w);
		attack = getAttack() + w->attack_increment;
		cout << "\nEquip sucessfully!" << endl;
		system("pause");
		return true;
	}
	else {
		cout << "\nYou haven't reach the required level!" << endl;
		system("pause");
	}
	return false;
}

bool NovicePlayer::equipWeapon_s(WeaponItem* w)
{
	if (getLevel() >= w->level_required) {
		if (weapon != NULL) {
			putItem(weapon);
		}
		setWeapon(w);
		attack = getAttack() + w->attack_increment;
		return true;
	}
	else {
		cout << "\nYou haven't reach the required level!" << endl;
		system("pause");
	}
	return false;
}

bool NovicePlayer::equipArmor(ArmorItem* a)
{
	if (getLevel() >= a->level_required) {
		if (armor != NULL) {
			putItem(armor);
		}
		setArmor(a);
		defense = getDefense() + a->defense_increment;
		cout << "\nEquip sucessfully!" << endl;
		system("pause");
		return true;
	}
	else {
		cout << "\nYou haven't reach the required level!" << endl;
		system("pause");
	}
	return false;
}

bool NovicePlayer::equipArmor_s(ArmorItem* a)
{
	if (getLevel() >= a->level_required) {
		if (armor != NULL) {
			putItem(armor);
		}
		setArmor(a);
		defense = getDefense() + a->defense_increment;
		return true;
	}
	else {
		cout << "\nYou haven't reach the required level!" << endl;
		system("pause");
	}
	return false;
}

bool NovicePlayer::haveCon()
{
	for (auto & i : backpack) {
		if (i->type == 'c') {
			return true;
		}
	}
	return false;
}

Item* NovicePlayer::chooseCon()
{
	cout << "===========" << endl;
	cout << "Potion List" << endl;
	cout << "===========" << endl;
	vector<Item*> conList;
	int count = 1;
	for (int i = 0; i < backpack.size(); i++) {
		if (backpack[i]->type == 'c') {
			cout << "Potion " << count << ": " << backpack[i]->name << endl;
			cout << "   effect: " << backpack[i]->effect << endl;
			count++;
			conList.push_back(backpack[i]);
		}
	}
	cout << "\nPlease choose the potion you want to use:";
	char choice = '0';
	while (1) {
		cin >> choice;
		if (count == 2) {
			if (choice == '1') {
				removeCon(conList[0]);
				return conList[0];
			}
			else {
				"\nWrong input! Please try again:";
			}
		}
		else if (count == 3) {
			if (choice == '1') {
				removeCon(conList[0]);
				return conList[0];
			}
			else if (choice == '2') {
				removeCon(conList[1]);
				return conList[1];
			}
			else {
				"\nWrong input! Please try again:";
			}
		}
		
	}
}

void NovicePlayer::removeCon(Item* t)
{
	for (int i = 0; i < backpack.size(); i++) {
		if (t == backpack[i]) {
			backpack.erase(backpack.begin() + i);
		}
	}
}


bool NovicePlayer::putItem(Item* i)
{
	backpack.push_back(i);
	return true;
}

Item* NovicePlayer::takeItem(int n)
{
	Item* take = backpack[n];
	backpack.erase(backpack.begin() + n);
	return take;
}

int NovicePlayer::count_weight()
{
	int weight = 0;
	for (int i = 0; i < backpack.size(); i++) {
		weight += backpack[i]->weight;
	}
	return weight;
}

void NovicePlayer::printBP()
{
	system("cls");
	cout << "=============================================" << endl;
	cout << "Press 'E' to leave your backpack" << endl;
	cout << "Press the index of item to equip it" << endl;
	cout << "(Current weight: " << count_weight() << " / Weight Limit: 20)" << endl;
	cout << "=============================================" << endl;
	char c;
	WeaponItem* temp_w_on;
	ArmorItem* temp_a_on;
	ConsumableItem* temp_c_on;
	WeaponItem* temp_w_off;
	ArmorItem* temp_a_off;
	ConsumableItem* temp_c_off;
	string itemClass;

	if (!backpack.empty()) {
		//印出背包的東西
		for (int i = 0; i < backpack.size(); i++) {
			cout << "Index: " << i + 1 << endl;
			cout << setw(18) << "Name: " << backpack[i]->name << endl;
			cout << setw(18) << "Level Required: " << backpack[i]->level_required << endl;
			cout << setw(18) << "Effect: " << backpack[i]->effect << endl;
			cout << setw(18) << "Description: " << backpack[i]->description << endl;
			cout << endl;
		}
	}

	while (c = _getch()) {
		if (!backpack.empty()) {
			if (c == 'e' || c == 'E') {
				return;
			}
			else if (c == '1' && backpack.size() >= 1) {
				switch (backpack[0]->type)
				{
				case 'w':
					temp_w_on = static_cast<WeaponItem*>(backpack[0]);
					itemClass = typeid(*temp_w_on).name();
					if (itemClass == "class WeaponItem") {
						if (equipWeapon(temp_w_on) == false) {
							printBP();
						}
						else {
							takeItem(0); //把0號這一格清空
						}
						printBP();
					}
					else {
						cout << "equip weapon error!!" << endl;
					}
					break;

				case 'a':
					temp_a_on = static_cast<ArmorItem*>(backpack[0]);
					itemClass = typeid(*temp_a_on).name();
					if (itemClass == "class ArmorItem") {
						if (equipArmor(temp_a_on) == false) {
							printBP();
						}
						else {
							takeItem(0); //把0號這一格清空
						}
						printBP();
					}
					else {
						cout << "equip armor error!!" << endl;
					}
					break;

				case 'c': //藥水只能在battle使用
					cout << "Potions can only be used in battle!" << endl;
					system("pause");
					system("cls");
					printBP();
					break;
				default:
					break;
				}
			}
			else if (c == '2' && backpack.size() >= 2) {
				switch (backpack[1]->type)
				{
				case 'w':
					temp_w_on = static_cast<WeaponItem*>(backpack[1]);
					itemClass = typeid(*temp_w_on).name();
					if (itemClass == "class WeaponItem") {
						if (equipWeapon(temp_w_on) == false) {
							printBP();
						}
						else {
							takeItem(1); //把0號這一格清空
						}
						printBP();
					}
					else {
						cout << "equip weapon error!!" << endl;
					}
					break;

				case 'a':
					temp_a_on = static_cast<ArmorItem*>(backpack[1]);
					itemClass = typeid(*temp_a_on).name();
					if (itemClass == "class ArmorItem") {
						if (equipArmor(temp_a_on) == false) {
							printBP();
						}
						else {
							takeItem(1); //把1號這一格清空
						}
						printBP();
					}
					else {
						cout << "equip armor error!!" << endl;
					}
					break;

				case 'c': //藥水只能在battle使用
					cout << "Potions can only be used in battle!" << endl;
					system("pause");
					system("cls");
					printBP();
					break;
				default:
					break;
				}
			}
			else if (c == '3' && backpack.size() >= 3) {
				switch (backpack[2]->type)
				{
				case 'w':
					temp_w_on = static_cast<WeaponItem*>(backpack[2]);
					itemClass = typeid(*temp_w_on).name();
					if (itemClass == "class WeaponItem") {
						if (equipWeapon(temp_w_on) == false) {
							printBP();
						}
						else {
							takeItem(2); //把2號這一格清空
						}
						printBP();
					}
					else {
						cout << "equip weapon error!!" << endl;
					}
					break;

				case 'a':
					temp_a_on = static_cast<ArmorItem*>(backpack[2]);
					itemClass = typeid(*temp_a_on).name();
					if (itemClass == "class ArmorItem") {
						if (equipArmor(temp_a_on) == false) {
							printBP();
						}
						else {
							takeItem(2); //把2號這一格清空
						}
						printBP();
					}
					else {
						cout << "equip armor error!!" << endl;
					}
					break;

				case 'c': //藥水只能在battle使用
					cout << "Potions can only be used in battle!" << endl;
					system("pause");
					system("cls");
					printBP();
					break;
				default:
					break;
				}
			}
			else if (c == '4' && backpack.size() >= 4) {
				switch (backpack[3]->type)
				{
				case 'w':
					temp_w_on = static_cast<WeaponItem*>(backpack[3]);
					itemClass = typeid(*temp_w_on).name();
					if (itemClass == "class WeaponItem") {
						if (equipWeapon(temp_w_on) == false) {
							printBP();
						}
						else {
							takeItem(3); //把3號這一格清空
						}
						printBP();
					}
					else {
						cout << "equip weapon error!!" << endl;
					}
					break;

				case 'a':
					temp_a_on = static_cast<ArmorItem*>(backpack[3]);
					itemClass = typeid(*temp_a_on).name();
					if (itemClass == "class ArmorItem") {
						if (equipArmor(temp_a_on) == false) {
							printBP();
						}
						else {
							takeItem(3); //把3號這一格清空
						}
						printBP();
					}
					else {
						cout << "equip armor error!!" << endl;
					}
					break;

				case 'c': //藥水只能在battle使用
					cout << "Potions can only be used in battle!" << endl;
					system("pause");
					system("cls");
					printBP();
					break;
				default:
					break;
				}
			}
			else if (c == '5' && backpack.size() >= 5) {
				switch (backpack[4]->type)
				{
				case 'w':
					temp_w_on = static_cast<WeaponItem*>(backpack[4]);
					itemClass = typeid(*temp_w_on).name();
					if (itemClass == "class WeaponItem") {
						if (equipWeapon(temp_w_on) == false) {
							printBP();
						}
						else {
							takeItem(4); //把4號這一格清空
						}
						printBP();
					}
					else {
						cout << "equip weapon error!!" << endl;
					}
					break;

				case 'a':
					temp_a_on = static_cast<ArmorItem*>(backpack[4]);
					itemClass = typeid(*temp_a_on).name();
					if (itemClass == "class ArmorItem") {
						if (equipArmor(temp_a_on) == false) {
							printBP();
						}
						else {
							takeItem(4); //把4號這一格清空
						}
						printBP();
					}
					else {
						cout << "equip armor error!!" << endl;
					}
					break;

				case 'c': //藥水只能在battle使用
					cout << "Potions can only be used in battle!" << endl;
					system("pause");
					system("cls");
					printBP();
					break;
				default:
					break;
				}
			}
			else if (c == '6' && backpack.size() >= 6) {
				switch (backpack[5]->type)
				{
				case 'w':
					temp_w_on = static_cast<WeaponItem*>(backpack[5]);
					itemClass = typeid(*temp_w_on).name();
					if (itemClass == "class WeaponItem") {
						if (equipWeapon(temp_w_on) == false) {
							printBP();
						}
						else {
							takeItem(5); //把5號這一格清空
						}
						printBP();
					}
					else {
						cout << "equip weapon error!!" << endl;
					}
					break;

				case 'a':
					temp_a_on = static_cast<ArmorItem*>(backpack[5]);
					itemClass = typeid(*temp_a_on).name();
					if (itemClass == "class ArmorItem") {
						if (equipArmor(temp_a_on) == false) {
							printBP();
						}
						else {
							takeItem(5); //把5號這一格清空
						}
						printBP();
					}
					else {
						cout << "equip armor error!!" << endl;
					}
					break;

				case 'c': //藥水只能在battle使用
					cout << "Potions can only be used in battle!" << endl;
					system("pause");
					system("cls");
					printBP();
					break;
				default:
					break;
				}
			}
			else {
				cout << "Input error!" << endl;
				system("pause");
				system("cls");
				printBP();
			}
		}
		else {
			cout << setw(50) << "Your backpack is currently empty." << endl;
			if (c == 'e' || c == 'E') {
				return;
			}
		}
	}
}
