#include "Christian.h"


Christian::Christian(int x, int y, bool isblack) : GamePiece(x, y, isblack)
{
	this->type = PieceTypes::Christian;
}


Christian::~Christian()
{
}

std::string Christian::textureToDraw(){
	std::string str = "";
	if (isblack)
		str += "black";
	else
		str += "white";
	str += "christian";
	return str;

}
std::shared_ptr<GamePiece> Christian::getCopyOfSelf(){
	std::shared_ptr<GamePiece> a(new Christian(x, y, isblack));
	a->setPlayedMoves(getPlayedMoves());
	return std::move(a);
}
bool Christian::canGoThere(sf::Vector2i whereto, GameBoard* gb){
	if (whereto.x >= gb->width || whereto.x < 0)return false;
	if (whereto.y >= gb->height || whereto.y < 0)return false;
	if (gb->hasPieceAt(whereto.y, whereto.x)){
		if (gb->getPieceAt(whereto.y, whereto.x)->isBlack() == this->isBlack())
			return false;
	}
	if (abs(whereto.y - this->y) != abs(whereto.x - this->x))return false;
	int dy = (whereto.y - this->y) / abs(whereto.y - this->y);
	int dx = (whereto.x - this->x) / abs(whereto.x - this->x);
	for (int i = 1; i < abs(whereto.y - this->y); i++){
		if (gb->getPieceAt(this->y + dy*i, this->x + dx*i))
			return false;
	}
	return true;
}