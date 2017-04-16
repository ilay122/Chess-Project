#pragma once
#include "GameStateManager.h"
#include "GameState.h"
class MenuState :
	public GameState
{
private:
	sf::Text stuff[4];
	bool clickedlasttime;
public:
	MenuState(GameStateManager* gsm);
	~MenuState();
	virtual void update(sf::Time elapsed);
	virtual void draw();
};

