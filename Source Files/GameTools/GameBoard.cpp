#include "GameBoard.h"


GameBoard::GameBoard(GameStateManager* gsm)
{
	this->width = 8;
	this->height = 8;
	this->isGameFinished = false;
	this->turnnumber = 1;
	// for now

	this->gsm = gsm;
	initilizeDefaultBoard();
	boardpos = sf::Vector2f(50, 50);
	this->tileheight = 65;
	this->tilewidth = 65;
	letters.setFont(gsm->getFontByName("noodle"));
	letters.setCharacterSize(24);
}
GameBoard::GameBoard(GameStateManager* gsm, GameBoard* b){
	this->width = 8;
	this->height = 8;
	// for now
	this->turnnumber = b->turnnumber;
	this->gsm = gsm;
	boardpos = sf::Vector2f(50, 50);
	this->tileheight = 65;
	this->tilewidth = 65;
	gameb = new std::vector<std::shared_ptr<GamePiece>>(height * width);
	/*
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			setPiece(i, j, std::shared_ptr<GamePiece>(nullptr));
		}
	}
	*/
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			if (b->getPieceAt(i, j).get() != nullptr){
				this->setPiece(i, j, b->getPieceAt(i, j)->getCopyOfSelf());
			}
		}
	}
	this->blackTurn = b->isBlackTurn();
}

GameBoard::~GameBoard()
{
}

