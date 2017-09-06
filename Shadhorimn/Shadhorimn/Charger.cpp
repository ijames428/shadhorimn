using namespace std;
#include <iostream>
#include "Charger.h"
#include "World.h"
#include "Singleton.h"
#define PI 3.14159265

Charger::Charger(sf::RenderWindow *window, sf::Vector2f position, sf::Vector2f dimensions, bool subject_to_gravity) : Creature::Creature(window, position, dimensions, subject_to_gravity) {
	entity_type = "Charger";
	starting_hit_points = 30;
	hit_points = starting_hit_points;

	jump_power = 1.0f;
	aggro_radius = 200.0f;
	is_charging = false;
	attack_radius = 75.0f;

	time_of_last_attack = 0;
	duration_of_attack = 1000;
	time_between_attacks = 1500;

	HitBox = new RigidBody(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(40.0f, 10.0f), false, false);
	HitBox->entity_type = "HitBox";

	WallDetector = new RigidBody(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(5.0f, 10.0f), false, false);
	WallDetector->entity_type = "WallDetector";// charger endofthegame hitbox
	WallDetector->entities_excluded_from_collision.push_back(entity_type);
	WallDetector->entities_excluded_from_collision.push_back(HitBox->entity_type);
	WallDetector->entities_excluded_from_collision.push_back("Projectile");
	WallDetector->entities_excluded_from_collision.push_back("EndOfTheGame");

	movement_speed = 1.0f;
	charge_speed = 8.0f;
	charge_speed_second_stage = 12.0f;

	charge_velocity = sf::Vector2f(0.0f, 0.0f);

	sf::RectangleShape shape(dimensions);
	shape.setFillColor(sf::Color::White);
	shape.setPosition(position);

	rectangle_shape = shape;

	target = Singleton<World>::Get()->main_character;

	//time_between_firing = 250;
	//time_of_last_firing = 0;
	for (int i = 0; i < 5; i++) {
		projectiles.push_back(new Projectile(window, position, sf::Vector2f(10.0f, 10.0f), false));
		projectiles[i]->ExcludeFromCollision(entity_type);
		projectiles[i]->ExcludeFromCollision(HitBox->entity_type);
		projectiles[i]->ExcludeFromCollision("Projectile");
		projectiles[i]->ExcludeFromCollision("EndOfTheGame");
	}
}

void Charger::UpdateBehavior(sf::Int64 curr_time) {
	current_time = curr_time;

	if (hit_points > 0) {
		if (hit_stun_start_time + hit_stun_duration <= current_time) {
			float knock_back_x = 2.0f;
			float knock_back_y = 6.0f;

			if (facing_right) {
				HitBox->x = x + width;
				WallDetector->x = x + width;
			} else {
				knock_back_x *= -1.0f;
				HitBox->x = x - HitBox->width;
				WallDetector->x = x - WallDetector->width;
			}

			HitBox->y = y;
			HitBox->Update(0);
			WallDetector->y = y;
			WallDetector->Update(0);

			if (is_charging) {
				velocity = charge_velocity;

				knock_back_x = 4.0f * (knock_back_x < 0.0f ? -1.0f : 1.0f);
				knock_back_y = 10.0f;

				std::vector<RigidBody*> hit_objects = WallDetector->GetCollidersRigidBodyIsCollidingWith();

				for (int i = 0; i < (int)hit_objects.size(); i++) {
					if (hit_objects[i]->entity_type == "Platform") {
						velocity.x = -knock_back_x;
						velocity.y = -knock_back_y;
						TakeHit(0, 3000);
						if (IsInSecondStage()) {
							FireSecondStageProjectiles();
						}
					} else if (hit_objects[i]->entity_type == "PlayerCharacter") {
						hit_objects[i]->velocity.x = knock_back_x;
						hit_objects[i]->velocity.y = -knock_back_y;
						velocity.x = -knock_back_x / 3.0f;
						velocity.y = -knock_back_y / 3.0f;
						((Creature*)(hit_objects[i]))->TakeHit(3, 1000);
						TakeHit(0, 1500);
						if (IsInSecondStage()) {
							FireSecondStageProjectiles();
						}
					}

					is_charging = false;
				}
			} else {
				StartCharge();
				//if (target->x > x) {
				//	velocity.x = movement_speed;
				//} else if (target->x < x) {
				//	velocity.x = -movement_speed;
				//}
				//
				//if (time_of_last_attack + time_between_attacks <= curr_time &&
				//	RigidBody::GetDistanceBetweenTwoPoints(sf::Vector2f(target->x + target->width / 2.0f, target->y + target->height / 2.0f), sf::Vector2f(x + width / 2.0f, y + height / 2.0f)) < attack_radius) {
				//
				//	std::vector<RigidBody*> hit_objects = WallDetector->GetCollidersRigidBodyIsCollidingWith();
				//
				//	for (int i = 0; i < (int)hit_objects.size(); i++) {
				//		if (hit_objects[i]->entity_type == "PlayerCharacter") {
				//			hit_objects[i]->velocity.x = knock_back_x;
				//			hit_objects[i]->velocity.y = -knock_back_y;
				//			((Creature*)(hit_objects[i]))->TakeHit(3, 1000);
				//		}
				//	}
				//}
			}
		}
	}
}

void Charger::FireSecondStageProjectiles() {
	sf::Vector2f starting_position = sf::Vector2f(x + width / 2.0f, y + height / 2.0f);
	float projectile_speed = 6.0f;
	float angled_speed = 4.24f;

	projectiles[0]->Fire(current_time, starting_position, sf::Vector2f(-projectile_speed, 0.0f));
	projectiles[1]->Fire(current_time, starting_position, sf::Vector2f(-angled_speed, -angled_speed));
	projectiles[2]->Fire(current_time, starting_position, sf::Vector2f(0.0f, -projectile_speed));
	projectiles[3]->Fire(current_time, starting_position, sf::Vector2f(angled_speed, -angled_speed));
	projectiles[4]->Fire(current_time, starting_position, sf::Vector2f(projectile_speed, 0.0f));
}

bool Charger::IsInSecondStage() {
	return hit_points <= starting_hit_points / 3;
}

void Charger::StartCharge() {
	is_charging = true;
	charge_velocity.y = 0.0f;
	float charge_speed_by_stage = charge_speed;

	if (IsInSecondStage()) {
		charge_speed_by_stage = charge_speed_second_stage;
	}

	if (target->x > x) {
		charge_velocity.x = charge_speed_by_stage;
	}
	else if (target->x < x) {
		charge_velocity.x = -charge_speed_by_stage;
	}
}

void Charger::UpdateProjectiles(sf::Int64 curr_time, sf::Int64 frame_delta) {
	for (int i = 0; i < (int)(projectiles.size()); i++) {
		if (projectiles[i]->is_active) {
			projectiles[i]->Update(frame_delta);
			projectiles[i]->UpdateProjectile(curr_time);
		}
	}
}

void Charger::DrawProjectiles(sf::Vector2f camera_position, sf::Int64 curr_time) {
	for (int i = 0; i < (int)(projectiles.size()); i++) {
		projectiles[i]->Draw(camera_position, curr_time);
	}
}

void Charger::Draw(sf::Vector2f camera_position) {
	rectangle_shape.setPosition(sf::Vector2f(x - camera_position.x, y - camera_position.y));
	if (IsInSecondStage()) {
		rectangle_shape.setFillColor(sf::Color::Red);
	}
	render_window->draw(rectangle_shape);
}