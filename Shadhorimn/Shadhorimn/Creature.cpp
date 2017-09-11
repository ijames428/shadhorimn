using namespace std;
#include <iostream>
#include "Creature.h"
#include "Singleton.h"
#include "World.h"

Creature::Creature(sf::RenderWindow *window, sf::Vector2f position, sf::Vector2f dimensions, bool subject_to_gravity) : RigidBody::RigidBody(position, dimensions, subject_to_gravity) {
	entity_type = Singleton<World>::Get()->ENTITY_TYPE_CREATURE;
	hit_points = 1;
	facing_right_when_hit = false;
	lock_facing_direction_when_hit = false;
	post_hit_invincibility_start_time = 0;
	post_hit_invincibility_duration = 2000;

	roll_start_time = 0;
	roll_duration = 300;
	roll_invincibility_start_time = 0;
	roll_invincibility_duration = 150;
	roll_height = dimensions.y / 2.0f;
	usual_height = dimensions.y;
	roll_velocity_x = 6.0f;

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

bool Creature::IsRolling() {
	return roll_start_time + roll_duration > current_time;
}

bool Creature::IsInvincible() {
	return IsInRollInvincibility() || IsInPostHitInvincibility();
}

bool Creature::IsInRollInvincibility() {
	return roll_invincibility_start_time + roll_invincibility_duration > current_time;
}

bool Creature::IsInPostHitInvincibility() {
	return post_hit_invincibility_start_time + post_hit_invincibility_duration > current_time;
}

void Creature::TakeHit(sf::Int64 damage, sf::Int64 hit_stun_dur, sf::Vector2f knock_back, bool lock_facing_direction) {
#ifdef _DEBUG
	if (entity_type == Singleton<World>::Get()->ENTITY_TYPE_PLAYER_CHARACTER) {
		//damage = 0;
	}
#endif

	if (hit_points <= 0) {
		return;
	}

	if (entity_type == Singleton<World>::Get()->ENTITY_TYPE_PLAYER_CHARACTER) {
		if (IsInPostHitInvincibility()) {
			return;
		}

		post_hit_invincibility_start_time = current_time;
	}

	if (lock_facing_direction) {
		facing_right_when_hit = facing_right;
		lock_facing_direction_when_hit = true;
	}

	velocity.x = knock_back.x;
	velocity.y = -knock_back.y;
	
	sf::Int16 adjusted_damage = (sf::Int16)damage;

	if (hit_points - adjusted_damage <= 0) {
		OnDeath();
	} else {
		hit_points = hit_points - adjusted_damage;
	}

	sf::Time sleep_time = sf::milliseconds(20);
	sf::sleep(sf::Time(sleep_time));
	hit_stun_duration = hit_stun_dur;
	hit_stun_start_time = current_time;
}

void Creature::OnDeath() {
	hit_points = 0;

	if (entity_type != Singleton<World>::Get()->ENTITY_TYPE_PLAYER_CHARACTER) {
		only_collide_with_platforms = true;
		entities_excluded_from_collision.push_back(Singleton<World>::Get()->ENTITY_TYPE_PLAYER_CHARACTER);
		entities_excluded_from_collision.push_back(Singleton<World>::Get()->ENTITY_TYPE_RIGID_BODY);
		entities_excluded_from_collision.push_back(Singleton<World>::Get()->ENTITY_TYPE_DRONE);
		entities_excluded_from_collision.push_back(Singleton<World>::Get()->ENTITY_TYPE_GRUNT);
		entities_excluded_from_collision.push_back(Singleton<World>::Get()->ENTITY_TYPE_GUNNER);
		entities_excluded_from_collision.push_back(Singleton<World>::Get()->ENTITY_TYPE_CHARGER);
		entities_excluded_from_collision.push_back(Singleton<World>::Get()->ENTITY_TYPE_PROJECTILE);
		entities_excluded_from_collision.push_back(Singleton<World>::Get()->ENTITY_TYPE_HIT_BOX);
	}
}