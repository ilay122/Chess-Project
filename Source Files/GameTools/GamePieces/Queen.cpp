#include "Queen.h"


Queen::Queen(int x, int y, bool isblack) : GamePiece(x, y, isblack)
{
	this->type = PieceTypes::Queen;
}


Queen::~Queen()
{
}
std::string Queen::textureToDraw(){
	std::string str = "";
	if (isblack)
		str += "black";
	else
		str += "white";
	str += "queen";
	return str;

}

std::shared_ptr<GamePiece> Queen::getCopyOfSelf(){
	std::shared_ptr<GamePiece> a(new Queen(x, y, isblack));
	a->setPlayedMoves(getPlayedMoves());
	return std::move(a);
}
bool Queen::canGoThere(sf::Vector2i whereto, GameBoard* gb){
	if (whereto.x >= gb->width || whereto.x < 0)return false;
	if (whereto.y >= gb->height || whereto.y < 0)return false;
	if (gb->hasPieceAt(whereto.y, whereto.x)){
		if (gb->getPieceAt(whereto.y, whereto.x)->isBlack() == this->isBlack())
			return false;
	}
	if (abs(whereto.y - this->y) == abs(whereto.x - this->x)){
		int dy = (whereto.y - this->y) / abs(whereto.y - this->y);
		int dx = (whereto.x - this->x) / abs(whereto.x - this->x);
		for (int i = 1; i < abs(whereto.y - this->y); i++){
			if (gb->getPieceAt(this->y + dy*i, this->x + dx*i))
				return false;
		}
		return true;
	}
	else{
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
	}
	return false;
}