using namespace std;
#include <iostream>
#include "PlayerCharacter.h"
#include "Singleton.h"
#include "Settings.h"
#include "World.h"

PlayerCharacter::PlayerCharacter(sf::RenderWindow *window, sf::Vector2f position, sf::Vector2f dimensions, bool subject_to_gravity) : 
	Creature::Creature(window, position, dimensions, subject_to_gravity) {
	entity_type = Singleton<World>::Get()->ENTITY_TYPE_PLAYER_CHARACTER;
	hit_points = 10;

	HitBox = new RigidBody(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(40.0f, 10.0f), false, false);
	HitBox->entities_excluded_from_collision.push_back(entity_type);
	HitBox->entity_type = Singleton<World>::Get()->ENTITY_TYPE_PLAYER_CHARACTER;

	test_projectile = new Projectile(window, position, sf::Vector2f(10.0f, 10.0f), false);
	test_projectile->ExcludeFromCollision(entity_type);
	test_projectile->ExcludeFromCollision(HitBox->entity_type);

	entities_excluded_from_collision.push_back(test_projectile->entity_type);

	speed = 5.0f;
	jump_power = 12.0f;

	sf::RectangleShape shape(dimensions);
	shape.setFillColor(sf::Color::Yellow);
	shape.setPosition(position);

	rectangle_shape = shape;

	idle_sprite_scale = 0.12f;
	idle_texture.loadFromFile("Images/Kaltar_Idle.png");
	idle_sprite = sf::Sprite(idle_texture);
	idle_sprite.setScale(idle_sprite_scale, idle_sprite_scale);
	idle_sprite.setColor(sf::Color::Cyan);

	running_animation = new SpriteAnimation(render_window, "Images/Kaltar_Running.png", 582, 522, 91, 9, 11, 0.12f, sf::Color::Cyan);

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
}

void PlayerCharacter::Draw(sf::Vector2f camera_position) {
	if (facing_right) {
		idle_sprite.setScale(idle_sprite_scale, idle_sprite.getScale().y);
	} else {
		idle_sprite.setScale(-idle_sprite_scale, idle_sprite.getScale().y);
	}

	if (facing_right != running_animation->IsFacingRight()) {
		running_animation->Flip();
	}

	if (velocity.x == 0) {
		idle_sprite.setPosition(sf::Vector2f((x + width / 2.0f) - (idle_texture.getSize().x * idle_sprite.getScale().x / 2.0f) - camera_position.x,
			(y + height / 2.0f) - (idle_texture.getSize().y * idle_sprite.getScale().y / 2.0f) - camera_position.y));
		render_window->draw(idle_sprite);
	} else {
		running_animation->Draw(camera_position, sf::Vector2f(x + width / 2.0f, y + height / 2.0f));
	}
}

void PlayerCharacter::HandleLeftStickInput(float horizontal, float vertical) {
	velocity.x = (horizontal / 100.0f) * speed;
}

void PlayerCharacter::HandleButtonAPress() {
	if (!in_the_air) {
		velocity.y = -(jump_power);
		in_the_air = true;
		soundJump.play();
	}
}

void PlayerCharacter::HandleButtonARelease() {
}

void PlayerCharacter::HandleButtonBPress() {
}

void PlayerCharacter::HandleButtonBRelease() {
}

void PlayerCharacter::HandleButtonXPress() {
	sf::Vector2f knock_back = sf::Vector2f();
	knock_back.x = 2.0f;
	knock_back.y = 6.0f;

	if (facing_right) {
		HitBox->x = x + width;
	} else {
		knock_back.x *= -1.0f;
		HitBox->x = x - HitBox->width;
	}
	HitBox->y = y;
	HitBox->Update(0);
	std::vector<RigidBody*> hit_objects = HitBox->GetCollidersRigidBodyIsCollidingWith();
	
	for (int i = 0; i < (int)hit_objects.size(); i++) {
		if (hit_sounds.size() > 0) {
			hit_sounds[rand() % 3].play();
		}

		if (!hit_objects[i]->only_collide_with_platforms &&
			(hit_objects[i]->entity_type == Singleton<World>::Get()->ENTITY_TYPE_DRONE ||
			hit_objects[i]->entity_type == Singleton<World>::Get()->ENTITY_TYPE_GRUNT ||
			hit_objects[i]->entity_type == Singleton<World>::Get()->ENTITY_TYPE_GUNNER ||
			hit_objects[i]->entity_type == Singleton<World>::Get()->ENTITY_TYPE_CHARGER)) {
			((Creature*)(hit_objects[i]))->TakeHit(1, 1000, knock_back);
		}
	}

	Singleton<World>::Get()->ScreenShake(hit_objects.size() > 0 ? 1.0f : 0.0f);
}

void PlayerCharacter::HandleButtonXRelease() {
}

void PlayerCharacter::HandleButtonYPress() {
	float x_velocity = 10.0f;
	sf::Vector2f starting_position = sf::Vector2f(x - test_projectile->width, y);
	if (!facing_right) {
		x_velocity *= -1.0f;
	} else {
		starting_position.x += width + test_projectile->width;
	}
	test_projectile->Fire(current_time, starting_position, sf::Vector2f(x_velocity, 0.0f));
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
	Singleton<World>::Get()->paused = !Singleton<World>::Get()->paused;
}

void PlayerCharacter::HandleButtonStartRelease() {
}