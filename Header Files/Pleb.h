#pragma once
#include "GamePiece.h"
#include <memory>
#include "GameBoard.h"

class Pleb :
	public GamePiece
{
public:
	Pleb(int x,int y,bool isblack);
	~Pleb();
	std::string textureToDraw();
	std::shared_ptr<GamePiece> getCopyOfSelf();
	bool canGoThere(sf::Vector2i whereto, GameBoard* gb);
	
};

