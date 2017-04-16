#pragma once
#include "GamePiece.h"
#include <memory>
#include "GameBoard.h"

class Rook :
	public GamePiece
{
public:
	Rook(int x, int y, bool isblack);
	~Rook();
	std::string textureToDraw();
	std::shared_ptr<GamePiece> getCopyOfSelf();
	bool canGoThere(sf::Vector2i whereto, GameBoard* gb);
};

