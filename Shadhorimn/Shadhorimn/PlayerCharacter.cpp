using namespace std;
#include <iostream>
#include "PlayerCharacter.h"
#include "AssetManager.h"
#include "Singleton.h"
#include "Settings.h"
#include "World.h"

PlayerCharacter::PlayerCharacter(sf::RenderWindow *window, sf::Vector2f position, sf::Vector2f dimensions, bool subject_to_gravity) : 
	Creature::Creature(window, position, dimensions, subject_to_gravity) {
	entity_type = Singleton<World>::Get()->ENTITY_TYPE_PLAYER_CHARACTER;
	hit_points = max_hit_points = 10;
	can_take_input = true;
	time_of_last_attack = 0;
	attack_cooldown = 600;
	attack_duration = 100;
	time_of_last_fire = 0;
	fire_cooldown = 1000;
	fire_duration = 200;
	was_in_air = false;
	hit_terminal_velocity = false;

	HitBox = new RigidBody(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(60.0f, 10.0f), false, false);
	HitBox->entities_excluded_from_collision.push_back(entity_type);
	HitBox->entity_type = Singleton<World>::Get()->ENTITY_TYPE_HIT_BOX;

	for (int i = 0; i < 10; i++) {
		projectiles.push_back(new Projectile(window, position, sf::Vector2f(20.0f, 20.0f), false));
		projectiles[i]->ExcludeFromCollision(entity_type);
		projectiles[i]->ExcludeFromCollision(HitBox->entity_type);
	}

	entities_excluded_from_collision.push_back(projectiles[0]->entity_type);

	speed = 5.0f;
	jump_power = 12.0f;

	sf::RectangleShape shape(dimensions);
	shape.setFillColor(sf::Color::Yellow);
	shape.setPosition(position);

	rectangle_shape = shape;

	player_color = sf::Color::Cyan;

	idle_sprite_scale = 0.12f;

	idle_texture = *Singleton<AssetManager>().Get()->GetTexture("Images/Kaltar_Idle.png");
	idle_sprite = sf::Sprite(idle_texture);
	//idle_sprite = sf::Sprite(Singleton<AssetManager>().Get()->GetTexture("Images/Kaltar_Idle.png"));
	idle_sprite.setScale(idle_sprite_scale, idle_sprite_scale);
	idle_sprite.setColor(player_color);

	running_animation = new SpriteAnimation(render_window, "Images/Kaltar_Running.png", 582, 522, 91, 9, 11, 0.12f, player_color);

	attack_texture = *Singleton<AssetManager>().Get()->GetTexture("Images/Kaltar_Attack.png");
	attack_sprite = sf::Sprite(attack_texture);
	//attack_sprite = sf::Sprite(Singleton<AssetManager>().Get()->GetTexture("Images/Kaltar_Attack.png"));
	attack_sprite.setScale(idle_sprite_scale, idle_sprite_scale);
	attack_sprite.setColor(player_color);

	fire_texture = *Singleton<AssetManager>().Get()->GetTexture("Images/Kaltar_Fire.png");
	fire_sprite = sf::Sprite(fire_texture);
	//fire_sprite = sf::Sprite(Singleton<AssetManager>().Get()->GetTexture("Images/Kaltar_Fire.png"));
	fire_sprite.setScale(idle_sprite_scale, idle_sprite_scale);
	fire_sprite.setColor(player_color);

	dodge_texture = *Singleton<AssetManager>().Get()->GetTexture("Images/Kaltar_Dodge.png");
	dodge_sprite = sf::Sprite(dodge_texture);
	dodge_sprite.setScale(idle_sprite_scale, idle_sprite_scale);
	dodge_sprite.setColor(player_color);

	taking_damage_texture = *Singleton<AssetManager>().Get()->GetTexture("Images/kaltar_taking_damage.png");
	taking_damage_sprite = sf::Sprite(taking_damage_texture);
	taking_damage_sprite.setScale(idle_sprite_scale, idle_sprite_scale);
	taking_damage_sprite.setColor(player_color);

	dead_on_ground_texture = *Singleton<AssetManager>().Get()->GetTexture("Images/kaltar_dead_on_ground.png");
	dead_on_ground_sprite = sf::Sprite(dead_on_ground_texture);
	dead_on_ground_sprite.setScale(idle_sprite_scale, idle_sprite_scale);
	dead_on_ground_sprite.setColor(player_color);

	if (!buffer0.loadFromFile("Sound/Hit0.wav")) {
		throw exception("Sound file not found");
	} else {
		hit_sounds.push_back(sf::Sound());
		hit_sounds[0].setBuffer(buffer0);
		hit_sounds[0].setVolume(20 * (Singleton<Settings>().Get()->effects_volume / 100.0f));
	}

	if (!buffer1.loadFromFile("Sound/Hit1.wav")) {
		throw exception("Sound file not found");
	} else {
		hit_sounds.push_back(sf::Sound());
		hit_sounds[1].setBuffer(buffer1);
		hit_sounds[1].setVolume(20 * (Singleton<Settings>().Get()->effects_volume / 100.0f));
	}

	if (!buffer2.loadFromFile("Sound/Hit2.wav")) {
		throw exception("Sound file not found");
	} else {
		hit_sounds.push_back(sf::Sound());
		hit_sounds[2].setBuffer(buffer2);
		hit_sounds[2].setVolume(20 * (Singleton<Settings>().Get()->effects_volume / 100.0f));
	}

	if (!bufferLand.loadFromFile("Sound/Land.wav")) {
		throw exception("Sound file not found");
	} else {
		soundLand.setBuffer(bufferLand);
		soundLand.setVolume(Singleton<Settings>().Get()->effects_volume);
	}

	if (!bufferJump.loadFromFile("Sound/Jump.wav")) {
		throw exception("Sound file not found");
	} else {
		soundJump.setBuffer(bufferJump);
		soundJump.setVolume(20 * (Singleton<Settings>().Get()->effects_volume / 100.0f));
	}

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

	if (!firing_projectile_buffer.loadFromFile("Sound/kaltar_firing.wav")) {
		throw exception("Sound file not found");
	} else {
		firing_projectile_sound.setBuffer(firing_projectile_buffer);
		firing_projectile_sound.setVolume(Singleton<Settings>().Get()->effects_volume * 2.0f);
	}

	if (!landing_buffer.loadFromFile("Sound/player_landing.wav")) {
		throw exception("Sound file not found");
	} else {
		landing_sound.setBuffer(landing_buffer);
		landing_sound.setVolume(Singleton<Settings>().Get()->effects_volume / 10.0f);
	}
}

