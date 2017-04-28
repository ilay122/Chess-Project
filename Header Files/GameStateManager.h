#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "GameState.h"
#include "PlayState.h"
#include "PlayAIState.h"
#include "MenuState.h"
#include "OnlineState.h"
#include <Windows.h>
#include <iostream>
#include <regex>


#define WIDTH 800
#define HEIGHT 600


enum GameStatesIndex{
	playlocal = 0,
	VSAi=1,
	mainmenu=2,
	onlinegame=3
};

class GameStateManager
{
private:
	bool running;
	int laststate;
	int currentstate;
	std::map<std::string, sf::Texture> textures;
	std::map<std::string, sf::Font> fonts;
	std::vector<std::shared_ptr<GameState>>* gamestates;
	sf::Clock clock;

public:
	GameStateManager(sf::RenderWindow* window);
	~GameStateManager();
	void update();
	void draw();
	void setState(int state);
	void addState(std::shared_ptr<GameState> gs);
	bool isRunning();
	void drawState(int st);
	int getpreviousState();
	void loadContent();
	void loadTexture(std::string texname, std::string path);
	void loadFont(std::string fontname, std::string path);
	sf::Texture& getTextureByName(std::string name);
	sf::Font& getFontByName(std::string name);
	void exit();

	sf::RenderWindow* window;
	std::vector<sf::Event> events;
};