void GameBoard::initilizeDefaultBoard(){
	this->width = 8;
	this->height = 8;
	this->turnnumber = 1;
	if (gameb != nullptr){
		free(gameb);
	}
	this->isCheck = false;
	this->isGameFinished = false;
	this->blackTurn = WHITE;
	gameb = new std::vector<std::shared_ptr<GamePiece>>(width * height);
	while (!sourcefrom.empty()){
		sourcefrom.pop();
	}
	while (!todest.empty()){
		todest.pop();
	}
	while (!themovment.empty()){
		themovment.pop();
	}

for (int i = 0; i < height; i++){
	for (int j = 0; j < width; j++){
		setPiece(i, j, std::shared_ptr<GamePiece>());
	}
}
for (int i = 0; i < width; i++){
	setPiece(6, i, std::shared_ptr<GamePiece>(new Pleb(i, 6, false)));
}

setPiece(7, 4, std::shared_ptr<GamePiece>(new King(4, 7, false)));
setPiece(7, 3, std::shared_ptr<GamePiece>(new Queen(3, 7, false)));
setPiece(7, 0, std::shared_ptr<GamePiece>(new Rook(0, 7, false)));
setPiece(7, 1, std::shared_ptr<GamePiece>(new Horsen(1, 7, false)));
setPiece(7, 2, std::shared_ptr<GamePiece>(new Christian(2, 7, false)));
setPiece(7, 5, std::shared_ptr<GamePiece>(new Christian(5, 7, false)));
setPiece(7, 6, std::shared_ptr<GamePiece>(new Horsen(6, 7, false)));
setPiece(7, 7, std::shared_ptr<GamePiece>(new Rook(7, 7, false)));


for (int i = 0; i < width; i++){
	setPiece(1, i, std::shared_ptr<GamePiece>(new Pleb(i, 1, true)));
}
setPiece(0, 4, std::shared_ptr<GamePiece>(new King(4, 0, true)));
setPiece(0, 3, std::shared_ptr<GamePiece>(new Queen(3, 0, true)));
setPiece(0, 0, std::shared_ptr<GamePiece>(new Rook(0, 0, true)));
setPiece(0, 1, std::shared_ptr<GamePiece>(new Horsen(1, 0, true)));
setPiece(0, 2, std::shared_ptr<GamePiece>(new Christian(2, 0, true)));
setPiece(0, 5, std::shared_ptr<GamePiece>(new Christian(5, 0, true)));
setPiece(0, 6, std::shared_ptr<GamePiece>(new Horsen(6, 0, true)));
setPiece(0, 7, std::shared_ptr<GamePiece>(new Rook(7, 0, true)));

}
void GameBoard::setPiece(int y, int x, std::shared_ptr<GamePiece> piece){
	this->gameb->at(y * width + x) = (piece);
}
std::shared_ptr<GamePiece> GameBoard::getPieceAt(int y, int x){
	if (y*width + x < 0 || y*width + x >64){
		return std::shared_ptr<GamePiece>();
	}
	return gameb->at(y * width + x);
}
void GameBoard::drawTiles(){
	sf::RectangleShape a(sf::Vector2f(tilewidth, tileheight));
	sf::Color tile1(sf::Color(255, 226, 192));
	sf::Color tile2(sf::Color(241, 158, 58));

	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			a.setPosition(boardpos);
			a.move(j * tilewidth, i * tileheight);
			if ((i % 2 + j % 2) % 2 == 0){
				a.setFillColor(tile1);
			}
			else{
				a.setFillColor(tile2);
			}
			gsm->window->draw(a);
		}
	}
	for (int i = 0; i < height; i++){
		letters.setString((char)('0' + height - i));
		letters.setPosition(boardpos.x - 24 + 10, boardpos.y);
		letters.move(0, i*tileheight + 12);
		gsm->window->draw(letters);
	}
	for (int i = 0; i < width; i++){
		letters.setString((char)('a' + i));
		letters.setPosition(boardpos.x, boardpos.y + height * tileheight);
		letters.move(i*tilewidth + 24, 0);
		gsm->window->draw(letters);
	}
	if (turnnumber != 1){
		sf::Color darkgreen(0, 100, 0);
		a.setFillColor(darkgreen);
		a.setPosition(this->getBoardPosRelative().x,this->getBoardPosRelative().y);
		Movement lastmove = themovment.top();
		a.move(lastmove.tox * tilewidth, lastmove.toy * tileheight);
		gsm->window->draw(a);
		a.setPosition(this->getBoardPosRelative().x, this->getBoardPosRelative().y);
		a.move(lastmove.fromx * tilewidth, lastmove.fromy * tileheight);
		gsm->window->draw(a);
	}
}
void GameBoard::draw(){
	drawTiles();
	sf::Sprite spr;
	sf::Vector2f scale = spr.getScale();
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			std::shared_ptr<GamePiece> ptr = (getPieceAt(i, j));
			if (ptr.get() != nullptr){
				std::string str = ptr->textureToDraw();
				spr.setTexture(gsm->getTextureByName(str));

				spr.setPosition(boardpos);
				spr.move(j*tilewidth, i*tileheight);
				gsm->window->draw(spr);
			}
		}
	}
}
void GameBoard::drawWithout(int ii, int jj, std::shared_ptr<std::vector<Movement>> myvector){
	drawTiles();
	if (!myvector->empty()){
		sf::RectangleShape a(sf::Vector2f(65, 65));
		a.setFillColor(sf::Color::Yellow);
		sf::Color darkyellow(204, 204, 0);
		for (int i = 0; i < myvector->size(); i++){
			Movement m = myvector->at(i);
			a.setFillColor(sf::Color::Yellow);
			if ((m.toy % 2 + m.tox % 2) % 2 == 0){
				a.setFillColor(darkyellow);
			}
			a.setPosition(m.tox * 65 + this->getBoardPosRelative().x, m.toy * 65 + this->getBoardPosRelative().y);
			gsm->window->draw(a);
		}

	}
	sf::Sprite spr;
	sf::Vector2f scale = spr.getScale();
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			if (ii == i && jj == j) continue;
			std::shared_ptr<GamePiece> ptr = (getPieceAt(i, j));
			if (ptr.get() != nullptr){
				std::string str = ptr->textureToDraw();
				spr.setTexture(gsm->getTextureByName(str));

				spr.setPosition(boardpos);
				spr.move(j*tilewidth, i*tileheight);
				gsm->window->draw(spr);
			}
		}
	}
}

