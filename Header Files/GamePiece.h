#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "Movement.h"


class GameBoard;

#define BLACK true
#define WHITE false

enum class PieceTypes {Horsen=3, King=1000 , Queen=9 , Christian=3, Pleb=1 , Rook=5};

class GamePiece
{
protected:
	int x;
	int y;
	bool isblack;
	PieceTypes type;
	int playedmoves;
public:
	GamePiece(int x , int y,bool isblack);
	~GamePiece();
	inline PieceTypes getType(){ return type; };
	inline bool isBlack(){ return isblack; };
	inline void setPlayedMoves(int newvalue){ this->playedmoves = newvalue; };
	inline int getPlayedMoves(){ return this->playedmoves; };
	inline sf::Vector2i whereOnBoard(){ return sf::Vector2i(x,y); };
	virtual std::string textureToDraw() = 0;
	virtual std::shared_ptr<GamePiece> getCopyOfSelf() = 0;
	void setPosition(int x, int y);
	sf::Vector2i getPosition();
	inline void incPlayedMoves(){ playedmoves++; };
	virtual bool canGoThere(sf::Vector2i whereto, GameBoard* gb) = 0;
	std::shared_ptr<std::vector<Movement>> allPossibleMoves(GameBoard* gb);
};