void PlayerCharacter::UpdatePlayerCharacter(sf::Int64 curr_time) {
	current_time = curr_time;

	if (IsDodging()) {
		can_take_input = false;
		if (facing_right) {
			velocity.x = dodge_velocity_x;
		} else {
			velocity.x = -dodge_velocity_x;
		}
	} else if (hit_stun_start_time + hit_stun_duration > current_time) {
		can_take_input = false;
	} else if (time_of_last_attack + attack_duration > current_time) {
		can_take_input = false;

		if (!in_the_air) {
			velocity.x = 0.0f;
			velocity.y = 0.0f;
		}
	} else if (time_of_last_fire + fire_duration > current_time) {
		can_take_input = false;
	} else {
		if (!in_the_air) {
			lock_facing_direction_when_hit = false;
		}
		can_take_input = true;
	}

	if (velocity.y > terminal_velocity * 0.99f) {
		hit_terminal_velocity = true;
	}

	if (was_in_air && !in_the_air) {
		hit_terminal_velocity = false;
		//landing_sound.play();
	}

	was_in_air = in_the_air;
}

void PlayerCharacter::UpdateProjectiles(sf::Int64 curr_time, sf::Int64 frame_delta) {
	for (int i = 0; i < (int)(projectiles.size()); i++) {
		if (projectiles[i]->is_active) {
			projectiles[i]->Update(frame_delta);
			projectiles[i]->UpdateProjectile(curr_time);
		}
	}
}

void PlayerCharacter::DrawProjectiles(sf::Vector2f camera_position, sf::Int64 curr_time) {
	for (int i = 0; i < (int)(projectiles.size()); i++) {
		projectiles[i]->Draw(camera_position, curr_time);
	}
}

