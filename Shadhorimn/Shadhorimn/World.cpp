using namespace std;
#include <iostream>
#include "World.h"

World::World() {
}

void World::Init(sf::RenderWindow* window, Camera* cam, PlayerCharacter* character) {
	render_window = window;
	camera = cam;
	main_character = character;

	platforms.push_back(new Platform(render_window, sf::Vector2f(540.0f, 500.0f), sf::Vector2f(200.0f, 1.0f)));
	platforms.push_back(new Platform(render_window, sf::Vector2f(0.0f, 600.0f), sf::Vector2f(1600.0f, 1.0f)));

	creatures.push_back(new Creature(render_window, sf::Vector2f(300.0f, 500.0f), sf::Vector2f(40.0f, 80.0f), true));
	creatures.push_back(new Creature(render_window, sf::Vector2f(200.0f, 500.0f), sf::Vector2f(40.0f, 80.0f), true));

	drones.push_back(new Drone(render_window, sf::Vector2f(600.0f, 450.0f), sf::Vector2f(30.0f, 30.0f), false));
	drones.push_back(new Drone(render_window, sf::Vector2f(200.0f, 300.0f), sf::Vector2f(30.0f, 30.0f), false));

	current_time = 0;
	screen_shaking = false;
	screen_shake_start_time = 0;
	screen_shake_duration = (sf::Int64)(0.01f * 1000000); // 1000000 is one second in microseconds.
}

void World::Update(sf::Int64 curr_time, sf::Int64 frame_delta) {
	render_window->clear();

	current_time = curr_time;

	sf::Vector2f screen_shake_amount = sf::Vector2f(0.0f, 0.0f);

	screen_shake_amount.x = screen_shake_magnitude * 5.0f * (rand() % 2 == 0 ? 1.0f : -1.0f);
	screen_shake_amount.y = screen_shake_magnitude * 5.0f * (rand() % 2 == 0 ? 1.0f : -1.0f);

	if (screen_shake_start_time + screen_shake_duration < current_time) {
		screen_shake_amount.x *= (rand() % 2 == 0 ? 1.0f : -1.0f);
		screen_shake_amount.y *= (rand() % 2 == 0 ? 1.0f : -1.0f);
	}

	if (screen_shake_start_time + screen_shake_duration * 2 < current_time) {
		screen_shake_magnitude = 0.0f;
	}

	float lerp = 0.00001f;
	sf::Vector2f position = camera->viewport_position;
	position.x += (main_character->x - camera->viewport_dimensions.x / 2.0f - position.x) * lerp * frame_delta;
	position.y += (main_character->y - camera->viewport_dimensions.y / 2.0f - position.y) * lerp * frame_delta;
	camera->viewport_position = sf::Vector2f(position.x, position.y);

	sf::Vector2f viewport_position_with_screen_shake = sf::Vector2f(camera->viewport_position.x + screen_shake_amount.x, camera->viewport_position.y + screen_shake_amount.y);

	main_character->Draw(viewport_position_with_screen_shake);
	for (int i = 0; i < (int)creatures.size(); i++) {
		creatures[i]->Draw(viewport_position_with_screen_shake);
	}
	for (int i = 0; i < (int)platforms.size(); i++) {
		platforms[i]->Draw(viewport_position_with_screen_shake);
	}
	for (int i = 0; i < (int)drones.size(); i++) {
		drones[i]->Draw(viewport_position_with_screen_shake);
	}

	main_character->Update(frame_delta);
	for (int i = 0; i < (int)creatures.size(); i++) {
		creatures[i]->Update(frame_delta);
	}
	for (int i = 0; i < (int)platforms.size(); i++) {
		platforms[i]->Update(frame_delta);
	}
	for (int i = 0; i < (int)drones.size(); i++) {
		drones[i]->Update(frame_delta);
		drones[i]->UpdateBehavior(current_time);
	}

	render_window->display();
}

void World::ScreenShake(float magnitude) {
	screen_shake_start_time = current_time;
	screen_shake_magnitude = magnitude;
}

void World::AddRigidBodyToGrid(RigidBody* rb) {
	int topLeftX = (int)rb->x / cell_width;
	int topLeftY = (int)rb->y / cell_height;
	int botRightX = (int)(rb->x + rb->width) / cell_width;
	int botRightY = (int)(rb->y + rb->height) / cell_height;

	rb->SetGridTopLeftX(topLeftX);
	rb->SetGridTopLeftY(topLeftY);
	rb->SetGridBotRightX(botRightX);
	rb->SetGridBotRightY(botRightY);

	for (int w = topLeftX; w <= botRightX; w++) {
		for (int h = topLeftY; h <= botRightY; h++) {
			bool found = false;

			for (int r = 0; r < (int)Grid[w][h].size(); r++) {
				if (Grid[w][h][r]->id == rb->id) {
					found = true;
				}
			}
			if (!found) {
				Grid[w][h].push_back(rb);
			}
		}
	}
}

void World::MoveRigidBodyInGrid(RigidBody* rb) {
	int topLeftX = (int)rb->x / cell_width;
	int topLeftY = (int)rb->y / cell_height;
	int botRightX = (int)(rb->x + rb->width) / cell_width;
	int botRightY = (int)(rb->y + rb->height) / cell_height;
	
	if (topLeftX != rb->grid_top_left_x || topLeftY != rb->grid_top_left_y || botRightX != rb->grid_bot_right_x || botRightY != rb->grid_bot_right_y) {
		for (int w = topLeftX; w <= botRightX; w++) {
			bool found = false;

			for (int h = topLeftY; h <= botRightY; h++) {
				for (int r = 0; r < (int)Grid[w][h].size(); r++) {
					if (Grid[w][h][r]->id == rb->id) {
						Grid[w][h].erase(Grid[w][h].begin() + r);
						found = true;
					}
				}
			}
		}

		AddRigidBodyToGrid(rb);
	}
}

std::vector<RigidBody*> World::GetObjectsInGridLocation(int grid_x, int grid_y) {
	return Grid[grid_x][grid_y];
}