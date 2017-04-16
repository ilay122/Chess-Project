#include "Horsen.h"


Horsen::Horsen(int x, int y, bool isblack) : GamePiece(x, y, isblack)
{
	this->type = PieceTypes::Horsen;
}


Horsen::~Horsen()
{
}

std::string Horsen::textureToDraw(){
	std::string str = "";
	if (isblack)
		str += "black";
	else
		str += "white";
	str += "horsen";
	return str;

}
std::shared_ptr<GamePiece> Horsen::getCopyOfSelf(){
	std::shared_ptr<GamePiece> a(new Horsen(x, y, isblack));
	a->setPlayedMoves(getPlayedMoves());
	return std::move(a);
}
bool Horsen::canGoThere(sf::Vector2i whereto, GameBoard* gb){
	if (whereto.x >= gb->width || whereto.x < 0)return false;
	if (whereto.y >= gb->height || whereto.y < 0)return false;
	if (gb->hasPieceAt(whereto.y, whereto.x)){
		if (gb->getPieceAt(whereto.y, whereto.x)->isBlack() == this->isBlack())
			return false;
	}
	return (abs(whereto.x - this->x) == 2 && abs(whereto.y - this->y) == 1 || abs(whereto.x - this->x) == 1 && abs(whereto.y - this->y) == 2);
}