using namespace std;
#include <iostream>
#include "World.h"

World::World() {
}

void World::Init(sf::RenderWindow* window, Camera* cam, PlayerCharacter* character) {
	Grid = std::vector<std::vector<std::vector<RigidBody*>>>(grid_width, std::vector<std::vector<RigidBody*>>(grid_height, std::vector<RigidBody*>()));

	render_window = window;
	camera = cam;
	main_character = character;
	paused = false;
	game_over_screen_sprite_transparency = 0;
	player_is_in_combat = false;

	end_of_the_game_trigger = new EndOfTheGame(render_window, sf::Vector2f(1560.0f, 590.0f), sf::Vector2f(40.0f, 10.0f), false);

	if (current_number_of_lives == starting_number_of_lives) {
		current_checkpoint = new Checkpoint(render_window, sf::Vector2f(500.0f, 590.0f), sf::Vector2f(40.0f, 10.0f), false);
	}

	main_character->x = current_checkpoint->x;
	main_character->y = current_checkpoint->y + current_checkpoint->height - main_character->height;
	main_character->hit_points = main_character->max_hit_points;
	main_character->velocity = sf::Vector2f(0.0f, 0.0f);

	checkpoints.erase(checkpoints.begin(), checkpoints.end());
	checkpoints.push_back(new Checkpoint(render_window, sf::Vector2f(2.0f, 590.0f), sf::Vector2f(40.0f, 10.0f), false));
	checkpoints.push_back(new Checkpoint(render_window, sf::Vector2f(620.0f, 490.0f), sf::Vector2f(40.0f, 10.0f), false));

	platforms.erase(platforms.begin(), platforms.end());
	platforms.push_back(new Platform(render_window, sf::Vector2f(540.0f, 500.0f), sf::Vector2f(200.0f, 1.0f)));
	platforms.push_back(new Platform(render_window, sf::Vector2f(0.0f, 0.0f), sf::Vector2f(1.0f, 600.0f)));
	platforms.push_back(new Platform(render_window, sf::Vector2f(1599.0f, 0.0f), sf::Vector2f(1.0f, 600.0f)));
	platforms.push_back(new Platform(render_window, sf::Vector2f(0.0f, 600.0f), sf::Vector2f(1600.0f, 10.0f)));
	platforms.push_back(new Platform(render_window, sf::Vector2f(0.0f, 0.0f), sf::Vector2f(1600.0f, 1.0f)));
	//platforms.push_back(new Platform(render_window, sf::Vector2f(800.0f, 500.0f), sf::Vector2f(10.0f, 100.0f)));

	creatures.erase(creatures.begin(), creatures.end());
	creatures.push_back(new Creature(render_window, sf::Vector2f(300.0f, 500.0f), sf::Vector2f(40.0f, 80.0f), true));
	creatures.push_back(new Creature(render_window, sf::Vector2f(200.0f, 500.0f), sf::Vector2f(40.0f, 80.0f), true));

	drones.erase(drones.begin(), drones.end());
	drones.push_back(new Drone(render_window, sf::Vector2f(1000.0f, 450.0f), sf::Vector2f(30.0f, 30.0f), false));
	drones.push_back(new Drone(render_window, sf::Vector2f(1100.0f, 300.0f), sf::Vector2f(30.0f, 30.0f), false));

	current_time = 0;
	screen_shaking = false;
	screen_shake_start_time = 0;
	screen_shake_duration = (sf::Int64)(0.01f * 1000); // 1000 is one second in milliseconds.

	hit_point_texture.loadFromFile("Images/HitPoint.png");
	for (int i = 0; i < main_character->hit_points; i++) {
		hit_point_sprites.push_back(sf::Sprite(hit_point_texture));
	}

	blank_screen_texture.loadFromFile("Images/StartMenuBackground.png");
	blank_screen_sprite = sf::Sprite(blank_screen_texture);

	game_over_texture.loadFromFile("Images/GameOverScreen.png");
	game_over_sprite = sf::Sprite(game_over_texture);

	parallax_background_texture.loadFromFile("Images/parallax_background.jpg");
	parallax_background_sprite = sf::Sprite(parallax_background_texture);
	parallax_background_sprite.setPosition(0.0f, 0.0f);

	if (!ringbearer_font.loadFromFile("Images/RingbearerFont.ttf"))
		return;
	
	lives_counter_text.setFont(ringbearer_font);
	lives_counter_text.setString("Lives: " + std::to_string(current_number_of_lives));
	lives_counter_text.setPosition(10.0f, 40.0f);

	continue_text.setFont(ringbearer_font);
	continue_text.setString("You have " + std::to_string(current_number_of_lives) + " lives left.\nPress Start to continue.");
	continue_text.setPosition(camera->viewport_dimensions.x / 2.0f - 150.0f, camera->viewport_dimensions.y / 2.0f - 100.0f);
}

