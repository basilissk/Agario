#pragma once
#include "SFML/Graphics.hpp"
#include "Enemy.h"
#include "Player.h"
#include "Food.h"

using namespace sf;
using namespace std;

const int windowHeight = 1000;
const int windowWidth = 1500;
const int foodAmount = 300;
const int enemyAmount = 9;
const float startSize = 40;
const float mapWidth = 1000;
const float mapHeight = 1000;
const float speedCoeff = 0.85;

extern float speedP;
extern float speedE;
extern float zoom;
extern float lineWidth;
extern float drawingDistance;
extern bool WinOrLose;

extern RectangleShape background;
extern RectangleShape resultsBackground;
extern RectangleShape lines;
extern RectangleShape columns;
extern CircleShape foodCircle;
extern Font font;

extern Food foodArr[foodAmount];
extern Color colorArray[10];

extern RenderWindow window;
extern Event event;
extern Texture menuBackgroundT;



