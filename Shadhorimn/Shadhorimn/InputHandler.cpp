using namespace std;
#include <iostream>
#include "InputHandler.h"

InputHandler::InputHandler(PlayerCharacter* pc) {
	player_character = pc;
	number_of_frames_to_eat_inputs = 0;
}

void InputHandler::EatInputsForNumberOfFrames(int number_of_frames) {
	number_of_frames_to_eat_inputs = number_of_frames;
}

void InputHandler::Update() {
	if (sf::Joystick::isConnected(0))
	{
		bool a_button_current = sf::Joystick::isButtonPressed(0, a_button);
		bool b_button_current = sf::Joystick::isButtonPressed(0, b_button);
		bool x_button_current = sf::Joystick::isButtonPressed(0, x_button);
		bool y_button_current = sf::Joystick::isButtonPressed(0, y_button);

		bool right_bumper_button_current = sf::Joystick::isButtonPressed(0, right_bumper_button);
		bool left_bumper_button_current = sf::Joystick::isButtonPressed(0, left_bumper_button);
		bool select_button_current = sf::Joystick::isButtonPressed(0, select_button);
		bool start_button_current = sf::Joystick::isButtonPressed(0, start_button);

		float left_stick_horizontal = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
		float left_stick_vertical = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);

		float right_stick_horizontal = sf::Joystick::getAxisPosition(0, sf::Joystick::U);
		float right_stick_vertical = sf::Joystick::getAxisPosition(0, sf::Joystick::R);

		//right trigger is the negative one
		float right_trigger = sf::Joystick::getAxisPosition(0, sf::Joystick::Z);
		float left_trigger = sf::Joystick::getAxisPosition(0, sf::Joystick::Z);

		float dpad_horizontal = sf::Joystick::getAxisPosition(0, sf::Joystick::PovX);
		float dpad_vertical = sf::Joystick::getAxisPosition(0, sf::Joystick::PovY);

		float mystery = sf::Joystick::getAxisPosition(0, sf::Joystick::V);

		if (number_of_frames_to_eat_inputs > 0) {
			number_of_frames_to_eat_inputs--;
		} else if (player_character->hit_points > 0) {
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


			if (right_bumper_button_current && !right_bumper_button_previous) {
				player_character->HandleButtonRightBumperPress();
			}
			if (!right_bumper_button_current && right_bumper_button_previous) {
				player_character->HandleButtonRightBumperRelease();
			}

			if (left_bumper_button_current && !left_bumper_button_previous) {
				player_character->HandleButtonLeftBumperPress();
			}
			if (!left_bumper_button_current && left_bumper_button_previous) {
				player_character->HandleButtonLeftBumperRelease();
			}

			if (select_button_current && !select_button_previous) {
				player_character->HandleButtonSelectPress();
			}
			if (!select_button_current && select_button_previous) {
				player_character->HandleButtonSelectRelease();
			}

			if (start_button_current && !start_button_previous) {
				player_character->HandleButtonStartPress();
			}
			if (!start_button_current && start_button_previous) {
				player_character->HandleButtonStartRelease();
			}

			if ((left_stick_horizontal >= deadzone || left_stick_horizontal <= -deadzone) ||
				(left_stick_vertical >= deadzone || left_stick_vertical <= -deadzone)) {
				player_character->HandleLeftStickInput(left_stick_horizontal, left_stick_vertical);
			}
		}

		a_button_previous = a_button_current;
		b_button_previous = b_button_current;
		x_button_previous = x_button_current;
		y_button_previous = y_button_current;

		right_bumper_button_previous = right_bumper_button_current;
		left_bumper_button_previous = left_bumper_button_current;
		select_button_previous = select_button_current;
		start_button_previous = start_button_current;
	}
}