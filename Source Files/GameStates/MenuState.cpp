#include "MenuState.h"


MenuState::MenuState(GameStateManager* gsm) : GameState(gsm)
{
	for (int i = 0; i < sizeof(stuff) / sizeof(stuff[0]); i++){
		stuff[i].setFont(gsm->getFontByName("noodle"));
		// default 30
	}
	stuff[0].setString("Play Versus Friend");
	stuff[1].setString("Play Versus AI");
	stuff[2].setString("Play Online");
	stuff[3].setString("Exit");
}


MenuState::~MenuState()
{
}

void MenuState::update(sf::Time elapsed){
	sf::Vector2i mousepos = sf::Mouse::getPosition(*gsm->window);
	bool nowclicking = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

	for (int i = 0; i < sizeof(stuff) / sizeof(stuff[0]); i++){
		stuff[i].setPosition((800 - stuff[i].getLocalBounds().width) / 2, 150 + 100 * i);
	}

	for (int i = 0; i < sizeof(stuff) / sizeof(stuff[0]); i++){
		if (stuff[i].getGlobalBounds().contains(mousepos.x, mousepos.y)){
			stuff[i].setStyle(sf::Text::Bold | sf::Text::Underlined);
		}
		else{
			stuff[i].setStyle(sf::Text::Regular);
		}
	}
	if (!clickedlasttime && nowclicking){
		for (int i = 0; i < sizeof(stuff) / sizeof(stuff[0]); i++){
			if (stuff[i].getGlobalBounds().contains(mousepos.x, mousepos.y)){
				switch (i)
				{
				case 0:{
					gsm->setState(GameStatesIndex::playlocal);
					break;
				}
				case 1:{
					gsm->setState(GameStatesIndex::VSAi);
					break;
				}
				case 2:{
					break;
				}
				case 3:{
					gsm->exit();
					break;
				}
				default:
					break;
				}
			}
		}
	}

	clickedlasttime = nowclicking;
}

void MenuState::draw(){
	for (int i = 0; i < sizeof(stuff) / sizeof(stuff[0]); i++){
		gsm->window->draw(stuff[i]);
	}
}