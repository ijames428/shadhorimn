using namespace std;
#include <iostream>
#include "Drone.h"
#include "World.h"
#include "Singleton.h"
#define PI 3.14159265

Drone::Drone(sf::RenderWindow *window, sf::Vector2f position, sf::Vector2f dimensions, bool subject_to_gravity) : Creature::Creature(window, position, dimensions, subject_to_gravity) {
	speed = 2.0f;
	jump_power = 1.0f;
	aggro_radius = 200.0f;
	is_aggroed = false;

	movement_speed = 0.05f;

	sf::RectangleShape shape(dimensions);
	shape.setFillColor(sf::Color::Blue);
	shape.setPosition(position);

	rectangle_shape = shape;

	target = Singleton<World>::Get()->main_character;
}

void Drone::UpdateBehavior() {
	is_aggroed = RigidBody::GetDistanceBetweenTwoPoints(sf::Vector2f(target->x + target->width / 2.0f, target->y + target->height / 2.0f), sf::Vector2f(x, y)) < aggro_radius;

	if (is_aggroed) {
		if (target->x > x) {
			velocity.x = movement_speed;
		}
		else if (target->x < x) {
			velocity.x = -movement_speed;
		}

		if (target->y > y) {
			velocity.y = movement_speed;
		}
		else if (target->y < y) {
			velocity.y = -movement_speed;
		}
	} else {
		velocity.x = 0.0f;
		velocity.y = 0.0f;
	}
}

void Drone::Draw(sf::Vector2f camera_position) {
	rectangle_shape.setPosition(sf::Vector2f(x - camera_position.x, y - camera_position.y));
	render_window->draw(rectangle_shape);

#ifdef _DEBUG
	sf::Color base_color(sf::Color::Blue);
	if (is_aggroed) {
		base_color = sf::Color::Red;
	}
	sf::Color circle_color(base_color.r, base_color.g, base_color.b, 65);
	sf::CircleShape aggro_circle(aggro_radius);
	aggro_circle.setFillColor(circle_color);
	aggro_circle.setPosition(sf::Vector2f(x + (width / 2) - (aggro_radius) - camera_position.x, y + (height / 2) - (aggro_radius) - camera_position.y));
	render_window->draw(aggro_circle);
#endif
}