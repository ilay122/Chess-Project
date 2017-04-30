#include "PlayAIState.h"


PlayAIState::PlayAIState(GameStateManager* gsm) : GameState(gsm)
{
	board = std::shared_ptr<GameBoard>(new GameBoard((gsm)));
	this->aicolor = BLACK;
	this->playercolor = !aicolor;
	this->st = currentState::choosing;

	
	whiterect.setSize(sf::Vector2f(150, 150));
	whiterect.setPosition(150, 100);
	whiterect.setFillColor(sf::Color::White);

	blackrect.setSize(sf::Vector2f(150, 150));
	blackrect.setFillColor(sf::Color::Black);
	blackrect.setPosition(450 + 50, 100);

	text.setFont(gsm->getFontByName("noodle"));
	text.setCharacterSize(26);

	for (int i = 0; i < sizeof(difficultes) / sizeof(difficultes[0]); i++){
		difficultes[i].setFont(gsm->getFontByName("noodle"));
		difficultes[i].setPosition(130 + 175 * i, 400);
		difficultes[i].setCharacterSize(30);
	}
	chosenrect.setFillColor(sf::Color(0x24, 0x23, 0x23));
	chosenrect.setOutlineColor(sf::Color::White);
	chosenrect.setOutlineThickness(10);
	
	backtomainmenu.setFont(gsm->getFontByName("noodle"));

	start.setFont(gsm->getFontByName("noodle"));
	start.setString("Start Game");
	start.setCharacterSize(40);
	start.setPosition((800 - start.getGlobalBounds().width) / 2, 500);

	chosencolor.setString(">");
	chosencolor.setFont(gsm->getFontByName("noodle"));
	chosencolor.setFillColor(sf::Color::Red);
	chosencolor.setCharacterSize(40);
	chosencolor.setPosition(120, 100);

	difficultes[0].setString("Random");
	difficultes[1].setString("Easy");
	difficultes[2].setString("Medium");
	difficultes[3].setString("Hard");
	diffchosen = 3;
	chosen = 0;

	chosenrect.setSize(sf::Vector2f(difficultes[0].getGlobalBounds().width * 2, 30 * 1.5));
	chosenrect.setPosition(difficultes[0].getPosition().x - difficultes[0].getGlobalBounds().width / 2, difficultes[0].getPosition().y);
}


