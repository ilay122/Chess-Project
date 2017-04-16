#include "GameStateManager.h"


GameStateManager::GameStateManager(sf::RenderWindow* window)
{
	this->window = window;
	this->running = true;
	this->gamestates = new std::vector<std::shared_ptr<GameState>>();

	window->setKeyRepeatEnabled(false);
	
	this->currentstate = GameStatesIndex::mainmenu;
}

void GameStateManager::loadContent(){
	loadFont("noodle", "data/big_noodle_titling.ttf");

	std::shared_ptr<GameState> x = std::shared_ptr<GameState>(new PlayState(this));
	this->addState(x);
	std::shared_ptr<GameState> y = std::shared_ptr<GameState>(new PlayAIState(this));
	this->addState(y);
	std::shared_ptr<GameState> z = std::shared_ptr<GameState>(new MenuState(this));
	this->addState(z);

	std::string directory = "data/Pics/";
	HANDLE dir;
	WIN32_FIND_DATA file_data;
	if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
		return; /* No files found */

	do {
		const std::string file_name = file_data.cFileName;

		const std::string full_file_name = directory + "/" + file_name;
		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
		if (is_directory)
			continue;
		std::string withoutend = file_name.substr(0, file_name.find("."));
		loadTexture(withoutend, full_file_name);
		std::cout << withoutend << std::endl;
		std::cout << full_file_name << std::endl;
		
	} while (FindNextFile(dir, &file_data));
	FindClose(dir);
	
}
void GameStateManager::exit(){
	this->running = false;
}
int GameStateManager::getpreviousState(){
	return this->laststate;
}
void GameStateManager::setState(int state){
	laststate = state;
	currentstate = state;
}

bool GameStateManager::isRunning(){
	return running;
}
void GameStateManager::draw(){
	window->clear(sf::Color(0x24, 0x23, 0x23));
	gamestates->at(currentstate)->draw();
	window->display();
	events.clear();
}
void GameStateManager::update(){
	sf::Time elapsed = clock.restart();
	gamestates->at(currentstate)->update(elapsed);
}
void GameStateManager::drawState(int st){
	gamestates->at(st)->draw();
	
}

void GameStateManager::addState(std::shared_ptr<GameState> gs){
	gamestates->push_back(gs);
}

void GameStateManager::loadTexture(std::string texname, std::string path){
	sf::Texture tex;
	tex.loadFromFile(path);
	textures[texname] = tex;
}

void GameStateManager::loadFont(std::string fontname, std::string path){
	sf::Font fontt;
	fontt.loadFromFile(path);
	fonts[fontname] = fontt;
}

sf::Font& GameStateManager::getFontByName(std::string name){
	if (fonts.find(name) != fonts.end()){
		std::cout << fonts.size() << std::endl;
		return fonts.at(name);
	}
	return sf::Font();
}

sf::Texture& GameStateManager::getTextureByName(std::string name){
	if (textures.find(name) != textures.end()){
		return textures.at(name);
	}
	return sf::Texture();
}

GameStateManager::~GameStateManager()
{
}