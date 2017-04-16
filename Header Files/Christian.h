#pragma once
#include "GamePiece.h"
#include <memory>
#include "GameBoard.h"
class Christian :
	public GamePiece
{
public:
	Christian(int x, int y, bool isblack);
	~Christian();
	std::string textureToDraw();
	std::shared_ptr<GamePiece> getCopyOfSelf();
	bool canGoThere(sf::Vector2i whereto, GameBoard* gb);
};

