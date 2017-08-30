using namespace std;
#include <iostream>
#include "Checkpoint.h"
#include "Singleton.h"
#include "World.h"  

Checkpoint::Checkpoint(sf::RenderWindow *window, sf::Vector2f position, sf::Vector2f dimensions, bool subject_to_gravity) : RigidBody::RigidBody(position, dimensions, subject_to_gravity) {
	entity_type = "Checkpoint";
	collision_enabled = false;
	render_window = window;

	sf::RectangleShape shape(dimensions);
	shape.setFillColor(sf::Color::Yellow);
	shape.setFillColor(sf::Color(shape.getFillColor().r, shape.getFillColor().g, shape.getFillColor().b, 127));
	shape.setPosition(position);

	rectangle_shape = shape;
}

void Checkpoint::Draw(sf::Vector2f camera_position) {
	rectangle_shape.setPosition(sf::Vector2f(x - camera_position.x, y - camera_position.y));
	render_window->draw(rectangle_shape);
}

void Checkpoint::UpdateCheckPoint() {
	std::vector<RigidBody*> hit_objects = GetCollidersRigidBodyIsCollidingWith();

	for (int i = 0; i < (int)hit_objects.size(); i++) {
		if (hit_objects[i]->entity_type == "PlayerCharacter") {
			Singleton<World>::Get()->SetCurrentCheckPoint(this);
			break;
		}
	}
}