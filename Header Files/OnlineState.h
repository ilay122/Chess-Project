#pragma once
#include "GameStateManager.h"
#include "GameState.h"
#include "GameBoard.h"
#include "Movement.h"
#include <regex>


enum currentStateOnline{
	enteringIP,
	queue,
	acceptofdecline,
	gamemode,
	endgamemode
};

class OnlineState :
	public GameState
{
private:
	std::shared_ptr<GameBoard> board;
	bool clickedlasttime;

	sf::Vector2i wowhat;
	std::shared_ptr<GamePiece> holdin;
	std::shared_ptr<std::vector<Movement>> movesavil;

	sf::TcpSocket sock;
	bool isConnected;

	currentStateOnline st;

	std::string input;
	int currentpointing;
	sf::Text text;

	sf::Text textstuff;

	sf::Text acceptenter;

	int counter;
	sf::Time time;

	sf::Text accept;
	sf::Text dec;
	bool clickedsomethinghere;

	bool shoulddrawwithout;
	bool mycolor;

	sf::Text mycoloris;
	sf::Text backtomainmenu;
	bool enemyleft;
public:
	OnlineState(GameStateManager* gsm);
	~OnlineState();
	virtual void update(sf::Time elapsed);
	virtual void draw();
	void handleKeyboardInput(std::string& str,int&);
};

