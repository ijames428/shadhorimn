using namespace std;
#include <iostream>
#include "Projectile.h"

Projectile::Projectile(sf::RenderWindow *window, sf::Vector2f position, sf::Vector2f dimensions, bool subject_to_gravity) : RigidBody::RigidBody(position, dimensions, subject_to_gravity) {
	entity_type = "Projectile";
	speed = 2.0f;
	current_time = 0;
	fired_position = sf::Vector2f(0.0f, 0.0f);
	fired_time = 0;
	length_of_life = 1000000;
	range = 10000;
	is_active = false;
	fired_velocity = sf::Vector2f(0.0f, 0.0f);
	collision_enabled = false;

	entities_excluded_from_collision.push_back("PlayerCharacter");

	render_window = window;

	sf::CircleShape shape(dimensions.x);
	shape.setFillColor(sf::Color::White);
	shape.setPosition(position);

	circle_shape = shape;
}

void Projectile::Draw(sf::Vector2f camera_position) {
	if (is_active) {
		circle_shape.setPosition(sf::Vector2f(x - camera_position.x, y - camera_position.y));
		render_window->draw(circle_shape);
	}
}

void Projectile::UpdateProjectile(sf::Int64 curr_time) {
	current_time = curr_time;

	if (is_active) {
		float knock_back_x = 0.1f;
		float knock_back_y = 0.05f;

		velocity = fired_velocity;

		std::vector<RigidBody*> hit_objects = GetCollidersRigidBodyIsCollidingWith();
		cout << hit_objects.size() << "\n";
		for (int i = 0; i < (int)hit_objects.size(); i++) {
			//if (hit_sounds.size() > 0) {
			//	hit_sounds[rand() % 3].play();
			//}

			if (hit_objects[i]->entity_type == "Drone" ||
				hit_objects[i]->entity_type == "Creature" ||
				hit_objects[i]->entity_type == "PlayerCharacter") {
				hit_objects[i]->velocity.x = knock_back_x;
				hit_objects[i]->velocity.y = -knock_back_y;
				((Creature*)(hit_objects[i]))->TakeHit(1, 500000);
			}
			is_active = false;
		}

		if (fired_time + length_of_life < current_time ||
			GetDistanceBetweenTwoPoints(sf::Vector2f(x, y), fired_position) > range) {
			is_active = false;
		}
	}
}

void Projectile::Fire(sf::Int64 curr_time, sf::Vector2f position, sf::Vector2f vel) {
	fired_time = current_time;
	x = position.x;
	y = position.y;
	fired_velocity = vel;
	is_active = true;
}