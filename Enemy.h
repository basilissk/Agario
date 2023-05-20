#pragma once
#include "SFML/Graphics.hpp"
#include "Globals.h"
#include "Player.h"
#include "Food.h"

class Player;

class Enemy {
public:
	float x;
	float y;
	float size;
	float speed;
	bool life;
	sf::CircleShape enemyBody;

	void pos(float X, float Y);

	void move(Player A, Enemy B, Food Arr[]);

	float getPlayerCoordX();

	float getPlayerCoordY();

	float getPlayerSize();

	sf::CircleShape getPlayerColor();

	void eatingFood(Enemy p);

	int getDir(float xE, float yE, float xF, float yF);

};

extern Enemy enemyArr[];