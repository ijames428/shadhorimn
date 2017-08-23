#ifndef DRONE_H
#define DRONE_H

using namespace std;
#include <SFML/Graphics.hpp>
#include "Creature.h"
#include "PlayerCharacter.h"

class Drone : public Creature {
private:
	float movement_speed;
	bool is_aggroed;
	float aggro_radius;
	PlayerCharacter* target;
public:
	Drone(sf::RenderWindow *window, sf::Vector2f position = sf::Vector2f(0.0f, 0.0f), sf::Vector2f dimensions = sf::Vector2f(0.0f, 0.0f), bool subject_to_gravity = true);
	void UpdateBehavior();
	void Draw(sf::Vector2f camera_position);
};

#endif