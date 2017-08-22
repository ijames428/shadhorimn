using namespace std;
#include <iostream>
#include "Creature.h"

Creature::Creature(sf::RenderWindow *window, sf::Vector2f position, sf::Vector2f dimensions, bool subject_to_gravity) : RigidBody::RigidBody(position, dimensions, subject_to_gravity) {
	speed = 2.0f;
	jump_power = 1.0f;

	render_window = window;

	sf::RectangleShape shape(dimensions);
	shape.setFillColor(sf::Color::Cyan);
	shape.setPosition(position);

	rectangle_shape = shape;
}

void Creature::Draw() {
	rectangle_shape.setPosition(sf::Vector2f(x, y));
	render_window->draw(rectangle_shape);
}