#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include<conio.h>
#include<windows.h>
#include<stdlib.h>
#include"field.h"
#include"BaseMonster.h"
#include"NovicePlayer.h"
#include"GoblinMonster.h"
#include"ZombieMonster.h"
#include"JWMonster.h"
#include"OrcPlayer.h"
#include"KnightPlayer.h"
#include"MagicianPlayer.h"
#include"Battle.h"
#include"ArmorItem.h"
#include"Cloak.h"
#include"ConsumableItem.h"
#include"Item.h"
#include"LifePotion.h"
#include"Patronum.h"
#include"Sword.h"
#include"Venom.h"
#include"Wand.h"
#include"WeaponItem.h"

//地圖基礎設定
#define WALL 1
#define ROAD 0
#define START 200
#define DEST 201
//獲得item 的事件
#define GETSWORD 100
#define GETVENOM 101
#define GETLIFE 102
#define GETPATRON 103
#define GETWAND 104
#define GETCLOAK 105
//戰鬥事件
#define BATTLE_A 202 //spiders
#define BATTLE_B 203 //giant
#define BATTLE_C 204 //warewolf
#define BATTLE_D 205 //Dementor
#define BATTLE_V 206 //Voldemort
//課堂事件
#define CHARM 300 //咒語問答 
#define TRANS 301 //機率猜測
#define POTION 302 //選適合的藥水
#define NPC 303 //單純NPC
#define DARK 304 //選適合的咒語
#define THREE 305 //用錢換exp 10000元可升等

using namespace std;

void Field::SetColor(int color = 7) const
{
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}

vector<int> Field::split(const string& s, vector<int>& help, const char delim1, const char delim2)
{
	help.clear();
	istringstream iss(s);
	string temp;

	while (getline(iss, temp, delim1))
	{
		istringstream iss2(temp);
		while (getline(iss2, temp, delim2)) {
			help.push_back(stoi(temp));
		}
	}
	return help;
}

Field::Field(string file, int x, int y, int w, int h, NovicePlayer* me, string c)
{
	ifstream in(file, ios::in);
	if (!in) {
		cerr << "File cannot be open!" << endl;
		exit(1);
	}

	int col, row;
	char skip;
	in >> col >> skip >> row;
	map_col = col;
	map_row = row;

	//設定好地圖的大小
	map_data = new int*[row];
	for (int i = 0; i < row; i++) {
		map_data[i] = new int[col];
	}

	vector<int> help;
	string s;
	string t;
	int row_count = 0;

	// char we don't want to be put into the vector
	const char delim1 = ',';
	const char delim2 = ' ';
	while (getline(in, s)) {
		stringstream ss(s);
		string data;
		while (ss >> data)
		{
			split(s, help, delim1, delim2);
		}
		if (help.size() != 0) {
			for (int j = 0; j < help.size(); j++) {
				map_data[row_count][j] = help[j];
			}
			row_count++;
		}
		help.clear();
	}

	setPosition(x, y);
	setVisionSize(w, h);
	setPlayer(me);

	//取得地圖名字
	stringstream ss2(c);
	string n;
	getline(ss2, n, '.');
	map_name = n;
}

Field::Field(string file, int x, int y, int w, int h, NovicePlayer* me)
{
	ifstream in(file, ios::in);
	if (!in) {
		cerr << "File cannot be open!" << endl;
		exit(1);
	}

	int col, row;
	char skip;
	in >> col >> skip >> row;
	map_col = col;
	map_row = row;

	//設定好地圖的大小
	map_data = new int*[row];
	for (int i = 0; i < row; i++) {
		map_data[i] = new int[col];
	}
	
	vector<int> help;
	string s;
	string t;
	int row_count = 0;

	// char we don't want to be put into the vector
	const char delim1 = ',';
	const char delim2 = ' ';
	while (getline(in, s)) {
		stringstream ss(s);
		string data;
		while (ss >> data)
		{
			split(s, help, delim1, delim2);
		}
		if (help.size() != 0) {
			for (int j = 0; j < help.size(); j++) {
				map_data[row_count][j] = help[j];
			}
			row_count++;
		}
		help.clear();
	}

	setPosition(x, y);
	setVisionSize(w, h);
	setPlayer(me);

	//取得地圖名字
	stringstream ss2(file);
	string n;
	getline(ss2, n, '.');
	map_name = n;
}

void Field::serialize()
{
	string outfile = "save_map.txt";
	ofstream output(outfile);

	output << map_row << "," << map_col << endl;
	for (int i = 0; i < map_row; i++) {
		for (int j = 0; j < map_col; j++) {
			int element = map_data[i][j];
			if (j == map_col - 1) {
				output << element;
			}
			else {
				output << element << ",";
			}
		}
		output << endl;
	}

	string outfile2 = "Player_in_map.txt";
	ofstream out(outfile2);
	out << getMapName() << "$" << current_position_x << "$" << current_position_y << "$";
}

void Field::setPlayer(NovicePlayer* me)
{
	player = me;
}


Field::~Field()
{
	for (auto* ptr : playPoint) {
		delete ptr;
	}
	playPoint.clear();

	for (auto* ptr : monPoint) {
		delete ptr;
	}
	monPoint.clear();

	delete wand;
	delete sword;
	delete cloak;
	delete patronum;
	delete lifePotion;
	delete venom;

	for (int i = 0; i < map_row; ++i) {
		delete[] map_data[i];
	}
	delete[] map_data;
}

bool Field::move(char c) //y控制col所以是左右 x控制row所以是上下
{
	switch (c)
	{
	case 'w':
	case 'W':
		if (map_data[current_position_x - 1][current_position_y] != WALL){
			setPosition(current_position_x - 1, current_position_y);
			system("cls");
			display();
			Events(map_data[current_position_x][current_position_y]);
			return true;
		}
		break;
	case 'a':
	case 'A':
		if (map_data[current_position_x][current_position_y - 1] != WALL){
			setPosition(current_position_x, current_position_y - 1);
			system("cls");
			display();
			Events(map_data[current_position_x][current_position_y]);
			return true;
		}
		break;
	case 's':
	case 'S':
		if (map_data[current_position_x + 1][current_position_y] != WALL){
			setPosition(current_position_x + 1, current_position_y);
			system("cls");
			display();
			Events(map_data[current_position_x][current_position_y]);
			return true;
		}
		break;
	case 'd':
	case 'D':
		if (map_data[current_position_x][current_position_y + 1] != WALL){
			setPosition(current_position_x, current_position_y + 1);
			system("cls");
			display();
			Events(map_data[current_position_x][current_position_y]);
			return true;
		}
		break;
	case 'b':
	case 'B':
		player->printBP();
		system("cls");
		display();
		break;
	case 'x':
	case 'X':
		system("cls");
		cout << "Do you want to save your current progess in the game?" << endl;
		cout << "Press 'y' for yes / Press others for no" << endl;
		cout << "Please choose:";
		char choice = '0';
		cin >> choice;
		if (choice == 'y' || choice == 'Y') {
			player->serialize();
			serialize();
			system("pause");
			system("cls");
			cout << "Saving successfully!" << endl;
			system("pause");
			system("cls");
			cout << "Do you want to return back to the game?" << endl;
			cout << "Press 'y' for yes / Press others for no" << endl;
			cout << "Please choose:";
			cin >> choice;
			if (choice == 'y' || choice == 'Y') {
				system("pause");
				system("cls");
				display();
			}
			else {
				exit(1);
			}
		}
		else {
			exit(1);
		}
	}
	cout << "Illegal move!\n";
	return false;
}

