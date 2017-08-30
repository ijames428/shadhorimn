//using namespace std;
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Platform.h"
#include "Singleton.h"
#include "World.h"
#include "InputHandler.h"
#include "PlayerCharacter.h"
#include "Drone.h"
#include "Camera.h"

int IncrementTransparency(int transparency);
void SetPreviousButtonValues();
void UpdateGameStateLogos();
void UpdateGameStateStartMenu();

void HandleClosingEvent();
bool WasButtonAPressed();
bool WasButtonAReleased();
bool WasButtonBPressed();
bool WasButtonBReleased();
bool WasButtonStartPressed();
bool WasButtonStartReleased();

enum GameStates
{
	GAME_STATE_LOGOS,
	GAME_STATE_START_MENU,
	GAME_STATE_INITILIZATION,
	GAME_STATE_IN_GAME,
	GAME_STATE_CREDITS
};
GameStates GameState = GAME_STATE_LOGOS;

float viewport_width;
float viewport_height;
float window_width;
float window_height;

sf::RenderWindow* window;
Camera* camera;

InputHandler* input_handler;
PlayerCharacter* main_character;

sf::Texture logo_screen_texture;
sf::Sprite logo_screen_sprite;
int logo_screen_sprite_transparency;
sf::Texture start_menu_background_texture;
sf::Sprite start_menu_background_sprite;
sf::Font ringbearer_font;
sf::Text title_text;
sf::Text start_text;

int a_button = 0;
int b_button = 1;
int start_button = 7;

bool a_button_current;
bool b_button_current;
bool start_button_current;

bool a_button_previous = false;
bool b_button_previous = false;
bool start_button_previous = false;

sf::Event event;

int main()
{
	sf::Clock clock;
	sf::Time time;
	viewport_width = 1280.0f;
	viewport_height = 720.0f;
	window_width = viewport_width;
	window_height = viewport_height;

	time = clock.getElapsedTime();

	int frames_per_second = 60;
	sf::Int64 microseconds_in_a_second = 1000000;
	sf::Int64 microseconds_per_frame = microseconds_in_a_second / frames_per_second;

	sf::Int64 time_current = time.asMicroseconds();
	sf::Int64 time_previous = time.asMicroseconds();
	sf::Int64 frame_delta;
	sf::Int64 start_time = time.asMicroseconds();

	camera = new Camera(sf::Vector2f(viewport_width / 2, viewport_height / 2), sf::Vector2f(viewport_width, viewport_height));

	window = new sf::RenderWindow(sf::VideoMode::VideoMode((int)window_width, (int)window_height), "Shadhorimn");// , sf::Style::Fullscreen);
	//sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "Shadhorimn");// , sf::Style::Fullscreen);
	main_character = new PlayerCharacter(window, sf::Vector2f(0.0f, 0.0f), sf::Vector2f(40.0f, 80.0f), true);
	input_handler = new InputHandler(main_character);

	if (!logo_screen_texture.loadFromFile("Images/LogoScreen.png"))
		return -1;

	logo_screen_sprite = sf::Sprite(logo_screen_texture);
	int logo_screen_sprite_transparency = 0;

	if (!start_menu_background_texture.loadFromFile("Images/StartMenuBackground.png"))
		return -1;

	start_menu_background_sprite = sf::Sprite(start_menu_background_texture);

	if (!ringbearer_font.loadFromFile("Images/RingbearerFont.ttf"))
		return -1;

	title_text = sf::Text("Shadhorimn", ringbearer_font, 90);
	title_text.setPosition(viewport_width / 2.0f - 260.0f, viewport_height / 2.0f - 200.0f);

	start_text = sf::Text("Press Start to begin", ringbearer_font);
	start_text.setPosition(viewport_width / 2.0f - 120.0f, viewport_height / 2.0f - 60.0f);

	while (window->isOpen())
	{
		time = clock.getElapsedTime();
		time_current = time.asMicroseconds();

		a_button_current = sf::Joystick::isButtonPressed(0, a_button);
		b_button_current = sf::Joystick::isButtonPressed(0, b_button);
		start_button_current = sf::Joystick::isButtonPressed(0, start_button);

		if (time_previous + microseconds_per_frame < time_current) {
			frame_delta = time_current - time_previous;
			time_previous = time_current;

			if (GameState == GAME_STATE_LOGOS) {
				UpdateGameStateLogos();
			} else if (GameState == GAME_STATE_START_MENU) {
				UpdateGameStateStartMenu();
			} else if (GameState == GAME_STATE_INITILIZATION) {
				Singleton<World>::Get()->Init(window, camera, main_character);
				GameState = GAME_STATE_IN_GAME;
			} else if (GameState == GAME_STATE_IN_GAME) {
				input_handler->Update();
				Singleton<World>::Get()->Update(time_current / 1000, frame_delta / 1000);

				if (main_character->hit_points <= 0) {
					if ((a_button_current && !a_button_previous) || (start_button_current && !start_button_previous)) {
						if (Singleton<World>::Get()->current_number_of_lives > 0) {
							Singleton<World>::Get()->current_number_of_lives--;
							GameState = GAME_STATE_INITILIZATION;
						} else {
							Singleton<World>::Get()->current_number_of_lives = 2;
							GameState = GAME_STATE_START_MENU;
						}
					}
				}

				HandleClosingEvent();
				SetPreviousButtonValues();
			}
		}
	}

	return 0;
}

void UpdateGameStateLogos() {
	window->clear();

	bool proceed = false;

	logo_screen_sprite_transparency = IncrementTransparency(logo_screen_sprite_transparency);
	logo_screen_sprite.setColor(sf::Color(255, 255, 255, logo_screen_sprite_transparency));
	window->draw(logo_screen_sprite);

	if (WasButtonAPressed() || WasButtonStartPressed()) {
		proceed = true;
	}

	if (proceed)
		GameState = GAME_STATE_START_MENU;

	HandleClosingEvent();
	SetPreviousButtonValues();

	window->display();
}

void UpdateGameStateStartMenu() {
	window->clear();

	window->draw(title_text);
	window->draw(start_text);

	if (WasButtonAPressed() || WasButtonStartPressed()) {
		GameState = GAME_STATE_INITILIZATION;
	}

	HandleClosingEvent();
	SetPreviousButtonValues();

	window->display();
}

int IncrementTransparency(int transparency) {
	if (transparency >= 255) {
		transparency = 255;
	}
	else {
		transparency++;
	}

	return transparency;
}

void SetPreviousButtonValues() {
	a_button_previous = a_button_current;
	b_button_previous = b_button_current;
	start_button_previous = start_button_current;
}

void HandleClosingEvent() {
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
			window->close();
	}
}

bool WasButtonAPressed() {
	return a_button_current && !a_button_previous;
}

bool WasButtonAReleased() {
	return !a_button_current && a_button_previous;
}

bool WasButtonBPressed() {
	return b_button_current && !b_button_previous;
}

bool WasButtonBReleased() {
	return !b_button_current && b_button_previous;
}

bool WasButtonStartPressed() {
	return start_button_current && !start_button_previous;
}

bool WasButtonStartReleased() {
	return !start_button_current && start_button_previous;
}