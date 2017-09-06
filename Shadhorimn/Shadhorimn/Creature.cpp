using namespace std;
#include <iostream>
#include "Creature.h"
#include "Singleton.h"
#include "World.h"

Creature::Creature(sf::RenderWindow *window, sf::Vector2f position, sf::Vector2f dimensions, bool subject_to_gravity) : RigidBody::RigidBody(position, dimensions, subject_to_gravity) {
	entity_type = "Creature"; 
	hit_points = 1;

	speed = 2.0f;
	jump_power = 1.0f;

	current_time = 0;
	hit_stun_start_time = 0;
	hit_stun_duration = 0;

	render_window = window;

	sf::RectangleShape shape(dimensions);
	shape.setFillColor(sf::Color::Red);
	shape.setPosition(position);

	rectangle_shape = shape;
}

void Creature::Draw(sf::Vector2f camera_position) {
	rectangle_shape.setPosition(sf::Vector2f(x - camera_position.x, y - camera_position.y));
	render_window->draw(rectangle_shape);
}

void Creature::TakeHit(sf::Int64 damage, sf::Int64 hit_stun_dur) {
#ifdef _DEBUG
	if (entity_type == "PlayerCharacter") {
		damage = 0;
	}
#endif
	
	sf::Int16 adjusted_damage = (sf::Int16)damage;

	if (hit_points - adjusted_damage <= 0) {
		hit_points = 0;
	} else {
		hit_points = hit_points - adjusted_damage;
	}

	sf::Time sleep_time = sf::milliseconds(20);
	sf::sleep(sf::Time(sleep_time));
	hit_stun_duration = hit_stun_dur;
	hit_stun_start_time = current_time;
}