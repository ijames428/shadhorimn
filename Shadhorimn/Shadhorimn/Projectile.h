#ifndef PROJECTILE_H
#define PROJECTILE_H

using namespace std;
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "RigidBody.h"
#include "Creature.h"

class Projectile : public RigidBody {
protected:
	sf::Texture impact_texture;
	sf::Sprite impact_sprite;
	sf::Sound hit_sound;
	sf::SoundBuffer buffer0;
	sf::SoundBuffer buffer1;
	sf::SoundBuffer buffer2;
	sf::Int64 current_time;
	sf::Int64 fired_time;
	sf::Vector2f fired_position;
	sf::Int64 range;
	float speed;
	sf::Vector2f impact_position;
	sf::Int64 time_of_impact;
	sf::Int64 duration_of_impact_animation;
public:
	sf::Vector2f fired_velocity;
	bool is_active;
	Projectile(sf::RenderWindow *window, sf::Vector2f position = sf::Vector2f(0.0f, 0.0f), sf::Vector2f dimensions = sf::Vector2f(0.0f, 0.0f), bool subject_to_gravity = true);
	sf::CircleShape circle_shape;
	sf::RenderWindow *render_window;
	void Draw(sf::Vector2f camera_position, sf::Int64 curr_time);
	void UpdateProjectile(sf::Int64 curr_time);
	void Fire(sf::Int64 curr_time, sf::Vector2f position = sf::Vector2f(0.0f, 0.0f), sf::Vector2f velocity = sf::Vector2f(0.0f, 0.0f));
	void ExcludeFromCollision(int ent_typ);
};

#endif