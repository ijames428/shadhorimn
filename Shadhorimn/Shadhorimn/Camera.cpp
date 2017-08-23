using namespace std;
#include <iostream>
#include "Camera.h"  

Camera::Camera(sf::Vector2f position, sf::Vector2f dimensions) {
	viewport_position = position;
	viewport_dimensions = dimensions;
}