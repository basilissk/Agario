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

	void move();

	float getPlayerCoordX();

	float getPlayerCoordY();

	float getPlayerSize();

	void setPlayerSize(float size);

	void eatingFood(Player p);

};
