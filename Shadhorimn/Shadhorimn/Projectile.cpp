using namespace std;
#include <iostream>
#include "Projectile.h"
#include "Singleton.h"
#include "Settings.h"
#include "World.h"

Projectile::Projectile(sf::RenderWindow *window, sf::Vector2f position, sf::Vector2f dimensions, bool subject_to_gravity) : RigidBody::RigidBody(position, dimensions, subject_to_gravity) {
	entity_type = Singleton<World>::Get()->ENTITY_TYPE_PROJECTILE;
	speed = 2.0f;
	current_time = 0;
	fired_position = sf::Vector2f(0.0f, 0.0f);
	fired_time = 0;
	range = 300;
	is_active = false;
	fired_velocity = sf::Vector2f(0.0f, 0.0f);
	collision_enabled = false;
	time_of_impact = 0;
	duration_of_impact_animation = 50;

	ExcludeFromCollision(Singleton<World>::Get()->ENTITY_TYPE_PROJECTILE);
	ExcludeFromCollision(Singleton<World>::Get()->ENTITY_TYPE_RIGID_BODY);
	ExcludeFromCollision(Singleton<World>::Get()->ENTITY_TYPE_HIT_BOX);
	ExcludeFromCollision(Singleton<World>::Get()->ENTITY_TYPE_BOSS_TRIGGER);

	render_window = window;

	sf::CircleShape shape(dimensions.x / 2.0f);
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
		sf::Vector2f knock_back = sf::Vector2f();
		knock_back.x = velocity.x / 2.0f;
		knock_back.y = -velocity.y / 2.0f;

		for (int i = 0; i < (int)hit_objects.size(); i++) {
			hit_sound.play();
			
			bool deactivate = true;

			if (hit_objects[i]->entity_type == Singleton<World>::Get()->ENTITY_TYPE_DRONE ||
				hit_objects[i]->entity_type == Singleton<World>::Get()->ENTITY_TYPE_GRUNT ||
				hit_objects[i]->entity_type == Singleton<World>::Get()->ENTITY_TYPE_GUNNER ||
				hit_objects[i]->entity_type == Singleton<World>::Get()->ENTITY_TYPE_CHARGER) {
				((Creature*)(hit_objects[i]))->TakeHit(1, 500, knock_back);
			}

			if (hit_objects[i]->entity_type == Singleton<World>::Get()->ENTITY_TYPE_PLAYER_CHARACTER) {
				if (((Creature*)(hit_objects[i]))->IsInvincible()) {
					deactivate = false;
				} else {
					((Creature*)(hit_objects[i]))->TakeHit(1, 500, knock_back);
				}
			}

			if (hit_objects[i]->entity_type == Singleton<World>::Get()->ENTITY_TYPE_STALAGTITE) {
				Singleton<World>::Get()->HitStalagtite();
			}

			if (deactivate) {
				is_active = false;
				time_of_impact = current_time;
				impact_position.x = x;
				impact_position.y = y;
				velocity.x = 0.0f;
				velocity.y = 0.0f;
			}
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

	entities_excluded_from_collision.erase(entities_excluded_from_collision.begin(), entities_excluded_from_collision.end());

	ExcludeFromCollision(Singleton<World>::Get()->ENTITY_TYPE_PROJECTILE);
	ExcludeFromCollision(Singleton<World>::Get()->ENTITY_TYPE_RIGID_BODY);
	ExcludeFromCollision(Singleton<World>::Get()->ENTITY_TYPE_HIT_BOX);
	ExcludeFromCollision(Singleton<World>::Get()->ENTITY_TYPE_BOSS_TRIGGER);
}

void Projectile::ExcludeFromCollision(int ent_typ) {
	entities_excluded_from_collision.push_back(ent_typ);
}