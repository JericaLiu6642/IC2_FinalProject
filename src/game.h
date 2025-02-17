#pragma once
#ifndef GAME_H
#define GAME_H

#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>
#include<typeinfo>
#include<stdlib.h>
#include<conio.h>
#include<Windows.h>
#include"field.h"
#include"NovicePlayer.h"

using namespace std;

class game
{
public:
	game();
	void intro();
	void task();
	void run();
};



#endif