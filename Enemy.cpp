#pragma once
#include "Enemy.h"
using namespace sf;

void Enemy::pos(float X, float Y) {
	x = X;
	y = Y;
	pl_form.setPosition(x - size, y - size);
}

void Enemy::move(Player A, Enemy B, Food Arr[]) {

	float xA = A.getPlayerCoordX();
	float yA = A.getPlayerCoordY();
	float xB = B.getPlayerCoordX();
	float yB = B.getPlayerCoordY();
	float tmp;

	speed = 5 / size;
	//speed = 0.1;
	if (abs(xA - xB) < 120 + size && abs(yA - yB) < 120 + size) {

		if (B.getPlayerSize() > A.getPlayerSize()) {

			if (xA != xB) {
				tmp = abs(xA - xB);
				if (abs(xA - xB + 1) < tmp)
				{
					if (x - speed - size > 0)
					{
						xB -= speed;
						x = xB;
					}
				}
				tmp = abs(xA - xB);
				if (abs(xA - xB - 1) < tmp)
				{
					if (x + speed + size < mapWidth)
					{
						xB += speed;
						x = xB;
					}
				}
			}

			if (yA != yB)
			{
				tmp = abs(yA - yB);
				if (abs(yA - yB + 1) < tmp)
				{
					if (y - speed - size > 0)
					{
						yB -= speed;
						y = yB;
					}
				}
				tmp = abs(yA - yB);
				if (abs(yA - yB - 1) < tmp)
				{
					if (y + speed + size < mapHeight)
					{
						yB += speed;
						y = yB;
					}
				}
			}

		}

		if (B.getPlayerSize() < A.getPlayerSize())
		{
			tmp = abs(xA - xB);
			if (abs(xA - xB + 1) > tmp)
			{
				if (x - speed - size > 0)
				{
					xB -= speed;
					x = xB;
				}
			}
			tmp = abs(xA - xB);
			if (abs(xA - xB - 1) > tmp)
			{

				if (x + speed + size < mapWidth)
				{
					xB += speed;
					x = xB;
				}

			}

			if (abs(xA - xB) < 100 && abs(yA - yB) < 100)
			{
				tmp = abs(yA - yB);
				if (abs(yA - yB + 1) > tmp)
				{
					if (y - speed - size > 0)
					{
						yB -= speed;
						y = yB;
					}
				}
				tmp = abs(yA - yB);
				if (abs(yA - yB - 1) > tmp)
				{
					if (y + speed + size < mapHeight)
					{
						yB += speed;
						y = yB;
					}
				}
			}
		}
	}

	else {

		int index = 0;
		int j = 0;
		tmp = 0;

		while (Arr[j].life == false) j++;
		float temp = getDir(xB, yB, Arr[j].x, Arr[j].y);

		for (int i = 0; i <= foodAmount; i++)
		{
			if (getDir(xB, yB, Arr[i].x, Arr[i].y) < temp && Arr[i].life==true)
			{
				temp = getDir(xB, yB, Arr[i].x, Arr[i].y);
				index = i;
			}
		}

		temp = 0;

		tmp = index;

		tmp = abs(Arr[index].x - xB);
		if (abs(Arr[index].x - xB + 1) < tmp)
		{
			if (x - speed - size > 0)
			{
				xB -= speed;
				x = xB;
				
			}
		}
		tmp = abs(Arr[index].x - xB);
		if (abs(Arr[index].x - xB - 1) < tmp)

		{
			if (x + speed + size < mapWidth)
			{
				xB += speed;
				x = xB;
				
			}
		}

		tmp = abs(Arr[index].y - yB);
		if (abs(Arr[index].y - yB + 1) < tmp)
		{
			if (y - speed - size > 0)
			{
				yB -= speed;
				y = yB;
				
			}
		}
		tmp = abs(Arr[index].y - yB);
		if (abs(Arr[index].y - yB - 1) < tmp)
		{
			if (y + speed + size < mapHeight)
			{
				yB += speed;
				y = yB;

			}
		}
	}


	pl_form.setPosition(x - size, y - size);
	pl_form.setRadius(size);
}

float Enemy::getPlayerCoordX() {
	return x;
}

float Enemy::getPlayerCoordY() {
	return y;
}

float Enemy::getPlayerSize() {
	return size;
}

void Enemy::eatingFood(Enemy p) {
	float xP = p.getPlayerCoordX();
	float yP = p.getPlayerCoordY();
	float sP = getPlayerSize();

	for (int i = 0; i < foodAmount; i++) {

		if ((((foodArr[i].x <= xP) && (foodArr[i].x >= xP - sP)) || ((foodArr[i].x >= xP) && (foodArr[i].x <= xP + sP))) &&
			(((foodArr[i].y <= yP) && (foodArr[i].y >= yP - sP)) || ((foodArr[i].y >= yP) && (foodArr[i].y <= yP + sP))) && foodArr[i].life == true)
		{
			foodArr[i].x = rand() % 900 + 50;
			foodArr[i].y = rand() % 900 + 50;
			foodArr[i].color = colorArray[rand() % 9];
			size += 0.2;
			pl_form.setRadius(size);

		}
	}
}
int Enemy::getDir(float xE, float yE, float xF, float yF)
{
	return sqrt(pow(xF - xE, 2) + pow(yF - yE, 2));
}
Enemy enemyArr[enemyAmount];
