using namespace std;
#include <iostream>
#include "Platform.h"
#include "Singleton.h"
#include "World.h"

Platform::Platform(sf::RenderWindow *window, sf::Vector2f position, sf::Vector2f dimensions, bool subject_to_gravity) : RigidBody::RigidBody(position, dimensions, subject_to_gravity) {
	entity_type = Singleton<World>::Get()->ENTITY_TYPE_PLATFORM;
	render_window = window;

	sf::RectangleShape shape(dimensions);
	shape.setFillColor(sf::Color::Green);
	shape.setPosition(position);

	entities_excluded_from_collision.push_back(entity_type);

	rectangle_shape = shape;
}

void Platform::Draw(sf::Vector2f camera_position) {
	rectangle_shape.setPosition(sf::Vector2f(x - camera_position.x, y - camera_position.y));
	render_window->draw(rectangle_shape);
}