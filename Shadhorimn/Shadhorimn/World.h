using namespace std;
#include <SFML/Graphics.hpp>
#include "RigidBody.h"
#include "PlayerCharacter.h"

class World {
	private:
		int grid_width = 50;
		int grid_height = 50;
		int grid_depth = 10;
		int cell_width = 100;
		int cell_height = 100;
		std::vector<std::vector<std::vector<RigidBody*>>> Grid = std::vector<std::vector<std::vector<RigidBody*>>>(grid_width, std::vector<std::vector<RigidBody*>>(grid_height, std::vector<RigidBody*>()));
	public:
		PlayerCharacter* main_character;
		World();
		void AddRigidBodyToGrid(RigidBody* rb);
		void MoveRigidBodyInGrid(RigidBody* rb);
		std::vector<RigidBody*> GetObjectsInGridLocation(int grid_x, int grid_y);
};