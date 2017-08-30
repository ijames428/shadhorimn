using namespace std;
#include <iostream>
#include "PlayerCharacter.h"
#include "Singleton.h"
#include "World.h"

PlayerCharacter::PlayerCharacter(sf::RenderWindow *window, sf::Vector2f position, sf::Vector2f dimensions, bool subject_to_gravity) : 
	Creature::Creature(window, position, dimensions, subject_to_gravity) {
	entity_type = "PlayerCharacter";
	hit_points = 10;

	HitBox = new RigidBody(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(40.0f, 10.0f), false, false);
	test_projectile = new Projectile(window, position, sf::Vector2f(10.0f, 10.0f), false);
	test_projectile->ExcludeFromCollision(entity_type);

	entities_excluded_from_collision.push_back("Projectile");

	speed = 5.0f;
	jump_power = 12.0f;

	sf::RectangleShape shape(dimensions);
	shape.setFillColor(sf::Color::Yellow);
	shape.setPosition(position);

	rectangle_shape = shape;

	if (!buffer0.loadFromFile("Sound/Hit0.wav")) {
		throw exception("Sound file not found");
	} else {
		hit_sounds.push_back(sf::Sound());
		hit_sounds[0].setBuffer(buffer0);
	}

	if (!buffer1.loadFromFile("Sound/Hit1.wav")) {
		throw exception("Sound file not found");
	} else {
		hit_sounds.push_back(sf::Sound());
		hit_sounds[1].setBuffer(buffer1);
	}

	if (!buffer2.loadFromFile("Sound/Hit2.wav")) {
		throw exception("Sound file not found");
	} else {
		hit_sounds.push_back(sf::Sound());
		hit_sounds[2].setBuffer(buffer2);
	}

	if (!bufferLand.loadFromFile("Sound/Land.wav")) {
		throw exception("Sound file not found");
	}
	else {
		soundLand.setBuffer(bufferLand);
	}

	if (!bufferJump.loadFromFile("Sound/Jump.wav")) {
		throw exception("Sound file not found");
	}
	else {
		soundJump.setBuffer(bufferJump);
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
	float knock_back_x = 2.0f;
	float knock_back_y = 6.0f;

	if (facing_right) {
		HitBox->x = x + width;
	} else {
		knock_back_x *= -1.0f;
		HitBox->x = x - HitBox->width;
	}
	HitBox->y = y;
	HitBox->Update(0);
	std::vector<RigidBody*> hit_objects = HitBox->GetCollidersRigidBodyIsCollidingWith();
	
	for (int i = 0; i < (int)hit_objects.size(); i++) {
		if (hit_sounds.size() > 0) {
			hit_sounds[rand() % 3].play();
		}

		if (hit_objects[i]->entity_type == "Drone" || hit_objects[i]->entity_type == "Creature") {
			hit_objects[i]->velocity.x = knock_back_x;
			hit_objects[i]->velocity.y = -knock_back_y;
			((Creature*)(hit_objects[i]))->TakeHit(1, 1000);
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