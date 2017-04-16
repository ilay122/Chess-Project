#include "GamePiece.h"
#include "GameBoard.h"


GamePiece::GamePiece(int x,int y,bool isblack)
{
	this->x = x;
	this->y = y;
	this->isblack = isblack;
	this->playedmoves = 0;
}


GamePiece::~GamePiece()
{
}
void GamePiece::setPosition(int x, int y){
	this->x = x;
	this->y = y;
}

sf::Vector2i GamePiece::getPosition(){
	return sf::Vector2i(x, y);
}
std::shared_ptr<std::vector<Movement>> GamePiece::allPossibleMoves(GameBoard* gb){
	std::vector<Movement> myvector;
	for (int i = 0; i < gb->height; i++){
		for (int j = 0; j < gb->width; j++){
			if (this->canGoThere(sf::Vector2i(j, i), gb)){
				Movement newmove = Movement(this->x, this->y, j, i);
				myvector.push_back(newmove);
			}
		}
	}
	return std::make_shared<std::vector<Movement> >(std::move(myvector));
}