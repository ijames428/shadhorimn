using namespace std;
#include <iostream>
#include "RigidBody.h"
#include "Singleton.h"
#include "World.h"

RigidBody::RigidBody(sf::Vector2f position, sf::Vector2f dimensions, bool subject_to_gravity, bool subject_to_collision) {
	x = position.x;
	y = position.y;
	width = dimensions.x;
	height = dimensions.y;
	gravity_enabled = subject_to_gravity;
	collision_enabled = subject_to_collision;
	facing_right = true;

	weight = 10.0f;

	gravity_acceleration = 0.003f;
	terminal_velocity = 20.0f;

	velocity.x = 0.0f;
	velocity.y = 0.0f;

	Singleton<World>::Get()->AddRigidBodyToGrid(this);
}

void RigidBody::Update(sf::Int64 delta_time) {
	float mMovementSpeedTimefactor = delta_time / 1000.0f;

	if (mMovementSpeedTimefactor > 2.0f) {
		mMovementSpeedTimefactor = 2.0f;
	}

	future_x = x;
	future_y = y;
	future_width = width;
	future_height = height;

	float pregravity_velocity_y = velocity.y;

	if (gravity_enabled) {
		velocity.y = velocity.y + gravity_acceleration * mMovementSpeedTimefactor;
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

	future_x = x + velocity.x * mMovementSpeedTimefactor;
	future_y = y + velocity.y * mMovementSpeedTimefactor;

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

			if (!colliders.empty()) {
				for (int c = 0; c < (int)colliders.size(); c++) {
					if (id != colliders[c]->id && colliders[c]->collision_enabled) {
						bool horizontal_collision = AreTheRigidBodiesCollidingHorizontally(this, colliders[c]);
						bool vertical_collision = AreTheRigidBodiesCollidingVertically(this, colliders[c]);
						if (horizontal_collision) {
							//if (velocity.x > 0.0f) {
							//	future_x = colliders[c]->x - future_width;
							//} else if (velocity.x < 0.0f) {
							//	future_x = colliders[c]->x + colliders[c]->width;
							//}

							if (x + (width / 2.0f) < colliders[c]->x + (colliders[c]->width / 2.0f)) {
								future_x = x + (colliders[c]->x - (x + future_width));
							} else {
								future_x = x + (x - (colliders[c]->x + colliders[c]->width));
							}
							velocity.x = 0.0f;
						}
						if (vertical_collision) {
							//if (velocity.y > 0.0f) {
							//	future_y = colliders[c]->y - future_height;
							//	in_the_air = false;
							//} else if (velocity.y < 0.0f) {
							//	future_y = colliders[c]->y + colliders[c]->height;
							//}
							if (y + (height / 2.0f) < colliders[c]->y + (colliders[c]->height / 2.0f)) {
								future_y = y + (colliders[c]->y - (y + future_height));
								in_the_air = false;
							} else {
								future_y = y + (y - (colliders[c]->y + colliders[c]->height));
							}
							velocity.y = 0.0f;
						}
					}
				}
			}
		}
	}
}

bool RigidBody::AreTheRigidBodiesCollidingHorizontally(RigidBody* rb1, RigidBody* rb2) {
	bool hor = (rb1->future_x + rb1->future_width < rb2->x || rb1->future_x > rb2->x + rb2->width);
	bool ver = ((rb1->y + 1.0f) + (rb1->future_height - 2.0f) < rb2->y || rb1->y + 1.0f > rb2->y + rb2->height);

	if (hor || ver) {
		return false;
	}

	return true;
}

bool RigidBody::AreTheRigidBodiesCollidingVertically(RigidBody* rb1, RigidBody* rb2) {
	bool hor = ((rb1->x + 1.0f) + (rb1->future_width - 2.0f) < rb2->x || rb1->x + 1.0f > rb2->x + rb2->width);
	bool ver = (rb1->future_y + rb1->future_height < rb2->y || rb1->future_y > rb2->y + rb2->height);

	if (hor || ver) {
		return false;
	}

	return true;
}

std::vector<RigidBody*> RigidBody::GetCollidersRigidBodyIsCollidingWith() {
	std::vector<RigidBody*> collidersBeingCollidedWith = std::vector<RigidBody*>();

	for (int w = grid_top_left_x; w <= grid_bot_right_x; w++) {
		for (int h = grid_top_left_y; h <= grid_bot_right_y; h++) {
			std::vector<RigidBody*> colliders = Singleton<World>::Get()->GetObjectsInGridLocation(w, h);

			if (!colliders.empty()) {
				for (int c = 0; c < (int)colliders.size(); c++) {
					if (id != colliders[c]->id) {
						if (AreTheRigidBodiesCollidingHorizontally(this, colliders[c]) && 
							AreTheRigidBodiesCollidingVertically(this, colliders[c])) {
							collidersBeingCollidedWith.push_back(colliders[c]);
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
	float distance = 0.0f;

	distance = sqrt((point_a.x - point_b.x) * (point_a.x - point_b.x) + (point_a.y - point_b.y) * (point_a.y - point_b.y));

	return distance;
}