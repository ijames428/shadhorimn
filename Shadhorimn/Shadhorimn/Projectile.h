#ifndef PROJECTILE_H
#define PROJECTILE_H

using namespace std;
#include <SFML/Graphics.hpp>
#include "RigidBody.h"
#include "Creature.h"

class Projectile : public RigidBody {
protected:
	sf::Vector2f fired_velocity;
	bool is_active;
	sf::Int64 current_time;
	sf::Int64 fired_time;
	sf::Vector2f fired_position;
	sf::Int64 length_of_life;
	sf::Int64 range;
	float speed;
public:
	Projectile(sf::RenderWindow *window, sf::Vector2f position = sf::Vector2f(0.0f, 0.0f), sf::Vector2f dimensions = sf::Vector2f(0.0f, 0.0f), bool subject_to_gravity = true);
	sf::CircleShape circle_shape;
	sf::RenderWindow *render_window;
	void Draw(sf::Vector2f camera_position);
	void UpdateProjectile(sf::Int64 curr_time);
	void Fire(sf::Int64 curr_time, sf::Vector2f position = sf::Vector2f(0.0f, 0.0f), sf::Vector2f velocity = sf::Vector2f(0.0f, 0.0f));
};

#endif