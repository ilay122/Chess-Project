#pragma once
#include "GamePiece.h"
#include <memory>
#include "GameBoard.h"
class Horsen :
	public GamePiece
{
public:
	Horsen(int x, int y, bool isblack);
	~Horsen();
	std::string textureToDraw();
	std::shared_ptr<GamePiece> getCopyOfSelf();
	bool canGoThere(sf::Vector2i whereto, GameBoard* gb);
};

