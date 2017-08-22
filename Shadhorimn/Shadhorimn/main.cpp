using namespace std;
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Platform.h"
#include "Singleton.h"
#include "World.h"
#include "InputHandler.h"
#include "PlayerCharacter.h"
#include "Drone.h"

int main()
{
	sf::Clock clock;
	sf::Time time = clock.getElapsedTime();

	sf::Int64 time_current = time.asMicroseconds();
	sf::Int64 time_previous = time.asMicroseconds();
	sf::Int64 frame_delta;
	int32_t cycles = 0;
	sf::Int64 start_time = time.asMicroseconds();

	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode::VideoMode(1024, 768), "Shadhorimn");// , sf::Style::Fullscreen);
	//sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "Shadhorimn");// , sf::Style::Fullscreen);

	SingletonAccess<World> TheWorld = Singleton<World>::Get();

	PlayerCharacter* main_character = new PlayerCharacter(window, sf::Vector2f(500.0f, 400.0f), sf::Vector2f(40.0f, 80.0f), true);
	InputHandler* input_handler = new InputHandler(main_character);
	TheWorld->main_character = main_character;

	std::vector<Platform*> platforms;
	platforms.push_back(new Platform(window, sf::Vector2f(600.0f, 370.0f), sf::Vector2f(200.0f, 1.0f)));
	platforms.push_back(new Platform(window, sf::Vector2f(0.0f, 600.0f), sf::Vector2f(1600.0f, 1.0f)));

	std::vector<Creature*> creatures;
	creatures.push_back(new Creature(window, sf::Vector2f(300.0f, 500.0f), sf::Vector2f(40.0f, 80.0f), true));
	creatures.push_back(new Creature(window, sf::Vector2f(200.0f, 500.0f), sf::Vector2f(40.0f, 80.0f), true));

	std::vector<Drone*> drones;
	drones.push_back(new Drone(window, sf::Vector2f(300.0f, 300.0f), sf::Vector2f(30.0f, 30.0f), false));
	drones.push_back(new Drone(window, sf::Vector2f(200.0f, 300.0f), sf::Vector2f(30.0f, 30.0f), false));

	while (window->isOpen())
	{
		cycles++;

		sf::Event event; 
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
				window->close();
		}

		window->clear();

		time = clock.getElapsedTime();
		time_current = time.asMicroseconds();
		frame_delta = time_current - time_previous;
		time_previous = time_current;

		main_character->Draw();
		for (int i = 0; i < (int)creatures.size(); i++) {
			creatures[i]->Draw();
		}
		for (int i = 0; i < (int)platforms.size(); i++) {
			platforms[i]->Draw();
		}
		for (int i = 0; i < (int)drones.size(); i++) {
			drones[i]->Draw();
		}

		input_handler->Update();
		main_character->Update(frame_delta);
		for (int i = 0; i < (int)creatures.size(); i++) {
			creatures[i]->Update(frame_delta);
		}
		for (int i = 0; i < (int)platforms.size(); i++) {
			platforms[i]->Update(frame_delta);
		}
		for (int i = 0; i < (int)drones.size(); i++) {
			drones[i]->Update(frame_delta);
			drones[i]->UpdateBehavior();
		}

		window->display();
	}

	return 0;
}