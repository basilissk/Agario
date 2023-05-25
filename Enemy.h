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

	void Pos(float X, float Y);

	void Move(std::vector<Player*>& players, Enemy B, Food Arr[]);

	void SetEnemyBody();

	float GetPlayerCoordX();	

	float GetPlayerCoordY();

	float getPlayerSize();

	sf::CircleShape getPlayerColor();

	void eatingFood(Enemy p);

	int getDir(float xE, float yE, float xF, float yF);

};

extern Enemy enemyArr[];