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
		GAME_STATE_OPENING_CRAWL,
		GAME_STATE_INITILIZATION,
		GAME_STATE_IN_GAME,
		GAME_STATE_CREDITS
	};
	GameStates GameState = GAME_STATE_LOGOS;

	float viewport_width = 1280.0f;
	float viewport_height = 720.0f;
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

	sf::Texture start_menu_background_texture;
	if (!start_menu_background_texture.loadFromFile("Images/StartMenuBackground.png"))
		return -1;

	sf::Sprite start_menu_background_sprite(start_menu_background_texture);

	sf::Font ringbearer_font;
	if (!ringbearer_font.loadFromFile("Images/RingbearerFont.ttf"))
		return -1;

	sf::Text title_text("Shadhorimn", ringbearer_font, 90);
	title_text.setPosition(viewport_width / 2.0f - 260.0f, viewport_height / 2.0f - 200.0f);

	sf::Text start_text("Press Start to begin", ringbearer_font);
	start_text.setPosition(viewport_width / 2.0f - 120.0f, viewport_height / 2.0f - 60.0f);

	int a_button = 0;
	int b_button = 1;
	int start_button = 7;

	bool a_button_previous = false;
	bool b_button_previous = false;
	bool start_button_previous = false;

	while (window->isOpen())
	{
		sf::Event event;

		time = clock.getElapsedTime();
		time_current = time.asMicroseconds();

		bool a_button_current = sf::Joystick::isButtonPressed(0, a_button);
		bool b_button_current = sf::Joystick::isButtonPressed(0, b_button);
		bool start_button_current = sf::Joystick::isButtonPressed(0, start_button);


		if (time_previous + microseconds_per_frame < time_current) {
			frame_delta = time_current - time_previous;
			time_previous = time_current;

			if (GameState == GAME_STATE_LOGOS) {
				window->clear();

				bool proceed = false;

				if (logo_screen_sprite_transparency >= 255) {
					logo_screen_sprite_transparency = 255;
				} else {
					logo_screen_sprite_transparency++;
				}

				logo_screen_sprite.setColor(sf::Color(255, 255, 255, logo_screen_sprite_transparency));

				window->draw(logo_screen_sprite);

				cout << a_button_current << " " << a_button_previous << "\n";
				if ((a_button_current && !a_button_previous) || (start_button_current && !start_button_previous)) {
					proceed = true;
				}

				if (proceed)
					GameState = GAME_STATE_START_MENU;

				while (window->pollEvent(event))
				{
					if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
						window->close();

					if (event.key.code == sf::Keyboard::Return) {
						//proceed = true;
					}
				}

				a_button_previous = a_button_current;
				b_button_previous = b_button_current;
				start_button_previous = start_button_current;

				window->display();
			} else if (GameState == GAME_STATE_START_MENU) {
				window->clear();

				window->draw(title_text);
				window->draw(start_text);

				while (window->pollEvent(event))
				{
					if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
						window->close();

					if (event.key.code == sf::Keyboard::Return) {//sf::Joystick::isButtonPressed(0, 0)) {
						GameState = GAME_STATE_INITILIZATION;
					}
				}

				if ((a_button_current && !a_button_previous) || (start_button_current && !start_button_previous)) {
					GameState = GAME_STATE_INITILIZATION;
				}

				a_button_previous = a_button_current;
				b_button_previous = b_button_current;
				start_button_previous = start_button_current;

				window->display();
			} else if (GameState == GAME_STATE_INITILIZATION) {
				TheWorld->Init(window, camera, main_character);

				GameState = GAME_STATE_IN_GAME;
			} else if (GameState == GAME_STATE_IN_GAME) {
				while (window->pollEvent(event))
				{
					if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
						window->close();
				}

				input_handler->Update();

				TheWorld->Update(time_current / 1000, frame_delta / 1000);

				if (main_character->hit_points <= 0) {
					if ((a_button_current && !a_button_previous) || (start_button_current && !start_button_previous)) {
						if (TheWorld->number_of_lives > 0) {
							TheWorld->number_of_lives--;
							GameState = GAME_STATE_INITILIZATION;
						} else {
							TheWorld->number_of_lives = 2;
							GameState = GAME_STATE_START_MENU;
						}
					}
				}

				a_button_previous = a_button_current;
				b_button_previous = b_button_current;
				start_button_previous = start_button_current;
			}
		}
	}

	return 0;
}