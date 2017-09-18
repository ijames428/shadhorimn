#ifndef CHARGER_H
#define CHARGER_H

using namespace std;
#include <SFML/Graphics.hpp>
#include "Creature.h"
#include "PlayerCharacter.h"
#include "Projectile.h"

class Charger : public Creature {
private:
	sf::Sound hitting_player_sound;
	sf::SoundBuffer hitting_player_buffer;
	sf::Sound hitting_wall_sound;
	sf::SoundBuffer hitting_wall_buffer;
	std::vector<Projectile*> projectiles = std::vector<Projectile*>();
	float movement_speed;
	bool is_charging;
	float aggro_radius;
	float charge_speed;
	float charge_speed_second_stage;
	//sf::Int64 time_between_firing;
	//sf::Int64 time_of_last_firing;
	PlayerCharacter* target;
	float attack_radius;
	sf::Int64 time_of_last_attack;
	sf::Int64 duration_of_attack;
	sf::Int64 time_between_attacks;
	RigidBody* HitBox;
	RigidBody* WallDetector;
	sf::Vector2f charge_velocity;
	int starting_hit_points;
public:
	Charger(sf::RenderWindow *window, sf::Vector2f position = sf::Vector2f(0.0f, 0.0f), sf::Vector2f dimensions = sf::Vector2f(0.0f, 0.0f), bool subject_to_gravity = true);
	void UpdateBehavior(sf::Int64 curr_time);
	void Draw(sf::Vector2f camera_position);
	void UpdateProjectiles(sf::Int64 curr_time, sf::Int64 frame_delta);
	void DrawProjectiles(sf::Vector2f camera_position, sf::Int64 curr_time);
	void StartCharge();
	bool IsInSecondStage();
	void FireSecondStageProjectiles();
};

#endif