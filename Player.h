#pragma once
#include "SFML/Graphics.hpp"
#include "View.h"
#include "Globals.h"

class Player {
public:
	float x;
	float y;
	float size;
	float speed;
	bool life;
	sf::CircleShape playerBody;

	void Move();

	float GetPlayerCoordX();

	float GetPlayerCoordY();

	float GetPlayerSize();

	void SetPlayerBody();

	void setPlayerSize(float size);

	void EatingFood();

};
