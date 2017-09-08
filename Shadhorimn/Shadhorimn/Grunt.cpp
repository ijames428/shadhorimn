using namespace std;
#include <iostream>
#include "Grunt.h"
#include "World.h"
#include "Singleton.h"
#define PI 3.14159265

Grunt::Grunt(sf::RenderWindow *window, sf::Vector2f position, sf::Vector2f dimensions, bool subject_to_gravity) : Creature::Creature(window, position, dimensions, subject_to_gravity) {
	entity_type = Singleton<World>::Get()->ENTITY_TYPE_GRUNT;
	hit_points = 3;
	jump_power = 1.0f;
	aggro_radius = 200.0f;
	attack_radius = 75.0f;
	is_aggroed = false;
	movement_speed = 1.0f;

	time_of_last_attack = 0;
	duration_of_attack = 1000;
	time_between_attacks = 1500;

	HitBox = new RigidBody(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(40.0f, 10.0f), false, false);
	HitBox->entity_type = Singleton<World>::Get()->ENTITY_TYPE_HIT_BOX;

	sf::RectangleShape shape(dimensions);
	shape.setFillColor(sf::Color::Magenta);
	shape.setPosition(position);

	rectangle_shape = shape;

	target = Singleton<World>::Get()->main_character;
}

void Grunt::UpdateBehavior(sf::Int64 curr_time) {
	current_time = curr_time;

	if (hit_points > 0) {
		if (hit_stun_start_time + hit_stun_duration <= curr_time) {
			rectangle_shape.setFillColor(sf::Color::Magenta);

			is_aggroed = RigidBody::GetDistanceBetweenTwoPoints(sf::Vector2f(target->x + target->width / 2.0f, target->y + target->height / 2.0f), sf::Vector2f(x, y)) < aggro_radius;

			if (is_aggroed) {
				if (time_of_last_attack + duration_of_attack <= curr_time) {
					if (target->x > x) {
						velocity.x = movement_speed;
					}
					else if (target->x < x) {
						velocity.x = -movement_speed;
					}
				}

				if (time_of_last_attack + time_between_attacks <= curr_time && 
					RigidBody::GetDistanceBetweenTwoPoints(sf::Vector2f(target->x + target->width / 2.0f, target->y + target->height / 2.0f), sf::Vector2f(x + width / 2.0f, y + height / 2.0f)) < attack_radius) {
					float knock_back_x = 2.0f;
					float knock_back_y = 6.0f;

					if (facing_right) {
						HitBox->x = x + width;
					}
					else {
						knock_back_x *= -1.0f;
						HitBox->x = x - HitBox->width;
					}

					HitBox->y = y;
					HitBox->Update(0);
					std::vector<RigidBody*> hit_objects = HitBox->GetCollidersRigidBodyIsCollidingWith();

					for (int i = 0; i < (int)hit_objects.size(); i++) {
						if (hit_objects[i]->entity_type == Singleton<World>::Get()->ENTITY_TYPE_PLAYER_CHARACTER) {
							hit_objects[i]->velocity.x = knock_back_x;
							hit_objects[i]->velocity.y = -knock_back_y;
							((Creature*)(hit_objects[i]))->TakeHit(1, 1000);
						}
					}

					time_of_last_attack = current_time;
				}
			} else {
				velocity.x = 0.0f;
				velocity.y = 0.0f;
			}
		} else {
			rectangle_shape.setFillColor(sf::Color::Red);
		}
	}
}

void Grunt::Draw(sf::Vector2f camera_position) {
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
	aggro_circle.setPosition(sf::Vector2f(x + (width / 2) - (aggro_radius)-camera_position.x, y + (height / 2) - (aggro_radius)-camera_position.y));
	render_window->draw(aggro_circle);
#endif
}