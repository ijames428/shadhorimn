using namespace std;
#include <SFML/Graphics.hpp>
#include "RigidBody.h"
#include "PlayerCharacter.h"
#include "Platform.h"
#include "Drone.h"
#include "Camera.h"

class World {
	private:
		sf::Font ringbearer_font = sf::Font();
		sf::Text lives_counter_text;
		int game_over_screen_sprite_transparency;
		sf::Texture game_over_texture;
		sf::Sprite game_over_sprite;
		sf::Texture hit_point_texture;
		std::vector<sf::Sprite> hit_point_sprites = std::vector<sf::Sprite>();
		sf::Int64 current_time;
		bool screen_shaking;
		sf::Int64 screen_shake_start_time;
		sf::Int64 screen_shake_duration;
		float screen_shake_magnitude;
		Camera* camera;
		sf::RenderWindow* render_window;
		std::vector<Platform*> platforms = std::vector<Platform*>();
		std::vector<Creature*> creatures = std::vector<Creature*>();
		std::vector<Drone*> drones = std::vector<Drone*>();
		int grid_width = 50;
		int grid_height = 50;
		int grid_depth = 10;
		int cell_width = 100;
		int cell_height = 100;
		std::vector<std::vector<std::vector<RigidBody*>>> Grid = std::vector<std::vector<std::vector<RigidBody*>>>(grid_width, std::vector<std::vector<RigidBody*>>(grid_height, std::vector<RigidBody*>()));
	public:
		int number_of_lives = 2;
		bool paused;
		PlayerCharacter* main_character;
		World();
		void Init(sf::RenderWindow* window, Camera* cam, PlayerCharacter* character);
		void Update(sf::Int64 current_time, sf::Int64 frame_delta);
		void AddRigidBodyToGrid(RigidBody* rb);
		void MoveRigidBodyInGrid(RigidBody* rb);
		std::vector<RigidBody*> GetObjectsInGridLocation(int grid_x, int grid_y);
		void ScreenShake(float magnitude);
};