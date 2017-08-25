using namespace std;
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Platform.h"
#include "Singleton.h"
#include "World.h"
#include "InputHandler.h"
#include "PlayerCharacter.h"
#include "Drone.h"
#include "Camera.h"

int main()
{
	float viewport_width = 1024.0f;
	float viewport_height = 768.0f;
	float window_width = viewport_width;
	float window_height = viewport_height;

	sf::Clock clock;
	sf::Time time = clock.getElapsedTime();

	sf::Int64 time_current = time.asMicroseconds();
	sf::Int64 time_previous = time.asMicroseconds();
	sf::Int64 frame_delta;
	int32_t cycles = 0;
	sf::Int64 start_time = time.asMicroseconds();

	Camera* camera = new Camera(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(viewport_width, viewport_height));

	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode::VideoMode((int)window_width, (int)window_height), "Shadhorimn");// , sf::Style::Fullscreen);
	//sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "Shadhorimn");// , sf::Style::Fullscreen);

	PlayerCharacter* main_character = new PlayerCharacter(window, sf::Vector2f(500.0f, 510.0f), sf::Vector2f(40.0f, 80.0f), true);
	InputHandler* input_handler = new InputHandler(main_character);

	SingletonAccess<World> TheWorld = Singleton<World>::Get();
	TheWorld->Init(window, camera, main_character);

	while (window->isOpen())
	{
		cycles++;

		sf::Event event; 
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
				window->close();
		}

		time = clock.getElapsedTime();
		time_current = time.asMicroseconds();
		frame_delta = time_current - time_previous;
		time_previous = time_current;

		input_handler->Update();

		TheWorld->Update(time_current, frame_delta);
	}

	return 0;
}