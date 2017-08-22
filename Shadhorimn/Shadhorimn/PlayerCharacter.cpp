using namespace std;
#include <iostream>
#include "PlayerCharacter.h"

PlayerCharacter::PlayerCharacter(sf::RenderWindow *window, sf::Vector2f position, sf::Vector2f dimensions, bool subject_to_gravity) : 
	Creature::Creature(window, position, dimensions, subject_to_gravity) {
	HitBox = new RigidBody(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(40.0f, 10.0f), false, false);

	speed = 0.5f;
	jump_power = 0.9f;

	sf::RectangleShape shape(dimensions);
	shape.setFillColor(sf::Color::Yellow);
	shape.setPosition(position);

	rectangle_shape = shape;
}

void PlayerCharacter::HandleLeftStickInput(float horizontal, float vertical) {
	velocity.x = (horizontal / 100.0f) * speed;
}

void PlayerCharacter::HandleButtonAPress() {
	if (!in_the_air) {
		velocity.y = -(jump_power);
		in_the_air = true;
	}
}

void PlayerCharacter::HandleButtonARelease() {
}

void PlayerCharacter::HandleButtonBPress() {
}

void PlayerCharacter::HandleButtonBRelease() {
}

void PlayerCharacter::HandleButtonXPress() {
	float knock_back_x = 0.2f;
	float knock_back_y = 0.6f;

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
		hit_objects[i]->velocity.x = knock_back_x;
		hit_objects[i]->velocity.y = -knock_back_y;
	}

	cout << hit_objects.size() << "\n";
}

void PlayerCharacter::HandleButtonXRelease() {
}

void PlayerCharacter::HandleButtonYPress() {
}

void PlayerCharacter::HandleButtonYRelease() {
}