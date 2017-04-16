#include "King.h"


King::King(int x, int y, bool isblack) : GamePiece(x, y, isblack)
{
	this->type = PieceTypes::King;
}


King::~King()
{
}

std::string King::textureToDraw(){
	std::string str = "";
	if (isblack)
		str += "black";
	else
		str += "white";
	str += "king";
	return str;

}

std::shared_ptr<GamePiece> King::getCopyOfSelf(){
	std::shared_ptr<GamePiece> a(new King(x, y, isblack));
	a->setPlayedMoves(getPlayedMoves());
	return std::move(a);
}
bool King::canGoThere(sf::Vector2i whereto, GameBoard* gb){
	if (whereto.x >= gb->width || whereto.x < 0)return false;
	if (whereto.y >= gb->height || whereto.y < 0)return false;
	if (gb->hasPieceAt(whereto.y, whereto.x)){
		if (gb->getPieceAt(whereto.y, whereto.x)->isBlack() == this->isBlack())
			return false;
	}
	return (abs(whereto.x - this->x) <= 1 && abs(whereto.y - this->y) <= 1);
}