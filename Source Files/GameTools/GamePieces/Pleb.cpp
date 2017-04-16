#include "Pleb.h"


Pleb::Pleb(int x,int y,bool isblack) : GamePiece(x,y,isblack)
{
	this->type = PieceTypes::Pleb;
}


Pleb::~Pleb()
{
}

std::string Pleb::textureToDraw(){
	std::string str = "";
	if (isblack)
		str += "black";
	else
		str += "white";
	str += "pleb";
	return str;
	
}
std::shared_ptr<GamePiece> Pleb::getCopyOfSelf(){
	std::shared_ptr<GamePiece> a(new Pleb(x, y, isblack));
	a->setPlayedMoves(getPlayedMoves());
	return std::move(a);
}

bool Pleb::canGoThere(sf::Vector2i whereto, GameBoard* gb){
	if (whereto.x >= gb->width || whereto.x < 0)return false;
	if (whereto.y >= gb->height || whereto.y < 0)return false;
	if (gb->hasPieceAt(whereto.y,whereto.x)){
		if (gb->getPieceAt(whereto.y, whereto.x)->isBlack() == this->isBlack())
			return false;
	}
	int movv = -1;
	if (this->isBlack()){
		movv *= -1;
	}
	if (this->y + movv == whereto.y && whereto.x == this->x && !gb->hasPieceAt(whereto.y, whereto.x)){
		return true;
	}
	if (abs(this->x - whereto.x) == 1 && this->y + movv == whereto.y && gb->hasPieceAt(whereto.y, whereto.x)){
		return true;
	}
	movv *= 2;
	if (this->getPlayedMoves() == 0){
		if (this->y + movv == whereto.y && this->x == whereto.x){
			if (!gb->hasPieceAt(whereto.y - movv / 2, whereto.x) && !gb->hasPieceAt(whereto.y, whereto.x))
				return true;
		}
		
	}

	return false;
}