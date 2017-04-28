#include "OnlineState.h"


OnlineState::OnlineState(GameStateManager* gsm) : GameState(gsm)
{
	board = std::shared_ptr<GameBoard>(new GameBoard((gsm)));
	this->st = currentStateOnline::enteringIP;
	text.setFont(gsm->getFontByName("noodle"));
	textstuff.setFont(gsm->getFontByName("noodle"));
	acceptenter.setFont(gsm->getFontByName("noodle"));
	
	accept.setFont(gsm->getFontByName("noodle"));
	dec.setFont(gsm->getFontByName("noodle"));

	mycoloris.setFont(gsm->getFontByName("noodle"));
	backtomainmenu.setFont(gsm->getFontByName("noodle"));
	

	accept.setString("Accept");
	dec.setString("Decline");

	this->isConnected = false;

	textstuff.setCharacterSize(24);
	acceptenter.setString("Connect");

	sock.setBlocking(false);

	this->currentpointing = 0;
	input = "";
}


OnlineState::~OnlineState()
{
}

void OnlineState::update(sf::Time elapsed){
	sf::Vector2i mousepos = sf::Mouse::getPosition(*gsm->window);
	bool nowclicking = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

	switch (st){
	case currentStateOnline::enteringIP:{
		handleKeyboardInput(input, currentpointing);
		text.setString(input);
		text.setPosition(sf::Vector2f((800 - text.getLocalBounds().width) / 2,400));
		acceptenter.setPosition(sf::Vector2f((800 - acceptenter.getLocalBounds().width) / 2, 400));
		acceptenter.move(0, 40);

		if (acceptenter.getGlobalBounds().contains(mousepos.x, mousepos.y)){
			acceptenter.setStyle(sf::Text::Bold | sf::Text::Underlined);
			if (nowclicking && !clickedlasttime){
				std::regex ip("^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$");
				if (std::regex_match(input, ip)){
					sf::Time timeout = sf::milliseconds(3000);
					sf::Socket::Status status = sock.connect(input, 42069, timeout);
					if (status == sf::Socket::Done)
					{
						isConnected = true;
						this->st = currentStateOnline::queue;
						std::cout << "connected to the server socket" << std::endl;
						return;
					}
					else{
						//SOCKETS ARE BLOCKING SO SHOULD CHECK CONNECTION TWICE BEFORE RULLING OUT
						sf::Socket::Status status = sock.connect(input, 42069, timeout);
						if (status == sf::Socket::Done)
						{
							isConnected = true;
							this->st = currentStateOnline::queue;
							std::cout << "connected to the server socket" << std::endl;
							return;
						}
						else{
							textstuff.setString("Could Not Connect");
						}
					}
				}
				else{
					textstuff.setString("Invalid IP Adress");
				}
			}
		}
		else{
			acceptenter.setStyle(sf::Text::Regular);
		}

		textstuff.setPosition(sf::Vector2f((800 - textstuff.getLocalBounds().width) / 2, 400));
		textstuff.move(0, 80);


		break;
	}
	case currentStateOnline::queue:{
		backtomainmenu.setString("To The\nMain Menu");
		backtomainmenu.setPosition(0,0);

		if (backtomainmenu.getGlobalBounds().contains(mousepos.x, mousepos.y)){
			backtomainmenu.setStyle(sf::Text::Bold | sf::Text::Underlined);
			if (nowclicking && !clickedlasttime){
				board->initilizeDefaultBoard();
				enemyleft = false;
				this->st = currentStateOnline::enteringIP;
				sock.disconnect();
				gsm->setState(GameStatesIndex::mainmenu);
				return;
			}
		}
		else{
			backtomainmenu.setStyle(sf::Text::Regular);
		}
		char data[128];
		std::size_t received;
		if (sock.receive(data, 128, received) == sf::Socket::Done)
		{
			data[received] = '\0';
			std::string normalstr(data);
			std::cout << "TEXT GOT:"<< normalstr << std::endl;
			if (normalstr == ("Accept Or Decline")){
				this->st = currentStateOnline::acceptofdecline;
				clickedsomethinghere = false;
				time = sf::Time::Zero;
				std::cout << "accept or decline my dude" << std::endl;
				break;
			}
		}


		std::string f = "In Queue";
		
		time += elapsed;
		if (time.asMilliseconds() >= 1000){
			counter++;
			time = sf::Time::Zero;
			counter %= 5;
		}
		for (int i = 0; i < counter; i++){
			f += ".";
		}
		textstuff.setString(f);
		textstuff.setPosition(sf::Vector2f((800 - textstuff.getLocalBounds().width) / 2, 400));
		break;
	}
	case currentStateOnline::acceptofdecline:{
		char data[128];
		std::size_t received;
		if (sock.receive(data, 128, received) == sf::Socket::Done)
		{
			data[received] = '\0';
			std::string normalstr(data);
			std::cout << normalstr << std::endl;
			if (normalstr == ("Back In Queue")){
				std::cout << "I need to go back into the queue" << std::endl;
				this->st = currentStateOnline::queue;
				time = sf::Time::Zero;
				return;
			}
			if (normalstr == "WHITE" || normalstr == "BLACK"){
				this->st = currentStateOnline::gamemode;
				std::cout << "got color" << std::endl;
				if (normalstr == "WHITE"){
					mycolor = WHITE;
				}
				else{
					mycolor = BLACK;
				}
				std::string colorthing="Your Color:\n";
				if (mycolor){
					colorthing += "Black";
					mycoloris.setColor(sf::Color::Black);
				}
				else{
					colorthing += "White";
					mycoloris.setColor(sf::Color::White);
				}
				mycoloris.setString(colorthing);
			}
		}
		time += elapsed;
		if (time.asMilliseconds() >= 11 * 1000){
			this->st = currentStateOnline::enteringIP;
			textstuff.setString("Failed to accept");
			time = sf::Time::Zero;
			try{
				sock.disconnect();
			}
			catch (...){}
			return;
		}
		accept.setPosition(400 - 50,400);
		dec.setPosition(400 + 30,400);

		
		if (!clickedsomethinghere){
			textstuff.setString("Please Accept or Decline !");
			if (accept.getGlobalBounds().contains(mousepos.x, mousepos.y)){
				accept.setStyle(sf::Text::Bold | sf::Text::Underlined);
				if (nowclicking && !clickedlasttime){
					std::string tosend = "Accept";
					sock.send(tosend.c_str(), tosend.size());
					clickedsomethinghere = true;
				}
			}
			else{
				accept.setStyle(sf::Text::Regular);
			}
			if (dec.getGlobalBounds().contains(mousepos.x, mousepos.y)){
				dec.setStyle(sf::Text::Bold | sf::Text::Underlined);
				if (nowclicking && !clickedlasttime){
					std::string tosend = "Decline";
					sock.send(tosend.c_str(), tosend.size());
					clickedsomethinghere = true;
					this->st = currentStateOnline::enteringIP;
					sock.disconnect();
				}
			}
			else{
				dec.setStyle(sf::Text::Regular);
			}
		}
		else{
			textstuff.setString("Waiting for opponent");
		}

		textstuff.setPosition((800 - textstuff.getLocalBounds().width) / 2, 200);

		break;
	}
	case currentStateOnline::gamemode:{

		bool isblackturn = board->isBlackTurn();
		if (board->isGameFinished){
			this->st = currentStateOnline::endgamemode;
			return;
		}
		if (isblackturn){
			text.setString("Current Turn : Black");
			text.setColor(sf::Color::Black);
		}
		else{
			text.setString("Current Turn : White");
			text.setColor(sf::Color::White);
		}
		if (board->isCheck){
			text.setString(text.getString() + "\nCheck Right Now");
		}

		if (mycolor == isblackturn){
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
				if (holdin->canGoThere(mousepos, board.get())){
					//std::cout << "I cant really go there" << std::endl;
					Movement mm(wowhat.x, wowhat.y, mousepos.x, mousepos.y);
					board->doMove(mm);
					std::string tosend = (std::to_string(wowhat.x) + std::to_string(wowhat.y) + std::to_string(mousepos.x) + std::to_string(mousepos.y));
					std::cout << "i need to send to the socket" << std::endl;
					sock.send(tosend.c_str(),tosend.size());
				}
				movesavil->clear();
				holdin = nullptr;
			}
		}
		else{
			char data[128];
			std::size_t received;
			if (sock.receive(data, 128, received) == sf::Socket::Done)
			{
				data[received] = '\0';
				std::string normalstr(data);
				if (normalstr == "Enemy Left"){
					this->st = currentStateOnline::endgamemode;
					text.setString("Enemy Left\nThe Game");
					enemyleft = true;
					return;
				}
				Movement mm(data[0] -'0', data[1]-'0', data[2]-'0', data[3]-'0');
				board->doMove(mm);
			}
		}

		break;
	}
	case currentStateOnline::endgamemode:{
		backtomainmenu.setString("To The\nMain Menu");
		backtomainmenu.setPosition(board->getBoardPosRelative().x, board->getBoardPosRelative().y);
		backtomainmenu.move(board->width * board->getBoardTileWidth() + board->getBoardTileWidth() / 2, 0);
		backtomainmenu.move(0, 100);

		if (backtomainmenu.getGlobalBounds().contains(mousepos.x, mousepos.y)){
			backtomainmenu.setStyle(sf::Text::Bold | sf::Text::Underlined);
			if (nowclicking && !clickedlasttime){
				board->initilizeDefaultBoard();
				enemyleft = false;
				this->st = currentStateOnline::enteringIP;
				sock.disconnect();
				gsm->setState(GameStatesIndex::mainmenu);
				return;
			}
		}
		else{
			backtomainmenu.setStyle(sf::Text::Regular);
		}

		if (!enemyleft){
			bool blackwon = !board->isBlackTurn();
			if (blackwon){
				text.setString("Game Over\nBlack Wins");
				text.setColor(sf::Color::Black);
			}
			else{
				text.setString("Game Over\nWhite Wins");
				text.setColor(sf::Color::White);
			}
		}
		break;
	}
	}

	clickedlasttime = nowclicking;
}
void OnlineState::draw(){
	switch (st){
	case currentStateOnline::enteringIP:{
		float width = text.getLocalBounds().width / text.getString().getSize();
		sf::RectangleShape tt(sf::Vector2f(width,width));
		tt.setPosition(text.getPosition());
		tt.move(currentpointing * width, text.getCharacterSize());
		gsm->window->draw(tt);
		gsm->window->draw(text);
		gsm->window->draw(acceptenter);
		gsm->window->draw(textstuff);
		break;
	}
	case currentStateOnline::queue:{
		gsm->window->draw(textstuff);
		gsm->window->draw(backtomainmenu);
		break;
	}
	case currentStateOnline::acceptofdecline:{
		gsm->window->draw(accept);
		gsm->window->draw(dec);
		gsm->window->draw(textstuff);
		break;
	}
	case currentStateOnline::gamemode:{
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
		mycoloris.setPosition(board->getBoardPosRelative().x, board->getBoardPosRelative().y);
		mycoloris.move(board->width * board->getBoardTileWidth() + board->getBoardTileWidth() / 2, 0);
		mycoloris.move(0, 200);
		gsm->window->draw(mycoloris);
		break;
	}
	case currentStateOnline::endgamemode:{
		board->draw();
		text.setPosition(board->getBoardPosRelative().x, board->getBoardPosRelative().y);
		text.move(board->width * board->getBoardTileWidth() + board->getBoardTileWidth() / 2, 0);
		gsm->window->draw(text);
		gsm->window->draw(backtomainmenu);
		break;
	}
	}
}

void OnlineState::handleKeyboardInput(std::string& str,int& wheree){
	auto events = gsm->events;
	for (int i = 0; i < events.size(); i++){
		sf::Event event = events[i];
		if (event.type == sf::Event::KeyPressed){
			if (event.key.code == sf::Keyboard::Right){
				wheree++;
				if (wheree > str.length())
					wheree = str.length();
			}
			if (event.key.code == sf::Keyboard::Left){
				wheree--;
				if (wheree < 0)
					wheree = 0;
			}
			
		}
		if (event.type == sf::Event::TextEntered){
			if (event.text.unicode < 128){
				if (event.text.unicode != 0x08 && event.text.unicode != 0x0D){//backspace
					char a = static_cast<char>(event.text.unicode);
					str.insert(wheree,1,a);
					wheree++;
					//str += a;
				}
				else{
					if (event.text.unicode != 0x0D && wheree != 0){
						if (str.length() != 0){
							str.erase(wheree-1, 1);
							wheree--;
							if (wheree < 0)
								wheree = 0;
						}
					}
				}
			}
		}
	}
}