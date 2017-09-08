using namespace std;
#include <iostream>
#include "RigidBody.h"
#include "Singleton.h"
#include "World.h"

RigidBody::RigidBody(sf::Vector2f position, sf::Vector2f dimensions, bool subject_to_gravity, bool subject_to_collision) {
	entity_type = "RigidBody";

	x = position.x;
	y = position.y;
	width = dimensions.x;
	height = dimensions.y;
	gravity_enabled = subject_to_gravity;
	collision_enabled = subject_to_collision;
	facing_right = true;
	only_collide_with_platforms = false;

	weight = 10.0f;

	gravity_acceleration = 0.5f;
	terminal_velocity = 20.0f;

	velocity.x = 0.0f;
	velocity.y = 0.0f;

	entities_excluded_from_collision = std::vector<std::string>();
	Singleton<World>::Get()->AddRigidBodyToGrid(this);
}

void RigidBody::Update(sf::Int64 delta_time) {
	float mMovementSpeedTimefactor = delta_time / 10.0f;

	if (mMovementSpeedTimefactor > 2.0f) {
		mMovementSpeedTimefactor = 2.0f;
	}

	future_x = x;
	future_y = y;
	future_width = width;
	future_height = height;

	float pregravity_velocity_y = velocity.y;

	if (gravity_enabled) {
		velocity.y = velocity.y + gravity_acceleration;// * mMovementSpeedTimefactor;
	}

	if (velocity.y > terminal_velocity) {
		velocity.y = terminal_velocity;
	}

	if (velocity.x > 0.0f) {
		facing_right = true;
		if (pregravity_velocity_y == 0.0f && future_y == old_y) {
			if (velocity.x > 0.2f) {
				velocity.x = velocity.x - 0.2f;
			}
			else {
				velocity.x = 0.0f;
			}
		}
	} else if (velocity.x < 0.0f) {
		if (pregravity_velocity_y == 0.0f && future_y == old_y) {
			facing_right = false;
			if (velocity.x < -0.2f) {
				velocity.x = velocity.x + 0.2f;
			}
			else {
				velocity.x = 0.0f;
			}
		}
	}

	future_x = x + velocity.x;// * mMovementSpeedTimefactor;
	if (entity_type == "PlayerCharacter") {
		future_y = y + velocity.y;// * mMovementSpeedTimefactor;
	} else {
		future_y = y + velocity.y;// * mMovementSpeedTimefactor;
	}

	if (old_x != future_x || old_y != future_y || old_width != future_width || old_height != future_height)
	{
		Singleton<World>::Get()->MoveRigidBodyInGrid(this);

		if (collision_enabled) {
			ChangeFutureValuesAndVelocityBasedOnCollisions();
		}

		x = future_x;
		y = future_y;
		width = future_width;
		height = future_height;
	}

	old_x = x;
	old_y = y;
	old_width = width;
	old_height = height;
}

void RigidBody::ChangeFutureValuesAndVelocityBasedOnCollisions() {
	for (int w = grid_top_left_x; w <= grid_bot_right_x; w++) {
		for (int h = grid_top_left_y; h <= grid_bot_right_y; h++) {
			std::vector<RigidBody*> colliders = Singleton<World>::Get()->GetObjectsInGridLocation(w, h);

			if (colliders.size() > 1) {
				for (int c = 0; c < (int)colliders.size(); c++) {
					if (id != colliders[c]->id && colliders[c]->collision_enabled &&
						!colliders[c]->only_collide_with_platforms &&
						(std::find(entities_excluded_from_collision.begin(), entities_excluded_from_collision.end(), colliders[c]->entity_type) == entities_excluded_from_collision.end())) {
						bool horizontal_collision = AreTheRigidBodiesCollidingHorizontally(this, colliders[c]);
						bool vertical_collision = AreTheRigidBodiesCollidingVertically(this, colliders[c]);

						if (horizontal_collision) {
							if (x + (width / 2.0f) < colliders[c]->x + (colliders[c]->width / 2.0f)) {
								future_x = x + (colliders[c]->x - (x + future_width));
							} else {
								future_x = x - (x - (colliders[c]->x + colliders[c]->width));
							}
							velocity.x = 0.0f;
						}

						if (vertical_collision) {
							if (y + (height / 2.0f) < colliders[c]->y + (colliders[c]->height / 2.0f)) {
								future_y = y + (colliders[c]->y - (y + future_height));
								in_the_air = false;
							} else {
								future_y = y - (y - (colliders[c]->y + colliders[c]->height));
							}
							velocity.y = 0.0f;
						}
					}
				}
			}
		}
	}
}