bool Field::moveUp(void)
{
	if (map_data[current_position_x - 1][current_position_y] != WALL) {
		setPosition(current_position_x - 1, current_position_y);
		system("cls");
		display();
		Events(map_data[current_position_x][current_position_y]);
		return true;
	}
	cout << "Illegal move!\n";
	return false;
}

bool Field::moveDown(void)
{
	if (map_data[current_position_x + 1][current_position_y] != WALL) {
		setPosition(current_position_x + 1, current_position_y);
		system("cls");
		display();
		Events(map_data[current_position_x][current_position_y]);
		return true;
	}
	cout << "Illegal move!\n";
	return false;
}

bool Field::moveLeft(void)
{
	if (map_data[current_position_x][current_position_y - 1] != WALL) {
		setPosition(current_position_x, current_position_y - 1);
		system("cls");
		display();
		Events(map_data[current_position_x][current_position_y]);
		return true;
	}
	cout << "Illegal move!\n";
	return false;
}

bool Field::moveRight(void)
{
	if (map_data[current_position_x][current_position_y + 1] != WALL) {
		setPosition(current_position_x, current_position_y + 1);
		system("cls");
		display();
		Events(map_data[current_position_x][current_position_y]);
		return true;
	}
	cout << "Illegal move!\n";
	return false;
}

int  Field::getCurrentPositionX(void) const
{
	return current_position_x;
}

int  Field::getCurrentPositionY(void) const
{
	return current_position_y;
}

int  Field::getVisionWidth(void) const
{
	return vision_width;
}

int  Field::getVisionHeight(void) const
{
	return vision_height;
}

string  Field::getMapName(void) const
{
	return map_name;
}

int  Field::getMapSymbol(int x, int y)
{
	return map_data[x][y];

}

void Field::setPosition(int x, int y)
{
	current_position_x = x;
	current_position_y = y;
}

void Field::setMapSymbol(int s, int x, int y)
{
	map_data[x][y] = s;
}

void Field::setVisionSize(int w, int h)
{
	vision_width = w;
	vision_height = h;
}

void Field::display(void) const
{
	if (map_name != "Forbidden_Forest") {
		cout << "========================================================================" << endl;
		cout << "                              The keys to map                           " << endl;
		cout << "                 S: Your initial place / D: Your destination            " << endl;
		cout << "             T: the Three Broomsticks / C: Classes / B : Battle         " << endl;
		cout << "========================================================================" << endl;
	}
	else {
		cout << "========================================================================" << endl;
		cout << "                             The keys to map                            " << endl;
		cout << "                S: Your initial place / D: Your destination             " << endl;
		cout << "            T: the Three Broomsticks / C: Classes / B : Battle          " << endl;
		cout << "              N: NPC / V: Start a battle with Lord Voldemort            " << endl;
		cout << "========================================================================" << endl;
	}

	cout << setw(25) << "Level: "; 
	SetColor(12);
	cout << player->getLevel() << endl;
	SetColor();

	//輸出玩家目前位置
	cout << "You are now at (" << getCurrentPositionX() << ", " << getCurrentPositionY() << ") of " << map_name << ". " << endl;

	//輸出7*7的視線
	cout << "Vision:" << endl;
	for (int i = current_position_x - 3; i <= current_position_x + 3; i++) {
		for (int j = current_position_y - 3; j <= current_position_y + 3; j++) {
			if (i == current_position_x && j == current_position_y) {
				if (getMapName() == "The_Great_Hall") {
					SetColor(236);
					cout << "▲ ";
					SetColor();
				}
				else if (getMapName() == "The_Quidditch_Pitch") {
					SetColor(172);
					cout << "▲ ";
					SetColor();
				}
				else if (getMapName() == "Forbidden_Forest") {
					SetColor(156);
					cout << "▲ ";
					SetColor();
				}
			}
			else if (map_data[i][j] == WALL) {
				SetColor(240);
				cout << "  ";
				SetColor();
			}
			else if (map_data[i][j] == ROAD) {
				if (getMapName() == "The_Great_Hall") {
					SetColor(238);
					cout << "  ";
					SetColor();
				}
				else if (getMapName() == "The_Quidditch_Pitch") {
					SetColor(160);
					cout << "  ";
					SetColor();
				}
				else if (getMapName() == "Forbidden_Forest") {
					SetColor(153);
					cout << "  ";
					SetColor();
				}
			}
			else if (map_data[i][j] == START) {
				if (getMapName() == "The_Great_Hall") {
					SetColor(236);
					cout << "S ";
					SetColor();
				}
				else if (getMapName() == "The_Quidditch_Pitch") {
					SetColor(172);
					cout << "S ";
					SetColor();
				}
				else if (getMapName() == "Forbidden_Forest") {
					SetColor(156);
					cout << "S ";
					SetColor();
				}
			}
			else if (map_data[i][j] == DEST) {
				if (getMapName() == "The_Great_Hall") {
					SetColor(236);
					cout << "D ";
					SetColor();
				}
				else if (getMapName() == "The_Quidditch_Pitch") {
					SetColor(172);
					cout << "D ";
					SetColor();
				}
				else if (getMapName() == "Forbidden_Forest") {
					SetColor(156);
					cout << "D ";
					SetColor();
				}
			}
			else if (map_data[i][j] == CHARM || map_data[i][j] == TRANS || map_data[i][j] == POTION || map_data[i][j] == DARK) {
				SetColor(176);
				cout << "C ";
				SetColor();
			}
			else if (map_data[i][j] == NPC) {
				SetColor(236);
				cout << "N ";
				SetColor();
			}
			else if (map_data[i][j] == THREE) {
				SetColor(78);
				cout << "T ";
				SetColor();
			}
			else if (map_data[i][j] == BATTLE_A || map_data[i][j] == BATTLE_B || map_data[i][j] == BATTLE_C
				|| map_data[i][j] == BATTLE_D || map_data[i][j] == GETSWORD || map_data[i][j] == GETVENOM || map_data[i][j] == GETLIFE
				|| map_data[i][j] == GETPATRON || map_data[i][j] == GETCLOAK || map_data[i][j] == GETWAND) {
				SetColor(206);
				cout << "B ";
				SetColor();
			}
			else if (map_data[i][j] == BATTLE_V) {
				SetColor(220);
				cout << "V ";
				SetColor();
			}
		}
		cout << endl;
	}

	//提示語
	player->print();
	cout << "\nPress (W : go up / S : go down / A : go left / D : go right)" << endl;
	cout << "Press 'b' to check your backpack" << endl;
	cout << "Press 'x' to leave the game" << endl;

}

