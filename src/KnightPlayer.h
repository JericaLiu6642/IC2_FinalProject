#pragma once
#ifndef KNIGHTPLAYER_H
#define KNIGHTPLAYER_H

#include <string>
#include "NovicePlayer.h"

class KnightPlayer :public NovicePlayer
{
public:
	KnightPlayer();
	KnightPlayer(int);
	KnightPlayer(int, string);
	KnightPlayer(const KnightPlayer&);
	virtual void setLevel(int);
	virtual void specialSkill(NovicePlayer*);
	static NovicePlayer* unserialize(string);
};
#endif
