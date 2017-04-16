#pragma once
#include "Movement.h"
#include "GameBoard.h"
#include <time.h> 
class AIManager
{
public:
	static Movement randomMoveFor(GameBoard* gb, bool isblack);
	static void bestMoveForBlack(GameBoard* gb, int depth);
	static void bestMoveForWhite(GameBoard* gb, int depth);
	static int minimax(GameBoard* gb, int alphamax, int betamin, int depth,bool currentplayer);
	static bool isRunning;
	static bool started;
	static Movement m;
	~AIManager();
private:
	AIManager();
};