void Field::Events(int n)
{
	switch (n)
	{
	case DEST:
		if (map_name != "Forbidden_Forest") { //代表不是最後一張圖
			system("cls");
			cout << "Do you want to leave the current place and move onto the next one?" << endl;
			cout << "(Think wisely, you can not return)" << endl;
			cout << "Press Y to leave, press others to stay:";
			char c;
			cin >> c;
			switch (c) {
			case 'y':
			case 'Y':
				leave = true;
				system("pause");
				system("cls");
				break;
			default:
				system("cls");
				display();
				return;
			}
		}
		else {
			cout << "Are you sure to leave the game?" << endl;
			cout << "Press Y to leave, press others to stay:";
			char c;
			cin >> c;
			switch (c) {
			case 'y':
			case 'Y':
				leave = true;
				break;
				return;
			default:
				system("cls");
				display();
				return;
				break;
			}
		}
		break;

	case GETSWORD:
		system("cls");
		playPoint.clear();
		monPoint.clear();
		monPoint.push_back(new GoblinMonster("Peeves", 20 * player->getLevel(), 10 * player->getLevel(), 10 * player->getLevel(), 20 * player->getLevel(), 
			25 * player->getLevel(), 5));

		cout << "======================" << endl;
		cout << "Encounter rare ememies" << endl;
		cout << "======================" << endl;

		cout << "Entering a battle! Win the battle to get epic item!!" << endl;
		cout << "Do you need another wizard's help? " << endl;
		cout << "Press Y for yes, press E for escape, press others for no:";
		char c;
		cin >> c;

		if (c == 'y' || c == 'Y') {
			system("cls");
			cout << "Choose your helper:" << endl;
			cout << "Enter 1 for Harry Potter: With special magic to change his mp into your hp." << endl;
			cout << "Enter 2 for Ron Weasley: Can cause huge damage to enemies when he attacks." << endl;
			cout << "Enter 3 for Hermione Granger: By decreasing her mp to decrease the enemy's defense." << endl;
			cout << "Please choose:";

			//輸入防呆
			char choice;
			while (1) {
				cin >> choice;
				if (choice == '1' || choice == '2' || choice == '3') {
					break;
				}
				else {
					cout << "Wrong input! Please try again:";
				}
			}

			switch (choice) {
			case '1':
				playPoint.push_back(player);
				playPoint.push_back(new KnightPlayer(player->getLevel(), "Harry Potter"));
				break;
			case '2':
				playPoint.push_back(player);
				playPoint.push_back(new OrcPlayer(player->getLevel(), "Ron Weasley"));
				break;
			case '3':
				playPoint.push_back(player);
				playPoint.push_back(new MagicianPlayer(player->getLevel(), "Hermione Granger"));
				break;
			}

			helperEquip();

			Battle battle(playPoint, monPoint);
			if (battle.playerWin == 2) {
				system("pause");
				system("cls");
				cout << "YOU LOSE THE GAME!" << endl;
				exit(1);
			}
			else if (battle.playerWin == 1) {
				system("cls");
				cout << "Get epic item : Sword of Gryffindor" << endl;
				sword = new Sword;
				player->putItem(sword);
				system("pause");
			}
		}
		else if (c == 'e' || c == 'E') {
			system("cls");
			display();
			return;
			break;
		}
		else {
			playPoint.push_back(player);
			Battle battle(playPoint, monPoint);
			if (battle.playerWin == 2) {
				system("pause");
				system("cls");
				cout << "YOU LOSE THE GAME!" << endl;
				exit(1);
			}
			else if (battle.playerWin == 1) {
				system("cls");
				cout << "Get epic item : Sword of Gryffindor" << endl;
				sword = new Sword;
				player->putItem(sword);
				system("pause");
			}
		}
		playPoint.clear();
		monPoint.clear();
		map_data[current_position_x][current_position_y] = 0;
		system("cls");
		display();
		return;
		break;

	case GETVENOM:
		system("cls");
		playPoint.clear();
		monPoint.clear();
		monPoint.push_back(new GoblinMonster("Draco Malfoy", 15 * player->getLevel(), 10 * player->getLevel(), 10 * player->getLevel(), 5 * player->getLevel(),
			20 * player->getLevel(), 5));

		cout << "======================" << endl;
		cout << "Encounter rare ememies" << endl;
		cout << "======================" << endl;

		cout << "Entering a battle! Win the battle to get epic item!!" << endl;
		cout << "Do you need another wizard's help? " << endl;
		cout << "Press Y for yes, press E for escape, press others for no:";
		cin >> c;

		if (c == 'y' || c == 'Y') {
			system("cls");
			cout << "Choose your helper:" << endl;
			cout << "Enter 1 for Harry Potter: With special magic to change his mp into your hp." << endl;
			cout << "Enter 2 for Ron Weasley: Can cause huge damage to enemies when he attacks." << endl;
			cout << "Enter 3 for Hermione Granger: By decreasing her mp to decrease the enemy's defense." << endl;
			cout << "Please choose:";

			//輸入防呆
			char choice;
			while (1) {
				cin >> choice;
				if (choice == '1' || choice == '2' || choice == '3') {
					break;
				}
				else {
					cout << "Wrong input! Please try again:";
				}
			}

			switch (choice) {
			case '1':
				playPoint.push_back(player);
				playPoint.push_back(new KnightPlayer(player->getLevel(), "Harry Potter"));
				break;
			case '2':
				playPoint.push_back(player);
				playPoint.push_back(new OrcPlayer(player->getLevel(), "Ron Weasley"));
				break;
			case '3':
				playPoint.push_back(player);
				playPoint.push_back(new MagicianPlayer(player->getLevel(), "Hermione Granger"));
				break;
			}

			helperEquip();

			Battle battle(playPoint, monPoint);
			if (battle.playerWin == 2) {
				system("pause");
				system("cls");
				cout << "YOU LOSE THE GAME!" << endl;
				exit(1);
			}
			else if (battle.playerWin == 1) {
				system("cls");
				cout << "Get rare item : Nagini's venom" << endl;
				venom = new Venom;
				player->putItem(venom);
				system("pause");
			}
		}
		else if (c == 'e' || c == 'E') {
			system("cls");
			display();
			return;
			break;
		}
		else {
			playPoint.push_back(player);
			Battle battle(playPoint, monPoint);
			if (battle.playerWin == 2) {
				system("cls");
				cout << "YOU LOSE THE GAME!" << endl;
				exit(1);
			}
			else if (battle.playerWin == 1) {
				system("pause");
				system("cls");
				cout << "Get rare item : Nagini's venom" << endl;
				venom = new Venom;
				player->putItem(venom);
				system("pause");
			}
		}
		playPoint.clear();
		monPoint.clear();
		map_data[current_position_x][current_position_y] = 0;
		system("cls");
		display();
		return;
		break;

	case GETPATRON:
		system("cls");
		playPoint.clear();
		monPoint.clear();
		monPoint.push_back(new GoblinMonster("Boggart", 25 * player->getLevel(), 15 * player->getLevel(), 15 * player->getLevel(), 25 * player->getLevel(),
			30 * player->getLevel(), 5));

		cout << "======================" << endl;
		cout << "Encounter rare ememies" << endl;
		cout << "======================" << endl;

		cout << "Entering a battle! Win the battle to get epic item!!" << endl;
		cout << "Do you need another wizard's help? " << endl;
		cout << "Press Y for yes, press E for escape, press others for no:";
		cin >> c;

		if (c == 'y' || c == 'Y') {
			system("cls");
			cout << "Choose your helper:" << endl;
			cout << "Enter 1 for Harry Potter: With special magic to change his mp into your hp." << endl;
			cout << "Enter 2 for Ron Weasley: Can cause huge damage to enemies when he attacks." << endl;
			cout << "Enter 3 for Hermione Granger: By decreasing her mp to decrease the enemy's defense." << endl;
			cout << "Please choose:";

			//輸入防呆
			char choice;
			while (1) {
				cin >> choice;
				if (choice == '1' || choice == '2' || choice == '3') {
					break;
				}
				else {
					cout << "Wrong input! Please try again:";
				}
			}

			switch (choice) {
			case '1':
				playPoint.push_back(player);
				playPoint.push_back(new KnightPlayer(player->getLevel(), "Harry Potter"));
				break;
			case '2':
				playPoint.push_back(player);
				playPoint.push_back(new OrcPlayer(player->getLevel(), "Ron Weasley"));
				break;
			case '3':
				playPoint.push_back(player);
				playPoint.push_back(new MagicianPlayer(player->getLevel(), "Hermione Granger"));
				break;
			}

			helperEquip();

			Battle battle(playPoint, monPoint);
			if (battle.playerWin == 2) {
				system("pause");
				system("cls");
				cout << "YOU LOSE THE GAME!" << endl;
				exit(1);
			}
			else if (battle.playerWin == 1) {
				system("cls");
				cout << "Get epic item : Patronum" << endl;
				patronum = new Patronum;
				player->putItem(patronum);
				system("pause");
			}
		}
		else if (c == 'e' || c == 'E') {
			system("cls");
			display();
			return;
			break;
		}
		else {
			playPoint.push_back(player);
			Battle battle(playPoint, monPoint);
			if (battle.playerWin == 2) {
				system("pause");
				system("cls");
				cout << "YOU LOSE THE GAME!" << endl;
				exit(1);
			}
			else if (battle.playerWin == 1) {
				system("cls");
				cout << "Get epic item : Patronum" << endl;
				patronum = new Patronum;
				player->putItem(patronum);
				system("pause");
			}
		}
		playPoint.clear();
		monPoint.clear();
		map_data[current_position_x][current_position_y] = 0;
		system("cls");
		display();
		return;
		break;

	case GETLIFE:
		system("cls");
		playPoint.clear();
		monPoint.clear();
		monPoint.push_back(new GoblinMonster("Dragons", 27 * player->getLevel(), 10 * player->getLevel(), 20 * player->getLevel(), 25 * player->getLevel(),
			25 * player->getLevel(), 5));

		cout << "======================" << endl;
		cout << "Encounter rare ememies" << endl;
		cout << "======================" << endl;

		cout << "Entering a battle! Win the battle to get epic item!!" << endl;
		cout << "Do you need another wizard's help? " << endl;
		cout << "Press Y for yes, press E for escape, press others for no:";
		cin >> c;

		if (c == 'y' || c == 'Y') {
			system("cls");
			cout << "Choose your helper:" << endl;
			cout << "Enter 1 for Harry Potter: With special magic to change his mp into your hp." << endl;
			cout << "Enter 2 for Ron Weasley: Can cause huge damage to enemies when he attacks." << endl;
			cout << "Enter 3 for Hermione Granger: By decreasing her mp to decrease the enemy's defense." << endl;
			cout << "Please choose:";

			//輸入防呆
			char choice;
			while (1) {
				cin >> choice;
				if (choice == '1' || choice == '2' || choice == '3') {
					break;
				}
				else {
					cout << "Wrong input! Please try again:";
				}
			}

			switch (choice) {
			case '1':
				playPoint.push_back(player);
				playPoint.push_back(new KnightPlayer(player->getLevel(), "Harry Potter"));
				break;
			case '2':
				playPoint.push_back(player);
				playPoint.push_back(new OrcPlayer(player->getLevel(), "Ron Weasley"));
				break;
			case '3':
				playPoint.push_back(player);
				playPoint.push_back(new MagicianPlayer(player->getLevel(), "Hermione Granger"));
				break;
			}

			helperEquip();

			Battle battle(playPoint, monPoint);
			if (battle.playerWin == 2) {
				system("pause");
				system("cls");
				cout << "YOU LOSE THE GAME!" << endl;
				exit(1);
			}
			else if (battle.playerWin == 1) {
				system("cls");
				cout << "Get epic item : Life Potion" << endl;
				lifePotion = new LifePotion;
				player->putItem(lifePotion);
				system("pause");
			}
		}
		else if (c == 'e' || c == 'E') {
			system("cls");
			display();
			return;
			break;
		}
		else {
			playPoint.push_back(player);
			Battle battle(playPoint, monPoint);
			if (battle.playerWin == 2) {
				system("pause");
				system("cls");
				cout << "YOU LOSE THE GAME!" << endl;
				exit(1);
			}
			else if (battle.playerWin == 1) {
				system("cls");
				cout << "Get epic item : Life Potion" << endl;
				lifePotion = new LifePotion;
				player->putItem(lifePotion);
				system("pause");
			}
		}
		playPoint.clear();
		monPoint.clear();
		map_data[current_position_x][current_position_y] = 0;
		system("cls");
		display();
		return;
		break;

	case GETWAND:
		system("cls");
		playPoint.clear();
		monPoint.clear();
		monPoint.push_back(new GoblinMonster("Merpeople", 25 * player->getLevel(), 15 * player->getLevel(), 25 * player->getLevel(), 25 * player->getLevel(),
			30 * player->getLevel(), 5));

		cout << "======================" << endl;
		cout << "Encounter rare ememies" << endl;
		cout << "======================" << endl;

		cout << "Entering a battle! Win the battle to get epic item!!" << endl;
		cout << "Do you need another wizard's help? " << endl;
		cout << "Press Y for yes, press E for escape, press others for no:";
		cin >> c;

		if (c == 'y' || c == 'Y') {
			system("cls");
			cout << "Choose your helper:" << endl;
			cout << "Enter 1 for Harry Potter: With special magic to change his mp into your hp." << endl;
			cout << "Enter 2 for Ron Weasley: Can cause huge damage to enemies when he attacks." << endl;
			cout << "Enter 3 for Hermione Granger: By decreasing her mp to decrease the enemy's defense." << endl;
			cout << "Please choose:";

			//輸入防呆
			char choice;
			while (1) {
				cin >> choice;
				if (choice == '1' || choice == '2' || choice == '3') {
					break;
				}
				else {
					cout << "Wrong input! Please try again:";
				}
			}

			switch (choice) {
			case '1':
				playPoint.push_back(player);
				playPoint.push_back(new KnightPlayer(player->getLevel(), "Harry Potter"));
				break;
			case '2':
				playPoint.push_back(player);
				playPoint.push_back(new OrcPlayer(player->getLevel(), "Ron Weasley"));
				break;
			case '3':
				playPoint.push_back(player);
				playPoint.push_back(new MagicianPlayer(player->getLevel(), "Hermione Granger"));
				break;
			}

			helperEquip();

			Battle battle(playPoint, monPoint);
			if (battle.playerWin == 2) {
				system("pause");
				system("cls");
				cout << "YOU LOSE THE GAME!" << endl;
				exit(1);
			}
			else if (battle.playerWin == 1) {
				system("cls");
				cout << "Get epic item : Elder Wand" << endl;
				wand = new Wand;
				player->putItem(wand);
				system("pause");
			}
		}
		else if (c == 'e' || c == 'E') {
			system("cls");
			display();
			return;
			break;
		}
		else {
			playPoint.push_back(player);
			Battle battle(playPoint, monPoint);
			if (battle.playerWin == 2) {
				system("pause");
				system("cls");
				cout << "YOU LOSE THE GAME!" << endl;
				exit(1);
			}
			else if (battle.playerWin == 1) {
				system("cls");
				cout << "Get epic item : Elder Wand" << endl;
				wand = new Wand;
				player->putItem(wand);
				system("pause");
			}
		}
		playPoint.clear();
		monPoint.clear();
		map_data[current_position_x][current_position_y] = 0;
		system("cls");
		display();
		return;
		break;

	case GETCLOAK:
		system("cls");
		playPoint.clear();
		monPoint.clear();
		monPoint.push_back(new GoblinMonster("Horned Serpent", 25 * player->getLevel(), 15 * player->getLevel(), 25 * player->getLevel(), 25 * player->getLevel(),
			30 * player->getLevel(), 5));

		cout << "======================" << endl;
		cout << "Encounter rare ememies" << endl;
		cout << "======================" << endl;

		cout << "Entering a battle! Win the battle to get epic item!!" << endl;
		cout << "Do you need another wizard's help? " << endl;
		cout << "Press Y for yes, press E for escape, press others for no:";
		cin >> c;

		if (c == 'y' || c == 'Y') {
			system("cls");
			cout << "Choose your helper:" << endl;
			cout << "Enter 1 for Harry Potter: With special magic to change his mp into your hp." << endl;
			cout << "Enter 2 for Ron Weasley: Can cause huge damage to enemies when he attacks." << endl;
			cout << "Enter 3 for Hermione Granger: By decreasing her mp to decrease the enemy's defense." << endl;
			cout << "Please choose:";

			//輸入防呆
			char choice;
			while (1) {
				cin >> choice;
				if (choice == '1' || choice == '2' || choice == '3') {
					break;
				}
				else {
					cout << "Wrong input! Please try again:";
				}
			}

			switch (choice) {
			case '1':
				playPoint.push_back(player);
				playPoint.push_back(new KnightPlayer(player->getLevel(), "Harry Potter"));
				break;
			case '2':
				playPoint.push_back(player);
				playPoint.push_back(new OrcPlayer(player->getLevel(), "Ron Weasley"));
				break;
			case '3':
				playPoint.push_back(player);
				playPoint.push_back(new MagicianPlayer(player->getLevel(), "Hermione Granger"));
				break;
			}

			helperEquip();

			Battle battle(playPoint, monPoint);
			if (battle.playerWin == 2) {
				system("pause");
				system("cls");
				cout << "YOU LOSE THE GAME!" << endl;
				exit(1);
			}
			else if (battle.playerWin == 1) {
				system("cls");
				cout << "Get epic item : Cloak of Invisibility" << endl;
				cloak = new Cloak;
				player->putItem(cloak);
				system("pause");
			}
		}
		else if (c == 'e' || c == 'E') {
			system("cls");
			display();
			return;
			break;
		}
		else {
			playPoint.push_back(player);
			Battle battle(playPoint, monPoint);
			if (battle.playerWin == 2) {
				system("pause");
				system("cls");
				cout << "YOU LOSE THE GAME!" << endl;
				exit(1);
			}
			else if (battle.playerWin == 1) {
				system("cls");
				cout << "Get epic item : Cloak of Invisibility" << endl;
				cloak = new Cloak;
				player->putItem(cloak);
				system("pause");
			}
		}
		playPoint.clear();
		monPoint.clear();
		map_data[current_position_x][current_position_y] = 0;
		system("cls");
		display();
		return;
		break;

	case BATTLE_A:
		system("cls");
		playPoint.clear();
		monPoint.clear();
		monPoint.push_back(new GoblinMonster("Spider", 50, 20, 20 + 10 * player->getLevel(), 20, 50 + 10 * player->getLevel(), 5));

		cout << "Encounter ememies! Entering a battle! \nDo you need another wizard's help? ";
		cout << "Press Y for yes, press E for escape, press others for no:";
		cin >> c;

		if (c == 'y' || c == 'Y') {
			system("cls");
			cout << "Choose your helper:" << endl;
			cout << "Enter 1 for Harry Potter: With special magic to change his mp into your hp." << endl;
			cout << "Enter 2 for Ron Weasley: Can cause huge damage to enemies when he attacks." << endl;
			cout << "Enter 3 for Hermione Granger: By decreasing her mp to decrease the enemy's defense." << endl;
			cout << "Please choose:";

			//輸入防呆
			char choice;
			while (1) {
				cin >> choice;
				if (choice == '1' || choice == '2' || choice == '3') {
					break;
				}
				else {
					cout << "Wrong input! Please try again:";
				}
			}

			switch (choice) {
			case '1':
				playPoint.push_back(player);
				playPoint.push_back(new KnightPlayer(player->getLevel(), "Harry Potter"));
				break;
			case '2':
				playPoint.push_back(player);
				playPoint.push_back(new OrcPlayer(player->getLevel(), "Ron Weasley"));
				break;
			case '3':
				playPoint.push_back(player);
				playPoint.push_back(new MagicianPlayer(player->getLevel(), "Hermione Granger"));
				break;
			}

			helperEquip();

			Battle battle(playPoint, monPoint);
			if (battle.playerWin == 2) {
				system("pause");
				system("cls");
				cout << "YOU LOSE THE GAME!" << endl;
				exit(1);
			}
		}
		else if (c == 'e' || c == 'E') {
			system("cls");
			display();
			return;
			break;
		}
		else {
			playPoint.push_back(player);
			Battle battle(playPoint, monPoint);
			if (battle.playerWin == 2) {
				system("pause");
				system("cls");
				cout << "YOU LOSE THE GAME!" << endl;
				exit(1);
			}
		}
		playPoint.clear();
		monPoint.clear();
		map_data[current_position_x][current_position_y] = 0;
		system("cls");
		display();
		return;
		break;

	case BATTLE_B:
		system("cls");
		playPoint.clear();
		monPoint.clear();
		monPoint.push_back(new ZombieMonster("Giant", 75, 45, 80 + 10 * player->getLevel(), 100, 100 + 15 * player->getLevel(), 30));

		cout << "Encounter ememies! Entering a battle! \nDo you need another wizard's help? ";
		cout << "Press Y for yes, press E for escape, press others for no:";
		char a;
		cin >> a;

		if (a == 'y' || a == 'Y') {
			system("cls");
			cout << "Choose your helper:" << endl;
			cout << "Enter 1 for Harry Potter: With special magic to change his mp into your hp." << endl;
			cout << "Enter 2 for Ron Weasley: Can cause huge damage to enemies when he attacks." << endl;
			cout << "Enter 3 for Hermione Granger: By decreasing her mp to decrease the enemy's defense." << endl;
			cout << "Please choose:";

			//輸入防呆
			char choice;
			while (1) {
				cin >> choice;
				if (choice == '1' || choice == '2' || choice == '3') {
					break;
				}
				else {
					cout << "Wrong input! Please try again:";
				}
			}
			switch (choice) {
			case '1':
				playPoint.push_back(player);
				playPoint.push_back(new KnightPlayer(player->getLevel(), "Harry Potter"));
				break;
			case '2':
				playPoint.push_back(player);
				playPoint.push_back(new OrcPlayer(player->getLevel(), "Ron Weasley"));
				break;
			case '3':
				playPoint.push_back(player);
				playPoint.push_back(new MagicianPlayer(player->getLevel(), "Hermione Granger"));
				break;
			}

			helperEquip();

			Battle battle(playPoint, monPoint);
			if (battle.playerWin == 2) {
				system("pause");
				system("cls");
				cout << "YOU LOSE THE GAME!" << endl;
				exit(1);
			}
		}
		else if (a == 'e' || a == 'E') {
			system("cls");
			display();
			return;
			break;
		}
		else {
			playPoint.push_back(player);
			Battle battle(playPoint, monPoint);
			if (battle.playerWin == 2) {
				system("pause");
				system("cls");
				cout << "YOU LOSE THE GAME!" << endl;
				exit(1);
			}
		}
		playPoint.clear();
		monPoint.clear();
		map_data[current_position_x][current_position_y] = 0;
		system("cls");
		display();
		return;
		break;

	case BATTLE_C:
		system("cls");
		playPoint.clear();
		monPoint.clear();
		monPoint.push_back(new ZombieMonster("Warewolf", 120, 65, 120 + 15 * player->getLevel(), 150, 175 + 15 * player->getLevel(), 30));

		cout << "Encounter ememies! Entering a battle! \nDo you need another wizard's help? ";
		cout << "Press Y for yes, press E for escape, press others for no:";
		char a1;
		cin >> a1;

		if (a1 == 'y' || a1 == 'Y') {
			system("cls");
			cout << "Choose your helper:" << endl;
			cout << "Enter 1 for Harry Potter: With special magic to change his mp into your hp." << endl;
			cout << "Enter 2 for Ron Weasley: Can cause huge damage to enemies when he attacks." << endl;
			cout << "Enter 3 for Hermione Granger: By decreasing her mp to decrease the enemy's defense." << endl;
			cout << "Please choose:";

			//輸入防呆
			char choice;
			while (1) {
				cin >> choice;
				if (choice == '1' || choice == '2' || choice == '3') {
					break;
				}
				else {
					cout << "Wrong input! Please try again:";
				}
			}
			switch (choice) {
			case '1':
				playPoint.push_back(player);
				playPoint.push_back(new KnightPlayer(player->getLevel(), "Harry Potter"));
				break;
			case '2':
				playPoint.push_back(player);
				playPoint.push_back(new OrcPlayer(player->getLevel(), "Ron Weasley"));
				break;
			case '3':
				playPoint.push_back(player);
				playPoint.push_back(new MagicianPlayer(player->getLevel(), "Hermione Granger"));
				break;
			}

			helperEquip();

			Battle battle(playPoint, monPoint);
			if (battle.playerWin == 2) {
				system("pause");
				system("cls");
				cout << "YOU LOSE THE GAME!" << endl;
				exit(1);
			}
		}
		else if (a1 == 'e' || a1 == 'E') {
			system("cls");
			display();
			return;
			break;
		}
		else {
			playPoint.push_back(player);
			Battle battle(playPoint, monPoint);
			if (battle.playerWin == 2) {
				system("pause");
				system("cls");
				cout << "YOU LOSE THE GAME!" << endl;
				exit(1);
			}
		}
		playPoint.clear();
		monPoint.clear();
		map_data[current_position_x][current_position_y] = 0;
		system("cls");
		display();
		return;
		break;

	case BATTLE_D:
		system("cls");
		playPoint.clear();
		monPoint.clear();
		monPoint.push_back(new ZombieMonster("Dementor", 150 , 80, 180 + 20 * player->getLevel(), 200, 200 + 10 * player->getLevel(), 30));

		cout << "Encounter ememies! Entering a battle! \nDo you need another wizard's help? ";
		cout << "Press Y for yes, press E for escape, press others for no:";
		char a2;
		cin >> a2;

		if (a2 == 'y' || a2 == 'Y') {
			system("cls");
			cout << "Choose your helper:" << endl;
			cout << "Enter 1 for Harry Potter: With special magic to change his mp into your hp." << endl;
			cout << "Enter 2 for Ron Weasley: Can cause huge damage to enemies when he attacks." << endl;
			cout << "Enter 3 for Hermione Granger: By decreasing her mp to decrease the enemy's defense." << endl;
			cout << "Please choose:";

			//輸入防呆
			char choice;
			while (1) {
				cin >> choice;
				if (choice == '1' || choice == '2' || choice == '3') {
					break;
				}
				else {
					cout << "Wrong input! Please try again:";
				}
			}
			switch (choice) {
			case '1':
				playPoint.push_back(player);
				playPoint.push_back(new KnightPlayer(player->getLevel(), "Harry Potter"));
				break;
			case '2':
				playPoint.push_back(player);
				playPoint.push_back(new OrcPlayer(player->getLevel(), "Ron Weasley"));
				break;
			case '3':
				playPoint.push_back(player);
				playPoint.push_back(new MagicianPlayer(player->getLevel(), "Hermione Granger"));
				break;
			}

			helperEquip();

			Battle battle(playPoint, monPoint);
			if (battle.playerWin == 2) {
				system("pause");
				system("cls");
				cout << "YOU LOSE THE GAME!" << endl;
				exit(1);
			}
		}
		else if (a2 == 'e' || a2 == 'E') {
			system("cls");
			display();
			return;
			break;
		}
		else {
			playPoint.push_back(player);
			Battle battle(playPoint, monPoint);
			if (battle.playerWin == 2) {
				system("pause");
				system("cls");
				cout << "YOU LOSE THE GAME!" << endl;
				exit(1);
			}
		}
		playPoint.clear();
		monPoint.clear();
		map_data[current_position_x][current_position_y] = 0;
		system("cls");
		display();
		return;
		break;

	case BATTLE_V:
		system("cls");
		playPoint.clear();
		monPoint.clear();
		monPoint.push_back(new JWMonster("Lord Voldemort", 250 + 10 * player->getLevel(), 150 + 5 * player->getLevel()
			, 1000, 1000, 375 + 15 * player->getLevel(), 100));

		cout << "Battle with Lord Voldemort! Win this battle to end the sememster! \nDo you need another wizard's help? ";
		cout << "Press Y for yes, press E for escape, press others for no:";
		char b;
		cin >> b;

		if (b == 'y' || b == 'Y') {
			system("cls");
			cout << "Choose your helper:" << endl;
			cout << "Enter 1 for Harry Potter: With special magic to change his mp into your hp." << endl;
			cout << "Enter 2 for Ron Weasley: Can cause huge damage to enemies when he attacks." << endl;
			cout << "Enter 3 for Hermione Granger: By decreasing her mp to decrease the enemy's defense." << endl;
			cout << "Please choose:";

			//輸入防呆
			char choice;
			while (1) {
				cin >> choice;
				if (choice == '1' || choice == '2' || choice == '3') {
					break;
				}
				else {
					cout << "Wrong input! Please try again:";
				}
			}

			switch (choice) {
			case '1':
				playPoint.push_back(player);
				playPoint.push_back(new KnightPlayer(player->getLevel(), "Harry Potter"));
				break;
			case '2':
				playPoint.push_back(player);
				playPoint.push_back(new OrcPlayer(player->getLevel(), "Ron Weasley"));
				break;
			case '3':
				playPoint.push_back(player);
				playPoint.push_back(new MagicianPlayer(player->getLevel(), "Hermione Granger"));
				break;
			}

			helperEquip();

			Battle battle(playPoint, monPoint);
			if (battle.playerWin == 1) {
				system("pause");
				system("cls");
				cout << "GAME OVER! YOU WIN!" << endl;
				exit(1);
			}
			else if (battle.playerWin == 2) {
				system("pause");
				system("cls");
				cout << "YOU LOSE THE GAME!" << endl;
				exit(1);
			}
		}
		else if (b == 'e' || b == 'E') {
			system("cls");
			display();
			return;
			break;
		}
		else {
			playPoint.push_back(player);
			Battle battle(playPoint, monPoint);
			if (battle.playerWin == 2) {
				system("pause");
				system("cls");
				cout << "YOU LOSE THE GAME!" << endl;
				exit(1);
			}
		}
		playPoint.clear();
		monPoint.clear();
		map_data[current_position_x][current_position_y] = 0;
		system("cls");
		display();
		return;
		break;

	case CHARM:
		Charm();
		break;
	case TRANS:
		Trans();
		break;
	case POTION:
		Potion();
		break;
	case NPC:
		npc();
		break;
	case DARK:
		Dark();
		break;
	case THREE:
		Three();
		break;
	}

}