void PlayerCharacter::Draw(sf::Vector2f camera_position) {
	sf::Color players_color_this_cycle = sf::Color(idle_sprite.getColor());

	if (IsInPostHitInvincibility()) {
		if (players_color_this_cycle.a != 50) {
			players_color_this_cycle.a = 50;

			idle_sprite.setColor(players_color_this_cycle);
			attack_sprite.setColor(players_color_this_cycle);
			fire_sprite.setColor(players_color_this_cycle);
			running_animation->SetColor(players_color_this_cycle);
			dodge_sprite.setColor(players_color_this_cycle);
			taking_damage_sprite.setColor(players_color_this_cycle);
			dead_on_ground_sprite.setColor(players_color_this_cycle);
		}
	} else {
		if (players_color_this_cycle.a != 255) {
			players_color_this_cycle.a = 255;

			idle_sprite.setColor(players_color_this_cycle);
			attack_sprite.setColor(players_color_this_cycle);
			fire_sprite.setColor(players_color_this_cycle);
			running_animation->SetColor(players_color_this_cycle);
			dodge_sprite.setColor(players_color_this_cycle);
			taking_damage_sprite.setColor(players_color_this_cycle);
			dead_on_ground_sprite.setColor(players_color_this_cycle);
		}
	}

	if (facing_right) {
		idle_sprite.setScale(idle_sprite_scale, idle_sprite.getScale().y);
		attack_sprite.setScale(idle_sprite_scale, idle_sprite.getScale().y);
		fire_sprite.setScale(idle_sprite_scale, idle_sprite.getScale().y);
		dodge_sprite.setScale(idle_sprite_scale, idle_sprite.getScale().y);
		taking_damage_sprite.setScale(idle_sprite_scale, idle_sprite.getScale().y);
		dead_on_ground_sprite.setScale(idle_sprite_scale, idle_sprite.getScale().y);
	} else {
		idle_sprite.setScale(-idle_sprite_scale, idle_sprite.getScale().y);
		attack_sprite.setScale(-idle_sprite_scale, idle_sprite.getScale().y);
		fire_sprite.setScale(-idle_sprite_scale, idle_sprite.getScale().y);
		dodge_sprite.setScale(-idle_sprite_scale, idle_sprite.getScale().y);
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
		} else {
			taking_damage_sprite.setPosition(sf::Vector2f(x + width - camera_position.x, y - camera_position.y));
			dead_on_ground_sprite.setPosition(sf::Vector2f(x + width - camera_position.x, y - camera_position.y));
		}

		if (in_the_air) {
			render_window->draw(taking_damage_sprite);
		} else {
			render_window->draw(dead_on_ground_sprite);
		}

	} else if (IsDodging()) {
		if (facing_right) {
			dodge_sprite.setPosition(sf::Vector2f(x - camera_position.x, y - camera_position.y));
		}
		else {
			dodge_sprite.setPosition(sf::Vector2f(x + width - camera_position.x, y - camera_position.y));
		}
		render_window->draw(dodge_sprite);
	} else if (hit_stun_start_time + hit_stun_duration > current_time) {
		if (facing_right) {
			taking_damage_sprite.setPosition(sf::Vector2f(x - camera_position.x, y - camera_position.y));
		}
		else {
			taking_damage_sprite.setPosition(sf::Vector2f(x + width - camera_position.x, y - camera_position.y));
		}
		render_window->draw(taking_damage_sprite);
	} else if (time_of_last_attack + attack_duration > current_time) {
		if (facing_right) {
			attack_sprite.setPosition(sf::Vector2f(x - camera_position.x, y - camera_position.y));
		} else {
			attack_sprite.setPosition(sf::Vector2f(x + width - camera_position.x, y - camera_position.y));
		}
		render_window->draw(attack_sprite);
	} else if (time_of_last_fire + fire_duration > current_time) {
		if (facing_right) {
			fire_sprite.setPosition(sf::Vector2f(x - camera_position.x, y - camera_position.y));
		} else {
			fire_sprite.setPosition(sf::Vector2f(x + width - camera_position.x, y - camera_position.y));
		}
		render_window->draw(fire_sprite);
	} else if (velocity.x == 0) {
		if (facing_right) {
			idle_sprite.setPosition(sf::Vector2f(x - camera_position.x, y - camera_position.y));
		}
		else {
			idle_sprite.setPosition(sf::Vector2f(x + width - camera_position.x, y - camera_position.y));
		}
		//idle_sprite.setPosition(sf::Vector2f((x + width / 2.0f) - (idle_texture.getSize().x * idle_sprite.getScale().x / 2.0f) - camera_position.x,
		//	(y + height / 2.0f) - (idle_texture.getSize().y * idle_sprite.getScale().y / 2.0f) - camera_position.y));
		render_window->draw(idle_sprite);
	} else {
		running_animation->Draw(camera_position, sf::Vector2f(x + width / 2.0f, y + height / 2.0f));
	}
}

void PlayerCharacter::HandleLeftStickInput(float horizontal, float vertical) {
	if (can_take_input && hit_points > 0) {
		velocity.x = (horizontal / 100.0f) * speed;

		if (lock_facing_direction_when_hit && hit_stun_start_time + hit_stun_duration < current_time) {
			lock_facing_direction_when_hit = false;
		}
	}
}

void PlayerCharacter::HandleButtonAPress() {
	if (!in_the_air && hit_points > 0) {
		velocity.y = -(jump_power);
		in_the_air = true;
		//soundJump.play();
	}
}

void PlayerCharacter::HandleButtonARelease() {
}

void PlayerCharacter::HandleButtonBPress() {
	if (!IsDodging() && !in_the_air && hit_points > 0) {
		dodge_start_time = current_time;
		dodge_invincibility_start_time = current_time;
	}
}

