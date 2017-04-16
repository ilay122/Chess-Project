#pragma once
#include "GamePiece.h"
#include <memory>
#include "GameBoard.h"
class King :
	public GamePiece
{
public:
	King(int x, int y, bool isblack);
	~King();
	std::string textureToDraw();
	std::shared_ptr<GamePiece> getCopyOfSelf();
	bool canGoThere(sf::Vector2i whereto, GameBoard* gb);
};