void Field::Charm()
{
	//互動部分
	system("cls");
	cout << "Welcome to charm class. I'm your professor Filius Flitwick." << endl;
	system("pause");
	cout << "Today we are learning the 'floating charm'. Can you please tell me which of the spell below is the one we're learning today?" << endl;
	cout << "Option A. Avada Kedavra" << endl;
	cout << "Option B. Wingardium Leviosa" << endl;
	cout << "Option C. Expecto patronum" << endl;
	system("pause");
	cout << "Please choose:";
	char ans;
	while (1) {
		cin >> ans;
		if (ans == 'B' || ans == 'b') {
			break;
		}
		else {
			cout << "Wrong answer! Please try again:";
		}
	}
	cout << "You choose the correct spell! Great job!" << endl;
	system("pause");
	system("cls");

	//主線推進部分
	cout << "What? You said you wanna find the Sword of Gryffindor?" << endl;
	system("pause");
	cout << "Humm..., it won't be easy for you to find it because Peeves stole it two weeks ago." << endl;
	system("pause");
	cout << "I recommend you search the 'southwest' side of the Great Hall first. The chances are high that he will be there." << endl;
	system("pause");
	cout << "Good luck!" << endl;
	system("pause");
	system("cls");
	map_data[current_position_x][current_position_y] = 0;
	display();
	return;
}