void PlayerCharacter::HandleButtonBRelease() {
}

void PlayerCharacter::HandleButtonXPress() {
	if (time_of_last_attack + attack_cooldown < current_time && hit_points > 0) {
		time_of_last_attack = current_time;

		sf::Vector2f knock_back = sf::Vector2f();
		knock_back.x = 2.0f;
		knock_back.y = 6.0f;

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
		bool hit_something = false;

		if (hit_objects.size() > 0) {
			for (int i = 0; i < (int)hit_objects.size(); i++) {
				if (!hit_objects[i]->only_collide_with_platforms &&
					(hit_objects[i]->entity_type == Singleton<World>::Get()->ENTITY_TYPE_DRONE ||
						hit_objects[i]->entity_type == Singleton<World>::Get()->ENTITY_TYPE_GRUNT ||
						hit_objects[i]->entity_type == Singleton<World>::Get()->ENTITY_TYPE_GUNNER ||
						hit_objects[i]->entity_type == Singleton<World>::Get()->ENTITY_TYPE_CHARGER)) {
					hit_something = true;
					((Creature*)(hit_objects[i]))->TakeHit(2, 500, knock_back, false, true);
					sword_hitting_enemy_sound.play();
				} else if (!hit_objects[i]->only_collide_with_platforms &&
					hit_objects[i]->entity_type == Singleton<World>::Get()->ENTITY_TYPE_PROJECTILE) {
					hit_something = true;
					((Projectile*)(hit_objects[i]))->fired_velocity.x *= -1.5f;

					((Projectile*)(hit_objects[i]))->entities_excluded_from_collision.erase(((Projectile*)(hit_objects[i]))->entities_excluded_from_collision.begin(), ((Projectile*)(hit_objects[i]))->entities_excluded_from_collision.end());

					((Projectile*)(hit_objects[i]))->ExcludeFromCollision(Singleton<World>::Get()->ENTITY_TYPE_PROJECTILE);
					((Projectile*)(hit_objects[i]))->ExcludeFromCollision(Singleton<World>::Get()->ENTITY_TYPE_RIGID_BODY);
					((Projectile*)(hit_objects[i]))->ExcludeFromCollision(Singleton<World>::Get()->ENTITY_TYPE_HIT_BOX);
					if (hit_sounds.size() > 0) {
						hit_sounds[rand() % 3].play();
					}
				} else if (hit_objects[i]->entity_type == Singleton<World>::Get()->ENTITY_TYPE_STALAGTITE) {
					hit_something = true;
					Singleton<World>::Get()->HitStalagtite();
					if (hit_sounds.size() > 0) {
						hit_sounds[rand() % 3].play();
					}
				}
			}
		}

		if (!hit_something) {
			sword_whiffing_sound.play();
		}

		Singleton<World>::Get()->ScreenShake(hit_objects.size() > 0 ? 1.0f : 0.0f);
	}
}

void PlayerCharacter::HandleButtonXRelease() {
}

void PlayerCharacter::HandleButtonYPress() {
	if (time_of_last_fire + fire_duration < current_time && hit_points > 0) {
		if (!in_the_air) {
			velocity.x = 0.0f;
			velocity.y = 0.0f;
		}

		for (int i = 0; i < (int)(projectiles.size()); i++) {
			if (!projectiles[i]->is_active) {
				float x_velocity = 10.0f;
				sf::Vector2f kick_back = sf::Vector2f(1.6f, 0.0f);
				sf::Vector2f starting_position = sf::Vector2f(x - projectiles[i]->width, y);

				if (!facing_right) {
					x_velocity *= -1.0f;
				} else {
					kick_back.x *= -1.0f;
					starting_position.x += width + projectiles[i]->width;
				}

				TakeHit(0, 200, kick_back, false, true);
				projectiles[i]->Fire(current_time, starting_position, sf::Vector2f(x_velocity, 0.0f));
				projectiles[i]->ExcludeFromCollision(entity_type);
				projectiles[i]->ExcludeFromCollision(HitBox->entity_type);

				time_of_last_fire = current_time;

				firing_projectile_sound.play();

				break;
			}
		}
	}
}

void PlayerCharacter::HandleButtonYRelease() {
}

void PlayerCharacter::HandleButtonRightBumperPress() {
}

void PlayerCharacter::HandleButtonRightBumperRelease() {
}

void PlayerCharacter::HandleButtonLeftBumperPress() {
}

void PlayerCharacter::HandleButtonLeftBumperRelease() {
}

void PlayerCharacter::HandleButtonSelectPress() {
}

void PlayerCharacter::HandleButtonSelectRelease() {
}

void PlayerCharacter::HandleButtonStartPress() {
}

void PlayerCharacter::HandleButtonStartRelease() {
}