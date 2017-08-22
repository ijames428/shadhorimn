#ifndef PLAYERCHARACTER_H
#define PLAYERCHARACTER_H

using namespace std;
#include <SFML/Graphics.hpp>
#include "Creature.h"

class PlayerCharacter : public Creature {
	private:
		RigidBody* HitBox;
	public:
		PlayerCharacter(sf::RenderWindow *window, sf::Vector2f position = sf::Vector2f(0.0f, 0.0f), sf::Vector2f dimensions = sf::Vector2f(0.0f, 0.0f), bool subject_to_gravity = true);
		void HandleLeftStickInput(float horizontal, float vertical);
		void HandleButtonAPress();
		void HandleButtonARelease();
		void HandleButtonBPress();
		void HandleButtonBRelease();
		void HandleButtonXPress();
		void HandleButtonXRelease();
		void HandleButtonYPress();
		void HandleButtonYRelease();
};

#endif