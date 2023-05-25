#pragma once
#include "SFML/Graphics.hpp"
#include "Globals.h"
#include "Player.h"
#include "Enemy.h"
#include "View.h"
#include "Server.h"
#include "Client.h"

using namespace sf;

void DrawingMap();

bool IsItVisible(Player p, float X, float Y);

void EatingEnemy(Player &p, Enemy &e);

void EatingEnemy(Player& p, Player& e);

void DrawingResults();

int DrawingMenu();

void DrawingRules(Sprite menuBackground);

void DrawingEnd(bool & is_the_end_of_program);

void CreateButton(Text &text, RectangleShape &button);

void ButtonNotSelected(Text &text, RectangleShape &button);

bool DrawingConnection(Server &server);