void World::Update(sf::Int64 curr_time, sf::Int64 frame_delta) {
	if (!paused)
	{
		render_window->clear();

		if (main_character->hit_points > 0) {
			current_time = curr_time;

			if (frame_delta > 250) {
				frame_delta = 50;
			}

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

			float lerp = 0.01f;
			sf::Vector2f position = camera->viewport_position;
			position.x += (main_character->x - camera->viewport_dimensions.x / 2.0f - position.x) * lerp * frame_delta;
			position.y += (main_character->y - camera->viewport_dimensions.y / 2.0f - position.y) * lerp * frame_delta;
			camera->viewport_position = sf::Vector2f(position.x, position.y);

			sf::Vector2f viewport_position_with_screen_shake = sf::Vector2f(camera->viewport_position.x + screen_shake_amount.x, camera->viewport_position.y + screen_shake_amount.y);

			sf::Vector2f parallax_background_viewport_position = sf::Vector2f(-(viewport_position_with_screen_shake.x / 10.0f), -(viewport_position_with_screen_shake.y / 10.0f));
			parallax_background_sprite.setPosition(parallax_background_viewport_position);
			render_window->draw(parallax_background_sprite);

			main_character->Draw(viewport_position_with_screen_shake);
			main_character->test_projectile->Draw(viewport_position_with_screen_shake);
			end_of_the_game_trigger->Draw(viewport_position_with_screen_shake);
			for (int i = 0; i < (int)checkpoints.size(); i++) {
				checkpoints[i]->Draw(viewport_position_with_screen_shake);
			}
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
			main_character->test_projectile->Update(frame_delta);
			main_character->test_projectile->UpdateProjectile(current_time);
			end_of_the_game_trigger->Update(frame_delta);
			end_of_the_game_trigger->UpdateEndOfTheGame();
			for (int i = 0; i < (int)checkpoints.size(); i++) {
				checkpoints[i]->Update(frame_delta);
				checkpoints[i]->UpdateCheckPoint();
			}
			for (int i = 0; i < (int)creatures.size(); i++) {
				creatures[i]->Update(frame_delta);
			}
			for (int i = 0; i < (int)platforms.size(); i++) {
				platforms[i]->Update(frame_delta);
			}

			player_is_in_combat = false;

			for (int i = 0; i < (int)drones.size(); i++) {
				drones[i]->Update(frame_delta);
				drones[i]->UpdateBehavior(current_time);
				drones[i]->UpdateProjectiles(current_time, frame_delta);

				if (drones[i]->hit_points > 0 && RigidBody::GetDistanceBetweenTwoPoints(sf::Vector2f(main_character->x, main_character->y), sf::Vector2f(drones[i]->x, drones[i]->y)) < 600.0f) {
					player_is_in_combat = true;
				}
			}

			for (int i = 0; i < main_character->hit_points; i++) {
				hit_point_sprites[i].setPosition(sf::Vector2f(10.0f + 27.0f * (float)i, 10.0f));
				render_window->draw(hit_point_sprites[i]);
			}

			lives_counter_text.setString("Lives: " + std::to_string(current_number_of_lives));
			render_window->draw(lives_counter_text);
		} else {
			lives_counter_text.setString(std::to_string(current_number_of_lives));

			if (game_over_screen_sprite_transparency >= 255) {
				game_over_screen_sprite_transparency = 255;
			} else {
				game_over_screen_sprite_transparency += 1;
			}

			if (current_number_of_lives > 0) {
				blank_screen_sprite.setColor(sf::Color(255, 255, 255, game_over_screen_sprite_transparency));
				render_window->draw(blank_screen_sprite);

				continue_text.setFillColor(sf::Color(255, 255, 255, game_over_screen_sprite_transparency));
				continue_text.setOutlineColor(sf::Color(255, 255, 255, game_over_screen_sprite_transparency));
				render_window->draw(continue_text);
			} else {
				game_over_sprite.setColor(sf::Color(255, 255, 255, game_over_screen_sprite_transparency));

				render_window->draw(game_over_sprite);
			}
		}

		render_window->display();
	}
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

	if (topLeftX > 0) {
		topLeftX--;
	}
	if (topLeftY > 0) {
		topLeftY--;
	}
	if (botRightX < grid_width) {
		botRightX++;
	}
	if (botRightY < grid_height) {
		botRightY++;
	}

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

	if (topLeftX < 0 || topLeftY < 0 || botRightX < 0 || botRightY < 0 ||
		topLeftX >= (int)Grid.size() || topLeftY >= (int)Grid.size() || botRightX >= (int)Grid.size() || botRightY >= (int)Grid.size()) {
		return;
	}
	
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

void World::SetCurrentCheckPoint(Checkpoint* cp) {
	current_checkpoint = cp;
}

void World::EndTheGame() {
	player_beat_the_game = true;
}

bool World::DidThePlayerBeatTheGame() {
	return player_beat_the_game;
}

bool World::IsPlayerInCombat() {
	return player_is_in_combat;
}