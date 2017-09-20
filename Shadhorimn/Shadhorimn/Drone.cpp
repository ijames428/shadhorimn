using namespace std;
#include <iostream>
#include "Drone.h"
#include "World.h"
#include "Singleton.h"
#include "Settings.h"
#define PI 3.14159265

Drone::Drone(sf::RenderWindow *window, sf::Vector2f position, sf::Vector2f dimensions, bool subject_to_gravity) : Creature::Creature(window, position, dimensions, subject_to_gravity) {
	entity_type = Singleton<World>::Get()->ENTITY_TYPE_DRONE;
	hit_points = 2;

	jump_power = 1.0f;
	aggro_radius = 500.0f;
	is_aggroed = false;

	movement_speed = 1.0f;

	sf::RectangleShape shape(dimensions);
	shape.setFillColor(sf::Color::Blue);
	shape.setPosition(position);

	rectangle_shape = shape;

	target = Singleton<World>::Get()->main_character;

	time_between_firing = 750;
	time_of_last_firing = 0;
	for (int i = 0; i < 10; i++) {
		projectiles.push_back(new Projectile(window, position, sf::Vector2f(20.0f, 20.0f), false));
		projectiles[i]->ExcludeFromCollision(Singleton<World>::Get()->ENTITY_TYPE_DRONE);
	}

	if (!firing_projectile_buffer.loadFromFile("Sound/drone_firing.wav")) {
		throw exception("Sound file not found");
	} else {
		firing_projectile_sound.setBuffer(firing_projectile_buffer);
		firing_projectile_sound.setVolume(Singleton<Settings>().Get()->effects_volume);
	}
}

void Drone::UpdateBehavior(sf::Int64 curr_time) {
	current_time = curr_time;

	if (hit_points <= 0) {
		gravity_enabled = true;
	} else {
		if (hit_stun_start_time + hit_stun_duration > curr_time) {
			gravity_enabled = true;
		} else {
			gravity_enabled = false;

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

				if (time_of_last_firing + time_between_firing < current_time) {
					for (int i = 0; i < (int)(projectiles.size()); i++) {
						if (!projectiles[i]->is_active) {
							float deltaX = target->x - x;
							float deltaY = target->y - y;
							sf::Vector2f vel = sf::Vector2f(deltaX, deltaY);
							sf::Vector2f starting_position = sf::Vector2f(x + width / 2.0f - projectiles[i]->width / 2.0f, y + height / 2.0f - projectiles[i]->height / 2.0f);
							float length = sqrt(deltaX * deltaX + deltaY * deltaY);

							if (length != 0) {
								vel.x = vel.x / length;
								vel.y = vel.y / length;
							}

							projectiles[i]->Fire(current_time, starting_position, vel * 4.0f);
							projectiles[i]->ExcludeFromCollision(Singleton<World>::Get()->ENTITY_TYPE_DRONE);

							time_of_last_firing = current_time;
							firing_projectile_sound.play();

							break;
						}
					}
				}
			} else {
				velocity.x = 0.0f;
				velocity.y = 0.0f;
			}
		}
	}
}

void Drone::UpdateProjectiles(sf::Int64 curr_time, sf::Int64 frame_delta) {
	for (int i = 0; i < (int)(projectiles.size()); i++) {
		if (projectiles[i]->is_active) {
			projectiles[i]->Update(frame_delta);
			projectiles[i]->UpdateProjectile(curr_time);
		}
	}
}

void Drone::DrawProjectiles(sf::Vector2f camera_position, sf::Int64 curr_time) {
	for (int i = 0; i < (int)(projectiles.size()); i++) {
		projectiles[i]->Draw(camera_position, curr_time);
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