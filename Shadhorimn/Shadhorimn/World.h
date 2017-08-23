using namespace std;
#include <SFML/Graphics.hpp>
#include "RigidBody.h"
#include "PlayerCharacter.h"
#include "Platform.h"
#include "Drone.h"
#include "Camera.h"

class World {
	private:
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
		PlayerCharacter* main_character;
		World();
		void Init(sf::RenderWindow* window, Camera* cam, PlayerCharacter* character);
		void Update(sf::Int64 frame_delta);
		void AddRigidBodyToGrid(RigidBody* rb);
		void MoveRigidBodyInGrid(RigidBody* rb);
		std::vector<RigidBody*> GetObjectsInGridLocation(int grid_x, int grid_y);
};