void Field::Trans()
{
	//互動部分
	system("cls");
	cout << "Transfiguration is some of the most complex and dangerous magic you will learn at Hogwarts. I'm your professor Minerva McGonagall." << endl;
	system("pause");
	cout << "Everyone, please look closely at the three things on the table, and guess which one is transfigured." << endl;
	cout << "Option A. the cup" << endl;
	cout << "Option B. the vase" << endl;
	cout << "Option C. the parchment" << endl;
	system("pause");
	cout << "Please choose:";
	char ans;
	while (1) {
		cin >> ans;
		if (ans == 'C' || ans == 'c') {
			break;
		}
		else {
			cout << "Wrong answer! Please try again:";
		}
	}
	cout << "Great guess. You've got talent in Transfiguration." << endl;
	system("pause");
	system("cls");

	//主線推進部分
	cout << player->getname() << ", can you please do me a favor?" << endl;
	system("pause");
	cout << "I need to see Draco Malfoy. This is his third time skipping my class." << endl;
	system("pause");
	cout << "He should be somewhere near the 'southeast' side of the Great Hall. Please go get him for me." << endl;
	system("pause");
	system("cls");
	map_data[current_position_x][current_position_y] = 0;
	display();
	return;
}

void Field::Potion()
{
	//互動部分
	system("cls");
	cout << "I believed it is unecessary to say more about myself. You all know my name is Snape." << endl;
	system("pause");
	cout << "You are here to learn the subtle science and exact art of potion-making." << endl;
	system("pause");
	cout << "So, answer my question. What potion transforms the person who drinks it into the physical form of another person?" << endl;
	cout << "Option A. Veritaserum" << endl;
	cout << "Option B. Amortentia" << endl;
	cout << "Option C. Polyjuice Potion" << endl;
	system("pause");
	cout << "Please choose:";
	char ans;
	while (1) {
		cin >> ans;
		if (ans == 'C' || ans == 'c') {
			break;
		}
		else {
			cout << "Wrong answer! Please try again:";
		}
	}
	cout << "I didn't expect you to get that one correct." << endl;
	system("pause");
	system("cls");

	//主線推進部分
	cout << "Looking for Life Potion, huh?" << endl;
	system("pause");
	cout << "I give you only one clue." << endl;
	system("pause");
	cout << "Southwest." << endl;
	system("pause");
	system("cls");
	map_data[current_position_x][current_position_y] = 0;
	display();
	return;
}

