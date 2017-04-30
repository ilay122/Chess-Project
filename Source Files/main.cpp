#include <SFML/Graphics.hpp>
#include "GameStateManager.h"

#define WIDTH 800
#define HEIGHT 600

int main()
{
	srand(time(NULL));
	sf::RenderWindow* window=new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Chess");
	GameStateManager gsm(window);

	gsm.loadContent();

	window->setFramerateLimit(60);
	
	while (gsm.isRunning() && window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed){
				window->close();
				break;
			}
			gsm.events.push_back(event);

		}
		
		gsm.update();

		gsm.draw();
	}

	return 0;
}