#include "Player.h"

using namespace sf;

void Player::Move() {

	if (Keyboard::isKeyPressed(Keyboard::A)) {
		if (GetPlayerCoordX() - speed - size > 0) x -= speed;
	}
	if (Keyboard::isKeyPressed(Keyboard::D)) {
		if (GetPlayerCoordX() + speed + size < mapWidth) x += speed;
	}
	if (Keyboard::isKeyPressed(Keyboard::W)) {
		if (GetPlayerCoordY() - speed - size > 0) y -= speed;
	}
	if (Keyboard::isKeyPressed(Keyboard::S)) {
		if (GetPlayerCoordY() + speed + size < mapHeight) y += speed;

	}
	SetView(GetPlayerCoordX(), GetPlayerCoordY());
	playerBody.setPosition(x - size, y - size);
}

float Player::GetPlayerCoordX() {
	return x;
}

float Player::GetPlayerCoordY() {
	return y;
}

float Player::GetPlayerSize() {
	return size;
}

void Player::SetPlayerBody()
{
	playerBody.setPosition(x - size, y - size);
}

void Player::setPlayerSize(float size)
{
	playerBody.setRadius(size);
}

void Player::EatingFood() {
	float xP = GetPlayerCoordX();
	float yP = GetPlayerCoordY();
	float sP = GetPlayerSize();

	for (int i = 0; i < foodAmount; i++) {

		if ((((foodArr[i].x <= xP) && (foodArr[i].x >= xP - sP)) || ((foodArr[i].x >= xP) && (foodArr[i].x <= xP + sP))) &&
			(((foodArr[i].y <= yP) && (foodArr[i].y >= yP - sP)) || ((foodArr[i].y >= yP) && (foodArr[i].y <= yP + sP))) && foodArr[i].life == true)
		{
			foodArr[i].x = rand() % 900 + 50;
			foodArr[i].y = rand() % 900 + 50;
			foodArr[i].color = colorArray[rand() % 9];
			size += 0.15;
			playerBody.setRadius(size);	
		}
	}
}