bool RigidBody::AreTheRigidBodiesCollidingHorizontally(RigidBody* rb1, RigidBody* rb2, bool just_check_no_response) {
	bool rb1_is_horizontally_outside_rb2 = (rb1->future_x + rb1->future_width < rb2->x || rb1->future_x > rb2->x + rb2->width);
	bool rb1_is_vertically_outside_rb2 = false;
	if (just_check_no_response) {
		rb1_is_vertically_outside_rb2 = (rb1->y + rb1->future_height < rb2->y || rb1->y > rb2->y + rb2->height);
	} else {
		rb1_is_vertically_outside_rb2 = ((rb1->y + 5.0f) + (rb1->future_height - 10.0f) < rb2->y || rb1->y + 5.0f > rb2->y + rb2->height);
	}

	if (rb1_is_horizontally_outside_rb2 || rb1_is_vertically_outside_rb2) {
		return false;
	}

	return true;
}

bool RigidBody::AreTheRigidBodiesCollidingVertically(RigidBody* rb1, RigidBody* rb2, bool just_check_no_response) {
	bool rb1_is_horizontally_outside_rb2 = false;
	bool rb1_is_vertically_outside_rb2 = (rb1->future_y + rb1->future_height < rb2->y || rb1->future_y > rb2->y + rb2->height);

	if (just_check_no_response) {
		rb1_is_horizontally_outside_rb2 = (rb1->x + rb1->future_width < rb2->x || rb1->x > rb2->x + rb2->width);
	} else {
		rb1_is_horizontally_outside_rb2 = ((rb1->x + 5.0f) + (rb1->future_width - 10.0f) < rb2->x || rb1->x + 5.0f > rb2->x + rb2->width);
	}

	if (rb1_is_horizontally_outside_rb2 || rb1_is_vertically_outside_rb2) {
		return false;
	}

	return true;
}

std::vector<RigidBody*> RigidBody::GetCollidersRigidBodyIsCollidingWith() {
	std::vector<RigidBody*> collidersBeingCollidedWith = std::vector<RigidBody*>();

	for (int w = grid_top_left_x; w <= grid_bot_right_x; w++) {
		for (int h = grid_top_left_y; h <= grid_bot_right_y; h++) {
			std::vector<RigidBody*> colliders = Singleton<World>::Get()->GetObjectsInGridLocation(w, h);

			if (colliders.size() > 1) {
				for (int c = 0; c < (int)colliders.size(); c++) {
					if (id != colliders[c]->id && (std::find(entities_excluded_from_collision.begin(), entities_excluded_from_collision.end(), colliders[c]->entity_type) == entities_excluded_from_collision.end())) {
						if (AreTheRigidBodiesCollidingHorizontally(this, colliders[c], true) && 
							AreTheRigidBodiesCollidingVertically(this, colliders[c], true)) {
							bool already_in_list = false;

							for (int i = 0; i < (int)collidersBeingCollidedWith.size(); i++) {
								if (collidersBeingCollidedWith[i]->entity_type == colliders[c]->entity_type &&
									collidersBeingCollidedWith[i]->id == colliders[c]->id) {
									already_in_list = true;
									break;
								}
							}

							if (!already_in_list) {
								collidersBeingCollidedWith.push_back(colliders[c]);
							}
						}
					}
				}
			}
		}
	}

	return collidersBeingCollidedWith;
}

void RigidBody::SetGridTopLeftX(int new_value) {
	grid_top_left_x = new_value;
};
void RigidBody::SetGridTopLeftY(int new_value) { 
	grid_top_left_y = new_value; 
};
void RigidBody::SetGridBotRightX(int new_value) { 
	grid_bot_right_x = new_value; 
};
void RigidBody::SetGridBotRightY(int new_value) { 
	grid_bot_right_y = new_value; 
};

float RigidBody::GetDistanceBetweenTwoPoints(sf::Vector2f point_a, sf::Vector2f point_b) {
	return sqrt((point_a.x - point_b.x) * (point_a.x - point_b.x) + (point_a.y - point_b.y) * (point_a.y - point_b.y));
}