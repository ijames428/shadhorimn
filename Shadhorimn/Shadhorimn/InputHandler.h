using namespace std;
#include <SFML/Graphics.hpp>
#include "PlayerCharacter.h"

class InputHandler {
	private:
		int a_button = 0;
		int b_button = 1;
		int x_button = 2;
		int y_button = 3;
		bool a_button_previous = false;
		bool b_button_previous = false;
		bool x_button_previous = false;
		bool y_button_previous = false;
		float deadzone = 20.0f;
		PlayerCharacter* player_character;
	public:
		InputHandler(PlayerCharacter* pc);
		void Update();
};