PlayAIState::~PlayAIState()
{
}
void PlayAIState::resetStuff(){
	difficultes[0].setString("Random");
	difficultes[1].setString("Easy");
	difficultes[2].setString("Medium");
	difficultes[3].setString("Hard");
	diffchosen = 3;
	chosen = 0;
	for (int i = 0; i < sizeof(difficultes) / sizeof(difficultes[0]); i++){
		difficultes[i].setStyle(sf::Text::Regular);
	}
	difficultes[0].setPosition(130 + 175 * 0, 400);
	chosenrect.setSize(sf::Vector2f(difficultes[0].getGlobalBounds().width * 2, 30 * 1.5));
	chosenrect.setPosition(difficultes[0].getPosition().x - difficultes[0].getGlobalBounds().width / 2, difficultes[0].getPosition().y);

	board->initilizeDefaultBoard();
}
void PlayAIState::update(sf::Time elapsed){
	bool isblackturn = board->isBlackTurn();

	sf::Vector2i mousepos = sf::Mouse::getPosition(*gsm->window);
	bool nowclicking = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

	
	if (this->st == currentState::choosing){
		backtomainmenu.setString("To The\nMain Menu");
		backtomainmenu.setPosition(0, 0);

		if (backtomainmenu.getGlobalBounds().contains(mousepos.x, mousepos.y)) {
			backtomainmenu.setStyle(sf::Text::Bold | sf::Text::Underlined);
			if (nowclicking && !clickedlasttime) {
				board->initilizeDefaultBoard();
				this->st = currentState::choosing;
				gsm->setState(GameStatesIndex::mainmenu);
				return;
			}
		}
		else {
			backtomainmenu.setStyle(sf::Text::Regular);
		}

		for (int i = 0; i < sizeof(difficultes) / sizeof(difficultes[0]); i++){
			if (difficultes[i].getGlobalBounds().contains(mousepos.x,mousepos.y) && nowclicking && !clickedlasttime){
				chosen = i;
				chosenrect.setSize(sf::Vector2f(difficultes[i].getGlobalBounds().width * 2, 30 * 1.5));
				chosenrect.setPosition(difficultes[i].getPosition().x - difficultes[i].getGlobalBounds().width / 2, difficultes[i].getPosition().y);
			}
		}
		if (whiterect.getGlobalBounds().contains(mousepos.x, mousepos.y) && nowclicking && !clickedlasttime){
			chosencolor.setPosition(120, 100);
			playercolor = WHITE;
			aicolor = BLACK;
		}
		if (blackrect.getGlobalBounds().contains(mousepos.x, mousepos.y) && nowclicking && !clickedlasttime){
			chosencolor.setPosition(450 + 50 - 30, 100);
			playercolor = BLACK;
			aicolor = WHITE;
		}
		if (start.getGlobalBounds().contains(mousepos.x, mousepos.y) && nowclicking && !clickedlasttime){
			if (chosen == 0){
				diffchosen = -1;
				std::cout << "easy" << std::endl;
			}
			if (chosen == 1){
				diffchosen = 1;
				std::cout << "easy" << std::endl;
			}
			if (chosen == 2){
				diffchosen = 3;
				std::cout << "medium" << std::endl;
			}
			if (chosen == 3){
				diffchosen = 5;
				std::cout << "hard" << std::endl;
			}
			std::cout << "is played black : " << playercolor << std::endl;
			this->st = currentState::playing;
		}
	}
	else if (this->st == currentState::playing){
		if (board->isGameFinished){
			this->st = currentState::endgamescreen;
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
		
		if (aicolor == isblackturn){
			//Movement randommove = AIManager::randomMoveFor(board.get(), aicolor);
			//board->doMove(randommove);
			if (diffchosen == -1){
				Movement m = AIManager::randomMoveFor(board.get(), aicolor);
				board->doMove(m);
			}
			else{
				if (!AIManager::started){
					if (aicolor == BLACK){
						std::thread th(AIManager::bestMoveForBlack, new GameBoard(gsm,board.get()), diffchosen);
						SetThreadPriority(th.native_handle(), REALTIME_PRIORITY_CLASS);
						SetPriorityClass(th.native_handle(), REALTIME_PRIORITY_CLASS);
						th.detach();//{remove duplication};
					}
					else{
						std::thread th(AIManager::bestMoveForWhite, new GameBoard(gsm, board.get()), diffchosen);
						SetThreadPriority(th.native_handle(), REALTIME_PRIORITY_CLASS);
						SetPriorityClass(th.native_handle(), REALTIME_PRIORITY_CLASS);
						th.detach();
					}

				}
				else if (AIManager::started && !AIManager::isRunning){
					Movement m = AIManager::m;
					board->doMove(m);
					AIManager::started = false;
				}
				
			}
		}
		else{
			if (!clickedlasttime && nowclicking){
				mousepos -= board->getBoardPosRelative();
				mousepos = mousepos / board->getBoardTileWidth();

				std::cout << mousepos.x << "    " << mousepos.y << std::endl;
				if (mousepos.x >= 0 && mousepos.x <= 7 && mousepos.y >= 0 && mousepos.y <= 7){
					holdin = board->getPieceAt(mousepos.y, mousepos.x);
					if (holdin != nullptr){
						if (isblackturn != holdin->isBlack() || holdin->isBlack() == aicolor){
							std::cout << "not his turn" << std::endl;
						}
						else{
							shoulddrawwithout = true;
							wowhat = sf::Vector2i(mousepos.x, mousepos.y);
							movesavil = holdin->allPossibleMoves(board.get());
							/*
							for (int i = movesavil->size()-1; i >= 0; i--){
								board->doMove(movesavil->at(i));
								if (board->isCheckRightNow(!aicolor)){
									movesavil->erase(movesavil->begin() + i);
								}
								board->popTurn();
							}
							*/
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
				movesavil->clear();
				holdin = nullptr;
			}
		}
	}
	else if (this->st == currentState::endgamescreen){
		bool blackwon = !board->isBlackTurn();
		if (blackwon){
			text.setString("Game Over\nBlack Wins");
			text.setFillColor(sf::Color::Black);
		}
		else{
			text.setString("Game Over\nWhite Wins");
			text.setFillColor(sf::Color::White);
		}
		difficultes[0].setPosition(text.getPosition().x,text.getPosition().y+100);
		difficultes[0].setString("Click On Me\nTo Start Restart\nThe Game");

		difficultes[1].setPosition(text.getPosition().x, text.getPosition().y + 200);
		difficultes[1].setString("Back To The\nMain Menu");


		for (int i = 0; i < sizeof(difficultes) / sizeof(difficultes[0]); i++){
			if (difficultes[i].getGlobalBounds().contains(mousepos.x, mousepos.y)){
				difficultes[i].setStyle(sf::Text::Bold);
			}
			else{
				difficultes[i].setStyle(sf::Text::Regular);
			}
			
		}
		
		if (difficultes[0].getGlobalBounds().contains(mousepos.x, mousepos.y) && nowclicking && !clickedlasttime){
			difficultes[0].setString("Random");
			difficultes[1].setString("Easy");
			board->initilizeDefaultBoard();
			this->st = currentState::playing;
		}

		if (difficultes[1].getGlobalBounds().contains(mousepos.x, mousepos.y) && nowclicking && !clickedlasttime){
			this->resetStuff();
			this->st = currentState::choosing;
			this->gsm->setState(GameStatesIndex::mainmenu);
			
		}

	}
	clickedlasttime = nowclicking;
}

void PlayAIState::draw(){
	if (this->st == currentState::choosing){
		gsm->window->draw(blackrect);
		gsm->window->draw(whiterect);

		gsm->window->draw(backtomainmenu);

		gsm->window->draw(chosenrect);
		for (int i = 0; i < sizeof(difficultes) / sizeof(difficultes[0]); i++){
			difficultes[i].setPosition(130+175 * i, 400);
			gsm->window->draw(difficultes[i]);
		}
		gsm->window->draw(chosencolor);
		gsm->window->draw(start);
	}
	else if (this->st == currentState::playing){
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
		text.setPosition(board->getBoardPosRelative().x,board->getBoardPosRelative().y);
		text.move(board->width * board->getBoardTileWidth() + board->getBoardTileWidth()/2, 0);
		gsm->window->draw(text);
	}
	else if (this->st == currentState::endgamescreen){
		board->draw();
		
		gsm->window->draw(text);
		gsm->window->draw(difficultes[0]);
		gsm->window->draw(difficultes[1]);
	}
}