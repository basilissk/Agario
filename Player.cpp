#include "Player.h"

using namespace sf;

void Player::move() {

	if (Keyboard::isKeyPressed(Keyboard::A)) {
		if (getPlayerCoordX() - speed - size > 0) x -= speed;
	}
	if (Keyboard::isKeyPressed(Keyboard::D)) {
		if (getPlayerCoordX() + speed + size < mapWidth) x += speed;
	}
	if (Keyboard::isKeyPressed(Keyboard::W)) {
		if (getPlayerCoordY() - speed - size > 0) y -= speed;
	}
	if (Keyboard::isKeyPressed(Keyboard::S)) {
		if (getPlayerCoordY() + speed + size < mapHeight) y += speed;

	}
	getPlayerCoordForView(getPlayerCoordX(), getPlayerCoordY());
	playerBody.setPosition(x - size, y - size);
}

float Player::getPlayerCoordX() {
	return x;
}

float Player::getPlayerCoordY() {
	return y;
}

float Player::getPlayerSize() {
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

void Player::eatingFood() {
	float xP = getPlayerCoordX();
	float yP = getPlayerCoordY();
	float sP = getPlayerSize();

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