void Field::Dark()
{
	//互動部分
	system("cls");
	cout << "Nice to see you all again! You must have seen me on the opening ceremony." << endl;
	system("pause");
	cout << "Just in case you don't know, I'm professor Lupin." << endl;
	system("pause");
	cout << "Which of you know what is the spell to battle a Boggart?" << endl;
	cout << "Option A. Riddikulus" << endl;
	cout << "Option B. Alohomora" << endl;
	cout << "Option C. Sectumsempra" << endl;
	system("pause");
	cout << "Please choose:";
	char ans;
	while (1) {
		cin >> ans;
		if (ans == 'A' || ans == 'a') {
			break;
		}
		else {
			cout << "Wrong answer! Please try again:";
		}
	}
	cout << "Correct answer! Well done!" << endl;
	system("pause");
	system("cls");

	//主線推進部分
	cout << "Are you sure " << player->getname() << "? You know the Patronum spell is a very advance magic." << endl;
	system("pause");
	cout << "Okay, if you insist." << endl;
	system("pause");
	cout << "Things are prepared. I'll wait for you at the 'northeast' of the Quidditch Pitch." << endl;
	system("pause");
	cout << "See you at midnight." << endl;
	system("pause");
	system("cls");
	map_data[current_position_x][current_position_y] = 0;
	display();
	return;
}

