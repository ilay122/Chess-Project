#pragma once
#include "GamePiece.h"
#include <memory>
#include "GameBoard.h"
class Queen :
	public GamePiece
{
public:
	Queen(int x, int y, bool isblack);
	~Queen();
	std::string textureToDraw();
	std::shared_ptr<GamePiece> getCopyOfSelf();
	bool canGoThere(sf::Vector2i whereto, GameBoard* gb);
};

