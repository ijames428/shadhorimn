using namespace std;
#include <iostream>
#include "Grunt.h"
#include "World.h"
#include "Singleton.h"
#include "AssetManager.h"
#include "Settings.h"
#define PI 3.14159265

Grunt::Grunt(sf::RenderWindow *window, sf::Vector2f position, sf::Vector2f dimensions, bool subject_to_gravity) : Creature::Creature(window, position, dimensions, subject_to_gravity) {
	entity_type = Singleton<World>::Get()->ENTITY_TYPE_GRUNT;
	hit_points = 3;
	jump_power = 1.0f;
	aggro_radius = 500.0f;
	attack_radius = 75.0f;
	is_aggroed = false;
	movement_speed = 1.0f;

	time_of_last_attack = 0;
	duration_of_attack = 1000;
	time_between_attacks = 1500;
	attack_animation_duration = 100;

	HitBox = new RigidBody(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(40.0f, 10.0f), false, false);
	HitBox->entity_type = Singleton<World>::Get()->ENTITY_TYPE_HIT_BOX;

	grunt_color = sf::Color::Magenta;

	idle_sprite_scale = 0.12f;
	//idle_texture.loadFromFile("Images/Kaltar_Idle.png");
	//idle_texture = Singleton<AssetManager>().Get()->GetTexture("Images/Kaltar_Idle.png");
	//idle_sprite = sf::Sprite(idle_texture);
	idle_sprite = sf::Sprite(*Singleton<AssetManager>().Get()->GetTexture("Images/Kaltar_Idle.png"));
	idle_sprite.setScale(idle_sprite_scale, idle_sprite_scale);
	idle_sprite.setColor(grunt_color);

	running_animation = new SpriteAnimation(render_window, "Images/Kaltar_Running.png", 582, 522, 91, 9, 11, 0.12f, grunt_color);

	//attack_texture.loadFromFile("Images/Kaltar_Attack.png");
	//attack_texture = Singleton<AssetManager>().Get()->GetTexture("Images/Kaltar_Attack.png");
	//attack_sprite = sf::Sprite(attack_texture);
	attack_sprite = sf::Sprite(*Singleton<AssetManager>().Get()->GetTexture("Images/Kaltar_Attack.png"));
	attack_sprite.setScale(idle_sprite_scale, idle_sprite_scale);
	attack_sprite.setColor(grunt_color);

	taking_damage_texture = *Singleton<AssetManager>().Get()->GetTexture("Images/kaltar_taking_damage.png");
	taking_damage_sprite = sf::Sprite(taking_damage_texture);
	taking_damage_sprite.setScale(idle_sprite_scale, idle_sprite_scale);
	taking_damage_sprite.setColor(grunt_color);

	dead_on_ground_texture = *Singleton<AssetManager>().Get()->GetTexture("Images/kaltar_dead_on_ground.png");
	dead_on_ground_sprite = sf::Sprite(dead_on_ground_texture);
	dead_on_ground_sprite.setScale(idle_sprite_scale, idle_sprite_scale);
	dead_on_ground_sprite.setColor(grunt_color);

	target = Singleton<World>::Get()->main_character;

	if (!sword_whiffing_buffer.loadFromFile("Sound/sword_whiffing.wav")) {
		throw exception("Sound file not found");
	} else {
		sword_whiffing_sound.setBuffer(sword_whiffing_buffer);
		sword_whiffing_sound.setVolume(Singleton<Settings>().Get()->effects_volume);
	}

	if (!sword_hitting_enemy_buffer.loadFromFile("Sound/sword_hitting.wav")) {
		throw exception("Sound file not found");
	} else {
		sword_hitting_enemy_sound.setBuffer(sword_hitting_enemy_buffer);
		sword_hitting_enemy_sound.setVolume(Singleton<Settings>().Get()->effects_volume);
	}
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
					sf::Vector2f knock_back = sf::Vector2f();
					knock_back.x = 3.0f;
					knock_back.y = 8.0f;

					if (facing_right) {
						HitBox->x = x + width;
					}
					else {
						knock_back.x *= -1.0f;
						HitBox->x = x - HitBox->width;
					}

					HitBox->y = y;
					HitBox->Update(0);
					std::vector<RigidBody*> hit_objects = HitBox->GetCollidersRigidBodyIsCollidingWith();

					bool hit_player = false;

					for (int i = 0; i < (int)hit_objects.size(); i++) {
						if (hit_objects[i]->entity_type == Singleton<World>::Get()->ENTITY_TYPE_PLAYER_CHARACTER) {
							if (!((Creature*)(hit_objects[i]))->IsInvincible()) {
								((Creature*)(hit_objects[i]))->TakeHit(1, 1000, knock_back, true, true);
								hit_player = true;
							}
						}
					}

					if (hit_player) {
						sword_hitting_enemy_sound.play();
					} else {
						sword_whiffing_sound.play();
					}

					time_of_last_attack = current_time;
				}
			} else if (!in_the_air) {
				velocity.x = 0.0f;
				velocity.y = 0.0f;
			}
		} else {
			rectangle_shape.setFillColor(sf::Color::Red);
		}
	}
}

