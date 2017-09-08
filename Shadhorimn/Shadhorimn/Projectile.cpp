using namespace std;
#include <iostream>
#include "Projectile.h"
#include "Singleton.h"
#include "Settings.h"

Projectile::Projectile(sf::RenderWindow *window, sf::Vector2f position, sf::Vector2f dimensions, bool subject_to_gravity) : RigidBody::RigidBody(position, dimensions, subject_to_gravity) {
	entity_type = "Projectile";
	speed = 2.0f;
	current_time = 0;
	fired_position = sf::Vector2f(0.0f, 0.0f);
	fired_time = 0;
	range = 1000;
	is_active = false;
	fired_velocity = sf::Vector2f(0.0f, 0.0f);
	collision_enabled = false;
	time_of_impact = 0;
	duration_of_impact_animation = 50;

	ExcludeFromCollision("Projectile");
	ExcludeFromCollision("Rigidbody");

	render_window = window;

	sf::CircleShape shape(dimensions.x);
	shape.setFillColor(sf::Color::White);
	shape.setPosition(position);

	circle_shape = shape;

	if (!buffer0.loadFromFile("Sound/Hit0.wav")) {
		throw exception("Sound file not found");
	}
	else {
		hit_sound.setBuffer(buffer0);
		hit_sound.setVolume(20 * (Singleton<Settings>().Get()->effects_volume / 100.0f));
	}

	impact_texture.loadFromFile("Images/BulletImpact.png");
	impact_sprite = sf::Sprite(impact_texture);

	impact_position = sf::Vector2f(0.0f, 0.0f);
}

void Projectile::Draw(sf::Vector2f camera_position, sf::Int64 curr_time) {
	if (is_active) {
		circle_shape.setPosition(sf::Vector2f(x - camera_position.x, y - camera_position.y));
		render_window->draw(circle_shape);
	}

	if (time_of_impact + duration_of_impact_animation > curr_time) {
		impact_sprite.setPosition(sf::Vector2f((impact_position.x + width / 2.0f) - (impact_texture.getSize().x / 2.0f) - camera_position.x, 
												(impact_position.y + height / 2.0f) - (impact_texture.getSize().y / 2.0f) - camera_position.y));
		render_window->draw(impact_sprite);
	}
}

void Projectile::UpdateProjectile(sf::Int64 curr_time) {
	current_time = curr_time;

	if (is_active) {
		velocity = fired_velocity;

		std::vector<RigidBody*> hit_objects = GetCollidersRigidBodyIsCollidingWith();

		for (int i = 0; i < (int)hit_objects.size(); i++) {
			hit_sound.play();

			if (hit_objects[i]->entity_type == "Drone" ||
				hit_objects[i]->entity_type == "Grunt" ||
				hit_objects[i]->entity_type == "PlayerCharacter" ||
				hit_objects[i]->entity_type == "Gunner" ||
				hit_objects[i]->entity_type == "Charger") {
				hit_objects[i]->velocity.x = velocity.x;
				hit_objects[i]->velocity.y = -velocity.y;
				((Creature*)(hit_objects[i]))->TakeHit(1, 500);
			}

			is_active = false;
			time_of_impact = current_time;
			impact_position.x = x;
			impact_position.y = y;
			velocity.x = 0.0f;
			velocity.y = 0.0f;
		}

		if (GetDistanceBetweenTwoPoints(sf::Vector2f(x, y), fired_position) > range) {
			is_active = false;
		}
	}
}

void Projectile::Fire(sf::Int64 curr_time, sf::Vector2f position, sf::Vector2f vel) {
	fired_position = position;
	fired_time = current_time;
	x = position.x;
	y = position.y;
	fired_velocity = vel;
	is_active = true;
}

void Projectile::ExcludeFromCollision(std::string entity_type) {
	entities_excluded_from_collision.push_back(entity_type);
}