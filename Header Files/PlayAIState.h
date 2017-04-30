#pragma once
#include "GameStateManager.h"
#include "GameState.h"
#include "GameBoard.h"
#include "Movement.h"
#include "AIManager.h"
#include <thread>

enum currentState{
	choosing,
	playing,
	endgamescreen
};

class PlayAIState :
	public GameState
{
private:
	std::shared_ptr<GameBoard> board;
	bool clickedlasttime;
	bool shoulddrawwithout;
	sf::Vector2i wowhat;
	std::shared_ptr<GamePiece> holdin;
	std::shared_ptr<std::vector<Movement>> movesavil;
	int state;
	bool aicolor;
	bool playercolor;
	currentState st;


	//UI STUFF
	sf::RectangleShape blackrect;
	sf::RectangleShape whiterect;
	sf::RectangleShape chosenrect;
	bool playercolorchosen;
	sf::Text chosencolor;

	sf::Text backtomainmenu;
	sf::Text text;
	sf::Text start;

	sf::Text difficultes[4];
	int diffchosen;
	int chosen;
	void resetStuff();
public:
	PlayAIState(GameStateManager* gsm);
	~PlayAIState();
	virtual void update(sf::Time elapsed);
	virtual void draw();
};

