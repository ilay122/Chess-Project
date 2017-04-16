#pragma once
#include "GameStateManager.h"
#include "GameState.h"
#include "GameBoard.h"
#include "Movement.h"
#include "AIManager.h"

class GameBoard;

enum currentStateVSFriend{
	playmode,
	endgame
};

class PlayState :
	public GameState
{
private:
	std::shared_ptr<GameBoard> board;
	bool clickedlasttime;
	bool clickeda;
	bool shoulddrawwithout;
	sf::Vector2i wowhat;
	std::shared_ptr<GamePiece> holdin;
	std::shared_ptr<std::vector<Movement>> movesavil;

	currentStateVSFriend st;

	sf::Text text;
	sf::Text restart;
	sf::Text tomainmenu;

public:
	PlayState(GameStateManager* gsm);
	~PlayState();
	virtual void update(sf::Time elapsed);
	virtual void draw();
};

