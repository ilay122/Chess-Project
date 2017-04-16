#pragma once
#include <SFML/Graphics.hpp>
#include "GamePiece.h"
#include "GameStateManager.h"
#include <memory>
#include <stack>

#include "Movement.h"

#include "Pleb.h"
#include "Horsen.h"
#include "King.h"
#include "Queen.h"
#include "Christian.h"
#include "Rook.h"


class GameBoard
{
private:
	GameStateManager* gsm;
	std::vector<std::shared_ptr<GamePiece>>* gameb;
	bool blackTurn;
	int tilewidth;
	int tileheight;
	sf::Vector2f boardpos;
	std::stack<std::shared_ptr<GamePiece>> sourcefrom;
	std::stack<std::shared_ptr<GamePiece>> todest;
	std::stack<Movement> themovment;
	sf::Text letters;
public:
	GameBoard(GameStateManager* gsm);
	GameBoard(GameStateManager* gsm , GameBoard* b);
	~GameBoard();
	void draw();
	void drawWithout(int ii, int jj, std::shared_ptr<std::vector<Movement>>);
	void drawTiles();
	int evaluate();
	void initilizeDefaultBoard();
	std::shared_ptr<GamePiece> getPieceAt(int y, int x);
	bool isCheckRightNow(bool kingcolorindanger);
	inline bool hasPieceAt(int y, int x){ return getPieceAt(y, x) != nullptr; };
	void setPiece(int y, int x, std::shared_ptr<GamePiece> piece);
	inline sf::Vector2i getBoardPosRelative(){ return sf::Vector2i((int)(boardpos.x), (int)(boardpos.y)); };
	inline int getBoardTileWidth(){ return tilewidth; };
	inline bool isBlackTurn(){ return blackTurn; };
	inline void switchTurn(){ blackTurn = !blackTurn; turnnumber++; };
	Movement* popTurn();
	inline void clearTile(int i, int j){ setPiece(i, j, std::shared_ptr<GamePiece>()); };
	void doMove(Movement m);
	int width;
	int height;
	int turnnumber;
	bool isGameFinished;
	bool isCheck;
};