void Grunt::Draw(sf::Vector2f camera_position) {
	if (facing_right) {
		idle_sprite.setScale(idle_sprite_scale, idle_sprite.getScale().y);
		attack_sprite.setScale(idle_sprite_scale, idle_sprite.getScale().y);
		taking_damage_sprite.setScale(idle_sprite_scale, idle_sprite.getScale().y);
		dead_on_ground_sprite.setScale(idle_sprite_scale, idle_sprite.getScale().y);
	} else {
		idle_sprite.setScale(-idle_sprite_scale, idle_sprite.getScale().y);
		attack_sprite.setScale(-idle_sprite_scale, idle_sprite.getScale().y);
		taking_damage_sprite.setScale(-idle_sprite_scale, idle_sprite.getScale().y);
		dead_on_ground_sprite.setScale(-idle_sprite_scale, idle_sprite.getScale().y);
	}

	if (facing_right != running_animation->IsFacingRight()) {
		running_animation->Flip();
	}

	if (hit_points <= 0) {
		if (facing_right) {
			taking_damage_sprite.setPosition(sf::Vector2f(x - camera_position.x, y - camera_position.y));
			dead_on_ground_sprite.setPosition(sf::Vector2f(x - camera_position.x, y - camera_position.y));
		}
		else {
			taking_damage_sprite.setPosition(sf::Vector2f(x + width - camera_position.x, y - camera_position.y));
			dead_on_ground_sprite.setPosition(sf::Vector2f(x + width - camera_position.x, y - camera_position.y));
		}

		if (in_the_air) {
			render_window->draw(taking_damage_sprite);
		}
		else {
			render_window->draw(dead_on_ground_sprite);
		}
	} else if (hit_stun_start_time + hit_stun_duration > current_time) {
		if (facing_right) {
			taking_damage_sprite.setPosition(sf::Vector2f(x - camera_position.x, y - camera_position.y));
		}
		else {
			taking_damage_sprite.setPosition(sf::Vector2f(x + width - camera_position.x, y - camera_position.y));
		}
		render_window->draw(taking_damage_sprite);
	} else if (time_of_last_attack + attack_animation_duration > current_time) {
		if (facing_right) {
			attack_sprite.setPosition(sf::Vector2f(x - camera_position.x, y - camera_position.y));
		} else {
			attack_sprite.setPosition(sf::Vector2f(x + width - camera_position.x, y - camera_position.y));
		}
		render_window->draw(attack_sprite);
	} else if (velocity.x == 0) {
		if (facing_right) {
			idle_sprite.setPosition(sf::Vector2f(x - camera_position.x, y - camera_position.y));
		} else {
			idle_sprite.setPosition(sf::Vector2f(x + width - camera_position.x, y - camera_position.y));
		}
		render_window->draw(idle_sprite);
	} else {
		running_animation->Draw(camera_position, sf::Vector2f(x + width / 2.0f, y + height / 2.0f));
	}

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