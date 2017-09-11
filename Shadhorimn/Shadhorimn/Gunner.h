#ifndef GUNNER_H
#define GUNNER_H

using namespace std;
#include <SFML/Graphics.hpp>
#include "Creature.h"
#include "PlayerCharacter.h"
#include "Projectile.h"

class Gunner : public Creature {
private:
	std::vector<Projectile*> projectiles = std::vector<Projectile*>();
	float movement_speed;
	bool is_aggroed;
	float aggro_radius;
	sf::Int64 time_between_firing;
	sf::Int64 time_of_last_firing;
	sf::Int64 fire_duration;
	PlayerCharacter* target;
	sf::Color gunner_color;
	SpriteAnimation* running_animation;
	sf::Texture idle_texture;
	sf::Sprite idle_sprite;
	float idle_sprite_scale;
	sf::Texture fire_texture;
	sf::Sprite fire_sprite;
public:
	Gunner(sf::RenderWindow *window, sf::Vector2f position = sf::Vector2f(0.0f, 0.0f), sf::Vector2f dimensions = sf::Vector2f(0.0f, 0.0f), bool subject_to_gravity = true);
	void UpdateBehavior(sf::Int64 curr_time);
	void Draw(sf::Vector2f camera_position);
	void UpdateProjectiles(sf::Int64 curr_time, sf::Int64 frame_delta); 
	void DrawProjectiles(sf::Vector2f camera_position, sf::Int64 curr_time);
};

#endif