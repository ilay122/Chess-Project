#include "PlayState.h"

PlayState::PlayState(GameStateManager* gsm) : GameState(gsm)
{
	board = std::shared_ptr<GameBoard>(new GameBoard((gsm)));
	
	this->st = currentStateVSFriend::playmode;

	text.setFont(gsm->getFontByName("noodle"));
	text.setCharacterSize(26);

	restart.setFont(gsm->getFontByName("noodle"));
	restart.setCharacterSize(26);

	tomainmenu.setFont(gsm->getFontByName("noodle"));
	tomainmenu.setCharacterSize(26);
}


PlayState::~PlayState()
{
	
}

void PlayState::update(sf::Time elapsed){
	bool isblackturn = board->isBlackTurn();

	sf::Vector2i mousepos = sf::Mouse::getPosition(*gsm->window);
	bool nowclicking = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
	bool nowclickinga = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	if (this->st == currentStateVSFriend::playmode){
		if (board->isGameFinished){
			this->st = currentStateVSFriend::endgame;
			return;
		}
		if (isblackturn){
			text.setString("Current Turn : Black");
			text.setFillColor(sf::Color::Black);
		}
		else{
			text.setString("Current Turn : White");
			text.setFillColor(sf::Color::White);
		}
		if (board->isCheck){
			text.setString(text.getString() + "\nCheck Right Now");
		}
		if (!clickedlasttime && nowclicking){
			mousepos -= board->getBoardPosRelative();
			mousepos = mousepos / board->getBoardTileWidth();
			
			std::cout << mousepos.x << "    " << mousepos.y << std::endl;
			if (mousepos.x >= 0 && mousepos.x <= 7 && mousepos.y >= 0 && mousepos.y <= 7){
				holdin = board->getPieceAt(mousepos.y, mousepos.x);
				if (holdin != nullptr){
					if (isblackturn != holdin->isBlack()){
						std::cout << "not his turn" << std::endl;
					}
					else{
						shoulddrawwithout = true;
						wowhat = sf::Vector2i(mousepos.x, mousepos.y);
						movesavil = holdin->allPossibleMoves(board.get());

						std::auto_ptr<GameBoard>copyboard(new GameBoard(gsm, board.get()));

						for (int i = movesavil->size() - 1; i >= 0; i--) {
							copyboard->doMove(movesavil->at(i));
							if (copyboard->isCheckRightNow(isblackturn)) {
								movesavil->erase(movesavil->begin() + i);
							}
							copyboard->popTurn();
						}

					}
				}
			}
		}
		else if (clickedlasttime && nowclicking){
			//std::cout << "holdin a game piece" << std::endl;
		}
		else if (clickedlasttime && !nowclicking && shoulddrawwithout){
			shoulddrawwithout = false;
			mousepos -= board->getBoardPosRelative();
			mousepos = mousepos / board->getBoardTileWidth();
			std::cout << "let go at " << mousepos.x << "   " << mousepos.y << std::endl;
			if (holdin->canGoThere(mousepos, board.get())){
				//std::cout << "I cant really go there" << std::endl;
				Movement mm(wowhat.x, wowhat.y, mousepos.x, mousepos.y);
				board->doMove(mm);
				std::cout << "piece moved to  " << holdin->getPosition().x << "   " << holdin->getPosition().y << " this piece played " << holdin->getPlayedMoves() << " turns" << std::endl;
			}

			//mousepos = moveTo   beforemove = moveFrom
			/*
			sf::Vector2i beforemove = holdin->getPosition();
			board->clearTile(beforemove.y, beforemove.x);
			holdin->setPosition(mousepos.x, mousepos.y);
			board->setPiece(mousepos.y, mousepos.x, holdin);
			holdin->incPlayedMoves();
			*/


			//board->switchTurn();
			movesavil->clear();
			holdin = nullptr;
		}

		if (nowclickinga && !clickeda){
			board->popTurn();
		}
	}
	else if (this->st == currentStateVSFriend::endgame){
		bool blackwon = !board->isBlackTurn();
		if (blackwon){
			text.setString("Game Over\nBlack Wins");
			text.setFillColor(sf::Color::Black);
		}
		else{
			text.setString("Game Over\nWhite Wins");
			text.setFillColor(sf::Color::White);
		}

		restart.setPosition(text.getPosition().x, text.getPosition().y + 100);
		restart.setString("Click On Me\nTo Start Restart\nThe Game");

		
		if (restart.getGlobalBounds().contains(mousepos.x, mousepos.y)){
			restart.setStyle(sf::Text::Bold);
		}
		else{
			restart.setStyle(sf::Text::Regular);
		}

		tomainmenu.setPosition(text.getPosition().x, text.getPosition().y + 200);
		tomainmenu.setString("Back To The\nMain Menu");
		if (tomainmenu.getGlobalBounds().contains(mousepos.x, mousepos.y)){
			tomainmenu.setStyle(sf::Text::Bold);
		}
		else{
			tomainmenu.setStyle(sf::Text::Regular);
		}

		if (restart.getGlobalBounds().contains(mousepos.x, mousepos.y) && nowclicking && !clickedlasttime){
			board->initilizeDefaultBoard();
			this->st = currentStateVSFriend::playmode;
		}
		if (tomainmenu.getGlobalBounds().contains(mousepos.x, mousepos.y) && nowclicking && !clickedlasttime){
			board->initilizeDefaultBoard();
			this->st = currentStateVSFriend::playmode;
			this->gsm->setState(GameStatesIndex::mainmenu);
		}

		
	}

	clickeda = nowclickinga;
	clickedlasttime = nowclicking;
}
void PlayState::draw(){
	if (this->st == currentStateVSFriend::playmode){
		if (shoulddrawwithout){
			board->drawWithout(wowhat.y, wowhat.x, movesavil);

			sf::Sprite piece;
			piece.setTexture(gsm->getTextureByName(holdin->textureToDraw()));
			sf::Vector2i mousepos = sf::Mouse::getPosition(*gsm->window);
			mousepos -= sf::Vector2i(65, 65) / 2;
			piece.setPosition(mousepos.x, mousepos.y);
			gsm->window->draw(piece);
		}
		else{
			board->draw();
		}
		text.setPosition(board->getBoardPosRelative().x, board->getBoardPosRelative().y);
		text.move(board->width * board->getBoardTileWidth() + board->getBoardTileWidth() / 2, 0);
		gsm->window->draw(text);
	}
	else if (this->st == currentStateVSFriend::endgame){
		board->draw();
		gsm->window->draw(text);
		gsm->window->draw(restart);
		gsm->window->draw(tomainmenu);
	}
}