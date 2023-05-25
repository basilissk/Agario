#pragma once
#include "Enemy.h"
using namespace sf;

void Enemy::Pos(float X, float Y) {
	x = X;
	y = Y;
	enemyBody.setPosition(x - size, y - size);
}

void Enemy::Move(std::vector<Player*>& players, Enemy B, Food Arr[]) {

	float xB = B.GetPlayerCoordX();
	float yB = B.GetPlayerCoordY();
	float tmp;
	bool toPlayer = false;
	bool fromPlayer = false;
	const int distance = 10;

	speed = speedCoeff / sqrt(sqrt(size));

	for (Player* A : players) {
		if (A->life == true) {
			float xA = A->GetPlayerCoordX();
			float yA = A->GetPlayerCoordY();

			if (B.getPlayerSize() >= A->GetPlayerSize()) {
				if (abs(xA - xB) < distance*10 + size && abs(yA - yB) < distance*10 + size) {
					toPlayer = true;

					tmp = abs(xA - xB);
					if (abs(xA - xB + 1) < tmp) {
						if (xB - speed - size > 0) {
							xB -= speed;
							x = xB;
						}
					}
					tmp = abs(xA - xB);
					if (abs(xA - xB - 1) < tmp) {
						if (xB + speed + size < mapWidth) {
							xB += speed;
							x = xB;
						}
					}

					tmp = abs(yA - yB);
					if (abs(yA - yB + 1) < tmp) {
						if (yB - speed - size > 0) {
							yB -= speed;
							y = yB;
						}
					}
					tmp = abs(yA - yB);
					if (abs(yA - yB - 1) < tmp) {
						if (yB + speed + size < mapHeight) {
							yB += speed;
							y = yB;
						}
					}
				}
			}

			if (B.getPlayerSize() < A->GetPlayerSize()) {
				if (abs(xA - xB) < distance*2 + size && abs(yA - yB) < distance*2 + size) {
					fromPlayer = true;
					tmp = abs(xA - xB);
					if (abs(xA - xB + 1) > tmp) {
						if (xB - speed - size > 0) {
							xB -= speed;
							x = xB;
						}
					}

					tmp = abs(xA - xB);
					if (abs(xA - xB - 1) > tmp) {
						if (xB + speed + size < mapWidth) {
							xB += speed;
							x = xB;
						}
					}

					tmp = abs(yA - yB);
					if (abs(yA - yB + 1) > tmp) {
						if (yB - speed - size > 0) {
							yB -= speed;
							y = yB;
						}
					}
					tmp = abs(yA - yB);
					if (abs(yA - yB - 1) > tmp) {
						if (yB + speed + size < mapHeight) {
							yB += speed;
							y = yB;
						}
					}
				}
			}
		}
	}

	if (!toPlayer && !fromPlayer) {
		int index = 0;
		int j = 0;
		tmp = 0;

		while (Arr[j].life == false)
			j++;
		float temp = getDir(xB, yB, Arr[j].x, Arr[j].y);

		for (int i = 0; i <= foodAmount; i++) {
			if (getDir(xB, yB, Arr[i].x, Arr[i].y) < temp && Arr[i].life == true) {
				temp = getDir(xB, yB, Arr[i].x, Arr[i].y);
				index = i;
			}
		}

		tmp = index;

		tmp = abs(Arr[index].x - xB);
		if (abs(Arr[index].x - xB + 1) < tmp) {
			if (x - speed - size > 0) {
				xB -= speed;
				x = xB;
			}
		}
		tmp = abs(Arr[index].x - xB);
		if (abs(Arr[index].x - xB - 1) < tmp) {
			if (x + speed + size < mapWidth) {
				xB += speed;
				x = xB;
			}
		}

		tmp = abs(Arr[index].y - yB);
		if (abs(Arr[index].y - yB + 1) < tmp) {
			if (y - speed - size > 0) {
				yB -= speed;
				y = yB;
			}
		}
		tmp = abs(Arr[index].y - yB);
		if (abs(Arr[index].y - yB - 1) < tmp) {
			if (y + speed + size < mapHeight) {
				yB += speed;
				y = yB;
			}
		}
	}

	enemyBody.setPosition(x - size, y - size);
	enemyBody.setRadius(size);
}

void Enemy::SetEnemyBody()
{
	enemyBody.setPosition(x - size, y - size);
}


float Enemy::GetPlayerCoordX() {
	return x;
}

float Enemy::GetPlayerCoordY() {
	return y;
}

float Enemy::getPlayerSize() {
	return size;
}

CircleShape Enemy::getPlayerColor()
{
	return CircleShape();
}

void Enemy::eatingFood(Enemy p) {
	float xP = p.GetPlayerCoordX();
	float yP = p.GetPlayerCoordY();
	float sP = getPlayerSize();

	for (int i = 0; i < foodAmount; i++) {

		if ((((foodArr[i].x <= xP) && (foodArr[i].x >= xP - sP)) || ((foodArr[i].x >= xP) && (foodArr[i].x <= xP + sP))) &&
			(((foodArr[i].y <= yP) && (foodArr[i].y >= yP - sP)) || ((foodArr[i].y >= yP) && (foodArr[i].y <= yP + sP))) && foodArr[i].life == true)
		{
			foodArr[i].x = rand() % 900 + 50;
			foodArr[i].y = rand() % 900 + 50;
			foodArr[i].color = colorArray[rand() % 9];
			size += 0.2;
			enemyBody.setRadius(size);

		}
	}
}
int Enemy::getDir(float xE, float yE, float xF, float yF)
{
	return sqrt(pow(xF - xE, 2) + pow(yF - yE, 2));
}
Enemy enemyArr[enemyAmount];
