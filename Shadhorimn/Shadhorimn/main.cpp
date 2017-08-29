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
	enum GameStates
	{
		GAME_STATE_LOGOS,
		GAME_STATE_START_MENU,
		GAME_STATE_INITILIZATION,
		GAME_STATE_IN_GAME
	};
	GameStates GameState = GAME_STATE_LOGOS;

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

	Camera* camera = new Camera(sf::Vector2f(viewport_width / 2, viewport_height / 2), sf::Vector2f(viewport_width, viewport_height));

	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode::VideoMode((int)window_width, (int)window_height), "Shadhorimn");// , sf::Style::Fullscreen);
	//sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "Shadhorimn");// , sf::Style::Fullscreen);
	InputHandler* input_handler;
	SingletonAccess<World> TheWorld = Singleton<World>::Get();
	PlayerCharacter* main_character = new PlayerCharacter(window, sf::Vector2f(0.0f, 0.0f), sf::Vector2f(40.0f, 80.0f), true);
	input_handler = new InputHandler(main_character);

	sf::Texture logo_screen_texture;
	if (!logo_screen_texture.loadFromFile("Images/LogoScreen.png"))
		return -1;

	sf::Sprite logo_screen_sprite(logo_screen_texture);
	int logo_screen_sprite_transparency = 0;

	while (window->isOpen())
	{
		sf::Event event;

		time = clock.getElapsedTime();
		time_current = time.asMicroseconds();

		if (time_previous + microseconds_per_frame < time_current) {
			frame_delta = time_current - time_previous;
			time_previous = time_current;

			if (GameState == GAME_STATE_LOGOS) {
				window->clear();

				bool proceed = false;

				if (logo_screen_sprite_transparency >= 255) {
					logo_screen_sprite_transparency = 255;
				}
				else {
					logo_screen_sprite_transparency++;
				}

				logo_screen_sprite.setColor(sf::Color(255, 255, 255, logo_screen_sprite_transparency));

				window->draw(logo_screen_sprite);

				while (window->pollEvent(event))
				{
					if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
						window->close();

					if (event.key.code == sf::Keyboard::Return)
						proceed = true;
				}

				if (sf::Joystick::isButtonPressed(0, 0))
					proceed = true;

				if (proceed)
					GameState = GAME_STATE_START_MENU;

				window->display();
			}
			else if (GameState == GAME_STATE_START_MENU) {

				GameState = GAME_STATE_INITILIZATION;
			}
			else if (GameState == GAME_STATE_INITILIZATION) {
				TheWorld->Init(window, camera, main_character);

				GameState = GAME_STATE_IN_GAME;
			}
			else if (GameState == GAME_STATE_IN_GAME) {
				while (window->pollEvent(event))
				{
					if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
						window->close();
				}

				input_handler->Update();

				TheWorld->Update(time_current / 1000, frame_delta / 1000);

				if (sf::Joystick::isButtonPressed(0, 7) && main_character->hit_points <= 0) {
					GameState = GAME_STATE_INITILIZATION;
				}
			}
		}
	}

	return 0;
}