#pragma once
#ifndef MAGICIANPLAYER_H
#define MAGICIANPLAYER_H
#include <string>
#include "NovicePlayer.h"
#include "BaseMonster.h"

class MagicianPlayer :public NovicePlayer
{
public:
	MagicianPlayer();
	MagicianPlayer(int);
	MagicianPlayer(int, string);
	MagicianPlayer(const MagicianPlayer&);
	virtual void setLevel(int);
	virtual void specialSkill(BaseMonster*);
	static NovicePlayer* unserialize(string);
};
#endif