#ifndef PLAYERCHARACTER_H
#define PLAYERCHARACTER_H

using namespace std;
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "SpriteAnimation.h"
#include "Projectile.h"
#include "Creature.h"

class PlayerCharacter : public Creature {
	private:
		SpriteAnimation* running_animation;
		sf::Texture idle_texture;
		sf::Sprite idle_sprite;
		float idle_sprite_scale;
		std::vector<sf::Sound> hit_sounds = std::vector<sf::Sound>();
		sf::SoundBuffer buffer0;
		sf::SoundBuffer buffer1;
		sf::SoundBuffer buffer2;
		sf::Sound soundLand;
		sf::SoundBuffer bufferLand;
		sf::Sound soundJump;
		sf::SoundBuffer bufferJump;
		RigidBody* HitBox;
		bool was_in_air;
	public:
		int max_hit_points = 10;
		Projectile* test_projectile;
		PlayerCharacter(sf::RenderWindow *window, sf::Vector2f position = sf::Vector2f(0.0f, 0.0f), sf::Vector2f dimensions = sf::Vector2f(0.0f, 0.0f), bool subject_to_gravity = true);
		void HandleLeftStickInput(float horizontal, float vertical);
		void HandleButtonAPress();
		void HandleButtonARelease();
		void HandleButtonBPress();
		void HandleButtonBRelease();
		void HandleButtonXPress();
		void HandleButtonXRelease();
		void HandleButtonYPress();
		void HandleButtonYRelease();
		void HandleButtonRightBumperPress();
		void HandleButtonRightBumperRelease();
		void HandleButtonLeftBumperPress();
		void HandleButtonLeftBumperRelease();
		void HandleButtonSelectPress();
		void HandleButtonSelectRelease();
		void HandleButtonStartPress();
		void HandleButtonStartRelease();
		void Draw(sf::Vector2f camera_position);
};

#endif