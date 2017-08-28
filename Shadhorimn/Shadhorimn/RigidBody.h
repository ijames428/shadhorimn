#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <SFML/Graphics.hpp>

class RigidBody {
	private:
		bool AreTheRigidBodiesCollidingHorizontally(RigidBody* rb1, RigidBody* rb2);
		bool AreTheRigidBodiesCollidingVertically(RigidBody* rb1, RigidBody* rb2);
	public:
		std::vector<std::string> entities_excluded_from_collision;
		std::string entity_type;
		float x;
		float y;
		float width;
		float height;
		float future_x;
		float future_y;
		float future_width;
		float future_height;
		float old_x;
		float old_y;
		float old_width;
		float old_height;
		float gravity_acceleration;
		float terminal_velocity;
		float weight;
		bool gravity_enabled;
		bool collision_enabled;
		bool facing_right;
		bool in_the_air;
		int id = rand();
		int grid_top_left_x;
		int grid_top_left_y;
		int grid_bot_right_x;
		int grid_bot_right_y;
		void SetGridTopLeftX(int new_value);
		void SetGridTopLeftY(int new_value);
		void SetGridBotRightX(int new_value);
		void SetGridBotRightY(int new_value);
		sf::Vector2f velocity;
		RigidBody(sf::Vector2f position, sf::Vector2f dimensions, bool subject_to_gravity = false, bool subject_to_collision = true);
		void Update(sf::Int64 delta_time);
		void ChangeFutureValuesAndVelocityBasedOnCollisions();
		std::vector<RigidBody*> GetCollidersRigidBodyIsCollidingWith();
		static float GetDistanceBetweenTwoPoints(sf::Vector2f point_a, sf::Vector2f point_b);
};

#endif