void Field::npc()
{
	system("cls");
	cout << "Hey, long time no see! It's me Hagrid!" << endl;
	system("pause");
	cout << "I guess you've already heard that the deathly hallows are in the forbidden forest." << endl;
	system("pause");
	cout << "I know you want to find them, but there is nothing much I can help you with." << endl;
	system("pause");
	cout << "Go through every battle is probably the best way." << endl;
	system("pause");
	cout << "Just like the saying goes \"April shower brings May flower\"." << endl;
	system("pause");
	cout << "Good luck, " << player->getname() << "." << endl;
	system("pause");
	system("cls");
	map_data[current_position_x][current_position_y] = 0;
	display();
	return;
}

void Field::Three()
{
	system("cls");
	cout << "You walk in the Three Broomsticks." << endl;
	system("pause");
	cout << "A strange man comes up and talks to you..." << endl;
	system("pause");
	cout << "[The man]  I have a good deal for you." << endl;
	cout << "[The man]  I can use my magic to make you level up by 1, but you have to give me $5000 as a return." << endl;
	system("pause");
	cout << "The money you have now: " << player->getmoney() << endl;
	cout << "\nPress 'y' to agree the deal, others to say no:";
	char ans;
	cin >> ans;
	switch (ans)
	{
	case 'y':
	case 'Y':
		if (player->getmoney() >= 5000) {
			player->setmoney(player->getmoney() - 5000);
			player->setLevel(player->getLevel() + 1);
			system("pause");
			system("cls");
			cout << "Deal completed!" << endl;
			system("pause");
			system("cls");
			map_data[current_position_x][current_position_y] = 0;
			display();
		}
		else {
			cout << "You don't have enough money!" << endl;
			system("pause");
			system("cls");
			display();
		}
		break;
	default:
		cout << "\nYou leave the Three Broomsticks." << endl;
		system("pause");
		system("cls");
		display();
		return;
		break;
	}
	return;
}

