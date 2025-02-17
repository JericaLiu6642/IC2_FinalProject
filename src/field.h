#include<iostream>
#include<string>
#include<vector>
#include"BaseMonster.h"
#include"NovicePlayer.h"
#include"GoblinMonster.h"
#include"ZombieMonster.h"
#include"JWMonster.h"
#include"OrcPlayer.h"
#include"KnightPlayer.h"
#include"MagicianPlayer.h"
#include"Battle.h"
#include"Wand.h"
#include"Sword.h"
#include"Cloak.h"
#include"Patronum.h"
#include"LifePotion.h"
#include"Venom.h"
using namespace std;

#ifndef Field_H
#define Field_H
class Field {

public:

	Field(string, int, int, int, int, NovicePlayer*);
	Field(string, int, int, int, int, NovicePlayer*, string c);
		//Similar to first one, but the first parameter is name of file that stores map data
		//The second and third parameter are the current place(x, y) of player
		//The forth and fifth parameter are the(width, height) of vision
	~Field();

	vector<int> split(const string &, vector<int>&, const char, const char);

	bool move(char);
		//Move player to next position, the parameter is the direction
		//The return value indicates whether this move is legal or not
	bool moveUp(void);
	bool moveDown(void);
	bool moveLeft(void);
	bool moveRight(void);
		//The same as move(char), move player to next position
		//But the direction is determined

	//These methods get the information of the map
	int getCurrentPositionX(void) const;
	int getCurrentPositionY(void) const;
	int getVisionWidth(void) const;
	int getVisionHeight(void) const;
	string getMapName(void) const;
	int getMapSymbol(int, int);
		//Parameter is the position(x, y)
	void setPosition(int, int);
		//Set the position of player, prameters are position(x, y)
	void setMapSymbol(int, int, int);
		//Set the symbol on a specific position
		//The first parameter is symbol, the rest are position(x, y)
	void setVisionSize(int, int);
		//Set the size of vision, parameters are(width, height)
	void display(void) const;
		//Displaying the map
	void setPlayer(NovicePlayer*);
	void Events(int);
	void SetColor(int) const;
	void serialize();

	//地圖中非戰鬥事件
	void Charm();
	void Trans();
	void Potion();
	void npc();
	void Dark();
	void Three();

	//helper裝備
	void helperEquip();

	bool leave = false; //判斷是否該離開這個地圖

	vector<BaseMonster*> monPoint;
	vector<NovicePlayer*> playPoint;
	Wand* wand;
	Sword* sword;
	Cloak* cloak;
	Patronum* patronum;
	LifePotion* lifePotion;
	Venom* venom;

private:
	int map_row;
	int map_col;

	int** map_data;
		//The actual map data with symbols and items
		//This is a double dimension array of integers

	int current_position_x;
	int current_position_y;
		//Current position of player
		//This can be used to display the map(player is always on the center)
	
	string map_name;
		//The name of this map

	int vision_width;
	int vision_height;
		//The width and height of vision

	NovicePlayer* player;

};
#endif