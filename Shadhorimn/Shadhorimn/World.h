using namespace std;
#include <SFML/Graphics.hpp>
#include "PlayerCharacter.h"
#include "Checkpoint.h"
#include "EndOfTheGame.h"
#include "RigidBody.h"
#include "Platform.h"
#include "Charger.h"
#include "Camera.h"
#include "Gunner.h"
#include "Drone.h"
#include "Grunt.h"

class World {
	private:
		sf::Vector2f screen_shake_amount;
		sf::Vector2f camera_position;
		sf::Vector2f viewport_position_with_screen_shake;
		sf::Vector2f parallax_background_viewport_position;
		bool player_is_in_combat;
		EndOfTheGame* end_of_the_game_trigger;
		bool player_beat_the_game = false;
		Checkpoint* starting_checkpoint;
		Checkpoint* current_checkpoint;
		sf::Font ringbearer_font = sf::Font();
		sf::Text continue_text;
		sf::Text lives_counter_text;
		sf::Text paused_text;
		int game_over_screen_sprite_transparency;
		sf::Texture level_art_texture;
		sf::Sprite level_art_sprite;
		sf::Texture game_over_texture;
		sf::Sprite game_over_sprite;
		sf::Texture blank_screen_texture;
		sf::Sprite blank_screen_sprite;
		sf::Texture hit_point_texture;
		sf::Texture parallax_background_texture;
		sf::Sprite parallax_background_sprite;
		std::vector<sf::Sprite> players_hit_point_sprites = std::vector<sf::Sprite>();
		std::vector<sf::Sprite> chargers_hit_point_sprites = std::vector<sf::Sprite>();
		sf::Int64 current_time;
		bool screen_shaking;
		sf::Int64 screen_shake_start_time;
		sf::Int64 screen_shake_duration;
		float screen_shake_magnitude;
		Camera* camera;
		sf::RenderWindow* render_window;
		std::vector<Checkpoint*> checkpoints = std::vector<Checkpoint*>();
		std::vector<Platform*> platforms = std::vector<Platform*>();
		std::vector<Gunner*> gunners = std::vector<Gunner*>();
		std::vector<Grunt*> grunts = std::vector<Grunt*>();
		std::vector<Drone*> drones = std::vector<Drone*>();
		RigidBody* boss_health_trigger;
		Platform* end_of_game_door;
		bool fighting_boss;
		Charger* charger;
		int grid_width = 100;
		int grid_height = 100;
		int grid_depth = 10;
		int cell_width = 100;
		int cell_height = 100;
		float combat_music_range;
		std::vector<std::vector<std::vector<RigidBody*>>> Grid = std::vector<std::vector<std::vector<RigidBody*>>>(grid_width, std::vector<std::vector<RigidBody*>>(grid_height, std::vector<RigidBody*>()));
	public:
		const int ENTITY_TYPE_CHARGER = 0;
		const int ENTITY_TYPE_CHECKPOINT = 1;
		const int ENTITY_TYPE_CREATURE = 2;
		const int ENTITY_TYPE_DRONE = 3;
		const int ENTITY_TYPE_END_OF_THE_GAME = 4;
		const int ENTITY_TYPE_GRUNT = 5;
		const int ENTITY_TYPE_GUNNER = 6;
		const int ENTITY_TYPE_PLATFORM = 7;
		const int ENTITY_TYPE_PLAYER_CHARACTER = 8;
		const int ENTITY_TYPE_PROJECTILE = 9;
		const int ENTITY_TYPE_RIGID_BODY = 10;
		const int ENTITY_TYPE_HIT_BOX = 11;
		const int ENTITY_TYPE_WALL_DETECTOR = 12;
		int starting_number_of_lives = 2;
		int current_number_of_lives = 2;
		bool paused;
		PlayerCharacter* main_character;
		World();
		void Init(sf::RenderWindow* window, Camera* cam, PlayerCharacter* character);
		void Update(sf::Int64 current_time, sf::Int64 frame_delta);
		void AddRigidBodyToGrid(RigidBody* rb);
		void MoveRigidBodyInGrid(RigidBody* rb);
		std::vector<RigidBody*> GetObjectsInGridLocation(int grid_x, int grid_y);
		void ScreenShake(float magnitude);
		void SetCurrentCheckPoint(Checkpoint* cp);
		void EndTheGame();
		bool DidThePlayerBeatTheGame();
		bool IsPlayerInCombat();
		void BuildDevLevel();
		void BuildTestLevel();
		bool IsObjectInUpdateRange(RigidBody* rb);
		bool IsNewGame();
		void StartNewGame();
};