void Field::helperEquip()
{
	if (!player->backpack.empty()) {
		system("pause");
		system("cls");
		cout << "Press the index of the item you wanna equip on the helper wizard." << endl;
		cout << "(Helper wizard can only put on one item)" << endl;
		cout << "===============" << endl;
		cout << "   Backpack    " << endl;
		cout << "===============" << endl;
		for (int i = 0; i < player->backpack.size(); i++) {
			if (player->backpack[i]->type != 'c') {
				cout << "Index: " << i + 1 << endl;
				cout << setw(18) << "Name: " << player->backpack[i]->name << endl;
				cout << setw(18) << "Level Required: " << player->backpack[i]->level_required << endl;
				cout << setw(18) << "Effect: " << player->backpack[i]->effect << endl;
				cout << setw(18) << "Description: " << player->backpack[i]->description << endl;
				cout << endl;
			}
		}

		char c;
		WeaponItem* temp_w_on;
		ArmorItem* temp_a_on;
		string itemClass;

		while (c = _getch()) {
			if (c == '1' && player->backpack.size() >= 1) {
				switch (player->backpack[0]->type)
				{
				case 'w':
					temp_w_on = static_cast<WeaponItem*>(player->backpack[0]);
					itemClass = typeid(*temp_w_on).name();
					if (itemClass == "class WeaponItem") {
						if (player->backpack[0]->name == "Sword_of_Gryffindor") {
							playPoint[playPoint.size()-1]->attack += 30;
							cout << "\nSucessfully equipped!" << endl;
							system("pause");
						}
						else if (player->backpack[0]->name == "Elder_Wand") {
							playPoint[playPoint.size()-1]->attack += 100;
							cout << "\nSucessfully equipped!" << endl;
							system("pause");
						}
					}
					break;

				case 'a':
					temp_a_on = static_cast<ArmorItem*>(player->backpack[0]);
					itemClass = typeid(*temp_a_on).name();
					if (itemClass == "class ArmorItem") {
						if (player->backpack[0]->name == "Cloak_of_Invisibility") {
							playPoint[playPoint.size()-1]->defense += 30;
							cout << "\nSucessfully equipped!" << endl;
							system("pause");
						}
						else if (player->backpack[0]->name == "Patronum") {
							playPoint[playPoint.size()-1]->defense += 100;
							cout << "\nSucessfully equipped!" << endl;
							system("pause");
						}
					}
					break;

				default:
					break;
				}
			}

			else if (c == '2' && player->backpack.size() >= 2) {
				switch (player->backpack[1]->type)
				{
				case 'w':
					temp_w_on = static_cast<WeaponItem*>(player->backpack[1]);
					itemClass = typeid(*temp_w_on).name();
					if (itemClass == "class WeaponItem") {
						if (player->backpack[1]->name == "Sword_of_Gryffindor") {
							playPoint[playPoint.size()-1]->attack += 30;
							cout << "\nSucessfully equipped!" << endl;
							system("pause");
						}
						else if (player->backpack[1]->name == "Elder_Wand") {
							playPoint[playPoint.size()-1]->attack += 100;
							cout << "\nSucessfully equipped!" << endl;
							system("pause");
						}
					}
					break;

				case 'a':
					temp_a_on = static_cast<ArmorItem*>(player->backpack[1]);
					itemClass = typeid(*temp_a_on).name();
					if (itemClass == "class ArmorItem") {
						if (player->backpack[1]->name == "Cloak_of_Invisibility") {
							playPoint[playPoint.size()-1]->defense += 30;
							cout << "\nSucessfully equipped!" << endl;
							system("pause");
						}
						else if (player->backpack[1]->name == "Patronum") {
							playPoint[playPoint.size()-1]->defense += 100;
							cout << "\nSucessfully equipped!" << endl;
							system("pause");
						}
					}
					break;

				default:
					break;
				}
			}

			else if (c == '3' && player->backpack.size() >= 3) {
				switch (player->backpack[2]->type)
				{
				case 'w':
					temp_w_on = static_cast<WeaponItem*>(player->backpack[2]);
					itemClass = typeid(*temp_w_on).name();
					if (itemClass == "class WeaponItem") {
						if (player->backpack[2]->name == "Sword_of_Gryffindor") {
							playPoint[playPoint.size()-1]->attack += 30;
							cout << "\nSucessfully equipped!" << endl;
							system("pause");
						}
						else if (player->backpack[2]->name == "Elder_Wand") {
							playPoint[playPoint.size()-1]->attack += 100;
							cout << "\nSucessfully equipped!" << endl;
							system("pause");
						}
					}
					break;

				case 'a':
					temp_a_on = static_cast<ArmorItem*>(player->backpack[2]);
					itemClass = typeid(*temp_a_on).name();
					if (itemClass == "class ArmorItem") {
						if (player->backpack[2]->name == "Cloak_of_Invisibility") {
							playPoint[playPoint.size()-1]->defense += 30;
							cout << "\nSucessfully equipped!" << endl;
							system("pause");
						}
						else if (player->backpack[2]->name == "Patronum") {
							playPoint[playPoint.size()-1]->defense += 100;
							cout << "\nSucessfully equipped!" << endl;
							system("pause");
						}
					}
					break;

				default:
					break;
				}
			}

			else if (c == '4' && player->backpack.size() >= 4) {
				switch (player->backpack[3]->type)
				{
				case 'w':
					temp_w_on = static_cast<WeaponItem*>(player->backpack[3]);
					itemClass = typeid(*temp_w_on).name();
					if (itemClass == "class WeaponItem") {
						if (player->backpack[3]->name == "Sword_of_Gryffindor") {
							playPoint[playPoint.size()-1]->attack += 30;
							cout << "\nSucessfully equipped!" << endl;
							system("pause");
						}
						else if (player->backpack[3]->name == "Elder_Wand") {
							playPoint[playPoint.size()-1]->attack += 100;
							cout << "\nSucessfully equipped!" << endl;
							system("pause");
						}
					}
					break;

				case 'a':
					temp_a_on = static_cast<ArmorItem*>(player->backpack[3]);
					itemClass = typeid(*temp_a_on).name();
					if (itemClass == "class ArmorItem") {
						if (player->backpack[3]->name == "Cloak_of_Invisibility") {
							playPoint[playPoint.size()-1]->defense += 30;
							cout << "\nSucessfully equipped!" << endl;
							system("pause");
						}
						else if (player->backpack[3]->name == "Patronum") {
							playPoint[playPoint.size()-1]->defense += 100;
							cout << "\nSucessfully equipped!" << endl;
							system("pause");
						}
					}
					break;

				default:
					break;
				}
			}
			break;
		}
	}
}