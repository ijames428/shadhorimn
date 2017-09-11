#ifndef CREATURE_H
#define CREATURE_H

using namespace std;
#include <SFML/Graphics.hpp>
#include "RigidBody.h"

class Creature : public RigidBody {
	protected:
		sf::Int64 current_time;
		sf::Int64 hit_stun_start_time;
		sf::Int64 hit_stun_duration;
		float speed;
		float jump_power;
		sf::Int64 post_hit_invincibility_start_time;
		sf::Int64 post_hit_invincibility_duration;
		sf::Int64 roll_start_time;
		sf::Int64 roll_duration;
		sf::Int64 roll_invincibility_start_time;
		sf::Int64 roll_invincibility_duration;
		float roll_height;
		float usual_height;
		float roll_velocity_x;
	public:
		sf::Int16 hit_points;
		Creature(sf::RenderWindow *window, sf::Vector2f position = sf::Vector2f(0.0f, 0.0f), sf::Vector2f dimensions = sf::Vector2f(0.0f, 0.0f), bool subject_to_gravity = true);
		sf::RectangleShape rectangle_shape;
		sf::RenderWindow *render_window;
		void Draw(sf::Vector2f camera_position);
		void TakeHit(sf::Int64 damage, sf::Int64 hit_stun_duration, sf::Vector2f knock_back, bool activate_invincibility = true, bool lock_facing_direction = false);
		void OnDeath();
		bool IsInPostHitInvincibility();
		bool IsInRollInvincibility();
		bool IsInvincible();
		bool IsRolling();
};

#endif