void GameBoard::doMove(Movement m){
	std::shared_ptr<GamePiece> holdin = this->getPieceAt(m.fromy, m.fromx);
	if (holdin.get() == nullptr){
		//who wants some spaghetti
		std::cout << "stuck in this shit" << std::endl;
		return;
	}
	sourcefrom.push(holdin->getCopyOfSelf());
	todest.push(this->getPieceAt(m.toy, m.tox));

	this->clearTile(m.fromy, m.fromx);
	if (this->hasPieceAt(m.toy, m.tox) && this->getPieceAt(m.toy, m.tox)->getType() == PieceTypes::King){
		this->isGameFinished = true;
	}
	if (holdin->getType() == PieceTypes::Pleb && (m.toy == height - 1 || m.toy == 0)){
		bool whatisthecolor = holdin->isBlack();
		holdin = std::shared_ptr<GamePiece>(new Queen(0, 0, whatisthecolor));
	}
	holdin->setPosition(m.tox, m.toy);
	this->setPiece(m.toy, m.tox, holdin);
	holdin->incPlayedMoves();
	

	themovment.push(m);

	bool ischeckrightnow = isCheckRightNow(!this->isBlackTurn());
	this->isCheck = (ischeckrightnow || isCheckRightNow(this->isBlackTurn()));

	this->switchTurn();
}

bool GameBoard::isCheckRightNow(bool kingcolorindanger){
	std::vector<Movement> allmoves;
	sf::Vector2i kingpos;
	for (int i = 0; i < this->height; i++){
		for (int j = 0; j < this->width; j++){
			if (this->hasPieceAt(i, j) && this->getPieceAt(i, j)->isBlack() == !kingcolorindanger){
				std::shared_ptr<std::vector<Movement>> allmovesforpiece = this->getPieceAt(i, j)->allPossibleMoves(this);
				allmoves.insert(allmoves.end(), allmovesforpiece->begin(), allmovesforpiece->end());
			}
			if (this->hasPieceAt(i, j) && this->getPieceAt(i, j)->isBlack() == kingcolorindanger && this->getPieceAt(i, j)->getType() == PieceTypes::King){
				kingpos = this->getPieceAt(i, j)->getPosition();
			}
		}
	}
	bool ischecknow = false;
	for (int i = 0; i < allmoves.size(); i++){
		if (allmoves[i].tox == kingpos.x && allmoves[i].toy == kingpos.y){
			ischecknow = true;
			//std::cout << "this is check" << std::endl;
			break;
		}
	}

	//this->isCheck = ischecknow;
	return ischecknow;
}
Movement* GameBoard::popTurn(){
	if (this->turnnumber == 1) return nullptr;
	this->turnnumber -= 1;
	this->blackTurn = !this->blackTurn;
	Movement m = themovment.top();

	std::shared_ptr<GamePiece> tothing = todest.top();
	todest.pop();
	this->setPiece(m.toy, m.tox, tothing);
	if (this->hasPieceAt(m.toy, m.tox) && this->getPieceAt(m.toy, m.tox)->getType() == PieceTypes::King){
		this->isGameFinished = false;
	}
	std::shared_ptr<GamePiece> sourcething = sourcefrom.top();
	sourcefrom.pop();
	this->setPiece(m.fromy, m.fromx, sourcething);

	//this->isCheck = this->isCheckRightNow(!this->isBlackTurn());
	themovment.pop();

	return &m;
}
//BLACK-MAX;
//WHITE-MIN;
int GameBoard::evaluate(){
	int blackscore = 0;
	int whitescore = 0;
	for (int i = 0; i < this->height; i++){
		for (int j = 0; j < this->width; j++){
			if (this->hasPieceAt(i, j)){
				if (this->getPieceAt(i, j)->isBlack()){
					blackscore += (int)(this->getPieceAt(i, j)->getType());
				}
				else{
					whitescore += (int)(this->getPieceAt(i, j)->getType());
				}
			}
		}
	}
	return blackscore - whitescore;
}