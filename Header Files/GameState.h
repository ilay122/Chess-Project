#pragma once
#include <SFML/Graphics.hpp>

class GameStateManager;

class GameState
{
protected:
	GameStateManager* gsm;
public:
	GameState(GameStateManager* gsm);
	virtual ~GameState();
	virtual void update(sf::Time elapsed) = 0;
	virtual void draw() = 0;
};

