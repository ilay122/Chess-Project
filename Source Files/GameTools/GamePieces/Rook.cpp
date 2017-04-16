#include "Rook.h"


Rook::Rook(int x, int y, bool isblack) : GamePiece(x, y, isblack)
{
	this->type = PieceTypes::Rook;
}


Rook::~Rook()
{
}

std::string Rook::textureToDraw(){
	std::string str = "";
	if (isblack)
		str += "black";
	else
		str += "white";
	str += "rook";
	return str;

}
std::shared_ptr<GamePiece> Rook::getCopyOfSelf(){
	std::shared_ptr<GamePiece> a(new Rook(x, y, isblack));
	a->setPlayedMoves(getPlayedMoves());
	return std::move(a);
}
bool Rook::canGoThere(sf::Vector2i whereto, GameBoard* gb){
	if (whereto.x >= gb->width || whereto.x < 0)return false;
	if (whereto.y >= gb->height || whereto.y < 0)return false;
	if (gb->hasPieceAt(whereto.y, whereto.x)){
		if (gb->getPieceAt(whereto.y, whereto.x)->isBlack() == this->isBlack())
			return false;
	}
	if (whereto.x == this->x){
		if (whereto.y > this->y){
			for (int i = this->y + 1; i < whereto.y; i++){
				if (gb->hasPieceAt(i, whereto.x))
					return false;
			}
			return true;
		}
		else{
			for (int i = this->y - 1; i > whereto.y; i--){
				if (gb->hasPieceAt(i, whereto.x))
					return false;
			}
			return true;
		}
	}
	else if (whereto.y == this->y){
		if (whereto.x > this->x){
			for (int i = this->x + 1; i < whereto.x; i++){
				if (gb->hasPieceAt(whereto.y, i))
					return false;
			}
			return true;
		}
		else{
			for (int i = this->x - 1; i > whereto.x; i--){
				if (gb->hasPieceAt(whereto.y, i))
					return false;
			}
			return true;
		}
	}
	return false;
}