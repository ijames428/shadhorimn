#ifndef CREATURE_H
#define CREATURE_H

using namespace std;
#include <SFML/Graphics.hpp>
#include "RigidBody.h"

class Creature : public RigidBody {
	protected:
		float speed;
		float jump_power;
	public:
		Creature(sf::RenderWindow *window, sf::Vector2f position = sf::Vector2f(0.0f, 0.0f), sf::Vector2f dimensions = sf::Vector2f(0.0f, 0.0f), bool subject_to_gravity = true);
		sf::RectangleShape rectangle_shape;
		sf::RenderWindow *render_window;
		void Draw();
};

#endif