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
		Checkpoint* current_checkpoint;
		sf::Font ringbearer_font = sf::Font();
		sf::Text continue_text;
		sf::Text lives_counter_text;
		int game_over_screen_sprite_transparency;
		sf::Texture game_over_texture;
		sf::Sprite game_over_sprite;
		sf::Texture blank_screen_texture;
		sf::Sprite blank_screen_sprite;
		sf::Texture hit_point_texture;
		sf::Texture parallax_background_texture;
		sf::Sprite parallax_background_sprite;
		std::vector<sf::Sprite> hit_point_sprites = std::vector<sf::Sprite>();
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
		Charger* charger;
		int grid_width = 100;
		int grid_height = 100;
		int grid_depth = 10;
		int cell_width = 100;
		int cell_height = 100;
		std::vector<std::vector<std::vector<RigidBody*>>> Grid = std::vector<std::vector<std::vector<RigidBody*>>>(grid_width, std::vector<std::vector<RigidBody*>>(grid_height, std::vector<RigidBody*>()));
	public:
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
};