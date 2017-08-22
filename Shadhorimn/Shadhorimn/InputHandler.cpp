using namespace std;
#include <iostream>
#include "InputHandler.h"

InputHandler::InputHandler(PlayerCharacter* pc) {
	player_character = pc;
}

void InputHandler::Update() {
	if (sf::Joystick::isConnected(0))
	{
		bool a_button_current = sf::Joystick::isButtonPressed(0, a_button);
		bool b_button_current = sf::Joystick::isButtonPressed(0, b_button);
		bool x_button_current = sf::Joystick::isButtonPressed(0, x_button);
		bool y_button_current = sf::Joystick::isButtonPressed(0, y_button);

		if (a_button_current && !a_button_previous) {
			player_character->HandleButtonAPress();
		}
		if (!a_button_current && a_button_previous) {
			player_character->HandleButtonARelease();
		}

		if (b_button_current && !b_button_previous) {
			player_character->HandleButtonBPress();
		}
		if (!b_button_current && b_button_previous) {
			player_character->HandleButtonBRelease();
		}

		if (x_button_current && !x_button_previous) {
			player_character->HandleButtonXPress();
		}
		if (!x_button_current && x_button_previous) {
			player_character->HandleButtonXRelease();
		}

		if (y_button_current && !y_button_previous) {
			player_character->HandleButtonYPress();
		}
		if (!y_button_current && y_button_previous) {
			player_character->HandleButtonYRelease();
		}

		a_button_previous = a_button_current;
		b_button_previous = b_button_current;
		x_button_previous = x_button_current;
		y_button_previous = y_button_current;

		float left_stick_horizontal = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
		float left_stick_vertical = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);

		if ((left_stick_horizontal >= deadzone || left_stick_horizontal <= -deadzone) ||
			(left_stick_vertical >= deadzone || left_stick_vertical <= -deadzone)) {
			player_character->HandleLeftStickInput(left_stick_horizontal, left_stick_vertical);
		}

		float right_stick_horizontal = sf::Joystick::getAxisPosition(0, sf::Joystick::U);
		float right_stick_vertical = sf::Joystick::getAxisPosition(0, sf::Joystick::R);

		//right trigger is the negative one
		float right_trigger = sf::Joystick::getAxisPosition(0, sf::Joystick::Z);
		float left_trigger = sf::Joystick::getAxisPosition(0, sf::Joystick::Z);

		float dpad_horizontal = sf::Joystick::getAxisPosition(0, sf::Joystick::PovX);
		float dpad_vertical = sf::Joystick::getAxisPosition(0, sf::Joystick::PovY);

		float mystery = sf::Joystick::getAxisPosition(0, sf::Joystick::V);
	}
}