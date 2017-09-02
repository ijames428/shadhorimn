#ifndef GRUNT_H
#define GRUNT_H

using namespace std;
#include <SFML/Graphics.hpp>
#include "Creature.h"
#include "PlayerCharacter.h"
#include "Projectile.h"

class Grunt : public Creature {
private:
	float movement_speed;
	bool is_aggroed;
	float aggro_radius;
	float attack_radius;
	sf::Int64 time_of_last_attack;
	sf::Int64 duration_of_attack;
	sf::Int64 time_between_attacks;
	PlayerCharacter* target;
	RigidBody* HitBox;
public:
	Grunt(sf::RenderWindow *window, sf::Vector2f position = sf::Vector2f(0.0f, 0.0f), sf::Vector2f dimensions = sf::Vector2f(0.0f, 0.0f), bool subject_to_gravity = true);
	void UpdateBehavior(sf::Int64 curr_time);
	void Draw(sf::Vector2f camera_position);
};

#endif