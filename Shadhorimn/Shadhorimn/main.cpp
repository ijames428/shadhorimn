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

	int frames_per_second = 60;
	sf::Int64 microseconds_in_a_second = 1000000;
	sf::Int64 microseconds_per_frame = microseconds_in_a_second / frames_per_second;

	sf::Int64 time_current = time.asMicroseconds();
	sf::Int64 time_previous = time.asMicroseconds();
	sf::Int64 frame_delta;
	sf::Int64 start_time = time.asMicroseconds();

	Camera* camera = new Camera(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(viewport_width, viewport_height));

	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode::VideoMode((int)window_width, (int)window_height), "Shadhorimn");// , sf::Style::Fullscreen);
	//sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "Shadhorimn");// , sf::Style::Fullscreen);

	PlayerCharacter* main_character = new PlayerCharacter(window, sf::Vector2f(0.0f, 0.0f), sf::Vector2f(40.0f, 80.0f), true);
	InputHandler* input_handler = new InputHandler(main_character);

	SingletonAccess<World> TheWorld = Singleton<World>::Get();
	TheWorld->Init(window, camera, main_character);

	while (window->isOpen())
	{
		time = clock.getElapsedTime();
		time_current = time.asMicroseconds();

		if (time_previous + microseconds_per_frame < time_current) {
			sf::Event event;
			while (window->pollEvent(event))
			{
				if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
					window->close();
			}

			frame_delta = time_current - time_previous;
			time_previous = time_current;

			input_handler->Update();

			TheWorld->Update(time_current / 1000, frame_delta / 1000);
		}
	}

	return 0;
}