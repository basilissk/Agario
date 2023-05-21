#include "Library.h"

using namespace sf;
using namespace std;

int main() {

	Text text("", font);
	window.setFramerateLimit(100);
	srand(time(NULL));
	setlocale(LC_ALL, "Russian");
	font.loadFromFile("resources\\impact2.ttf");

	bool isThePlayerDied = false;
	bool isAllEnemiesDied = false;
	bool isTheEndOfPlay = false;
	bool isTheEndOfProgram = false;

	bool is_all_players_died = false;

	int mode;
	int sock;

	Server server;

	struct GameState
	{
		Enemy enemies_arr[9];
		Food food_arr[300];
	};

	UINT32 colorInteger;
	Color colorTemp;

	Packet playerCoordinatePacket;

	vector<Player*> players;

	while (window.isOpen()) {

		while (!isTheEndOfProgram) {

			mode = drawingMenu();
			if (mode == 1) break;
			if (mode == 0) {
				//if ((sock = DrawingConnection()) == NULL) continue;


				isTheEndOfPlay = false;

				while (!isTheEndOfPlay) {

					isThePlayerDied = false;
					isAllEnemiesDied = false;

					view.reset(FloatRect(0, 0, windowWidth / 4, windowHeight / 4));
					drawingMap();

					zoom = 0;
					lineWidth = 0.2;
					view.setSize(windowWidth / 1, windowHeight / 1);
					drawingDistance = 200;

					lines.setFillColor(Color(192, 192, 192));
					columns.setFillColor(Color(192, 192, 192));

					server.StartServer();
					server.WaitingClients();

					for (Client* client : server.clients) {
						Player* player = new Player;
						Packet PlayerPacket;
						PlayerPacket = client->getClientPacket();
						PlayerPacket >> player->x >> player->y >> player->size >> player->speed >> player->life >> colorInteger;
						player->playerBody.setRadius(player->size);
						player->playerBody.setFillColor(Color(colorInteger));
						players.push_back(player);
					}

					Player player;
					player.x = rand() % 900 + 50;
					player.y = rand() % 900 + 50;
					player.size = 10;
					player.playerBody.setRadius(player.size);
					player.speed = speedP;
					player.playerBody.setFillColor(colorArray[rand() % 9]);
					player.life = true;

					players.push_back(&player);

					for (int i = 0; i < enemyAmount; i++) {
						enemyArr[i].x = rand() % 900 + 50;
						enemyArr[i].y = rand() % 900 + 50;
						enemyArr[i].size = rand() % 1 + 5;
						enemyArr[i].speed = speedCoeff / sqrt(sqrt(enemyArr[i].size));
						enemyArr[i].enemyBody.setRadius(enemyArr[i].size);
						enemyArr[i].enemyBody.setFillColor(colorArray[rand() % 9]);
						enemyArr[i].enemyBody.setPosition(Vector2f(enemyArr[i].x, enemyArr[i].y));
						enemyArr[i].life = true;
					}

					for (int i = 0; i < foodAmount; i++) {
						foodArr[i].life = true;
						foodArr[i].x = rand() % 900 + 50;
						foodArr[i].y = rand() % 900 + 50;
						foodArr[i].color = colorArray[rand() % 9];

					}

					Packet enemiesPacket;
					enemiesPacket << enemyAmount;

					for (int i = 0; i < enemyAmount; i++) {
						enemiesPacket << enemyArr[i].x;
						enemiesPacket << enemyArr[i].y;
						enemiesPacket << enemyArr[i].size;
						enemiesPacket << enemyArr[i].speed;
						enemiesPacket << enemyArr[i].life;

						colorTemp = enemyArr[i].enemyBody.getFillColor();
						enemiesPacket << colorTemp.toInteger();

					}

					server.SendPacketToAllClients(enemiesPacket);

					Packet foodPacket;
					foodPacket << foodAmount;

					for (int i = 0; i < foodAmount; i++) {
						foodPacket << foodArr[i].x;
						foodPacket << foodArr[i].y;
						foodPacket << foodArr[i].life;

						colorTemp = foodArr[i].color;
						foodPacket << colorTemp.toInteger();
					}

					server.SendPacketToAllClients(foodPacket);


					while (!isThePlayerDied) {

						isAllEnemiesDied = false;

						while (window.pollEvent(event)) {
							if (event.type == Event::Closed)
								window.close();
						}

						window.setView(view);
						if (player.size > 40) {
							view.setSize(windowWidth / 2, windowHeight / 2);
							zoom = 8;
							lineWidth = 0.4;
							drawingDistance = 380;

						}

						player.move();
						/*for (Player* player : players) {
							player->move();
						}*/

						playerCoordinatePacket.clear();

						for (Client* client : server.clients) {
							if (players[client->GetId()]->life == true) {
								playerCoordinatePacket = client->getClientPacket();
								playerCoordinatePacket >> players[client->GetId()]->x;
								playerCoordinatePacket >> players[client->GetId()]->y;
								players[client->GetId()]->SetPlayerBody();
							}
						}
						
						//players[0]->move();

						window.clear(Color(153, 153, 255));
						window.draw(background);

						for (int i = 0; i < enemyAmount; i++) {
							if (enemyArr[i].life == true) {
								enemyArr[i].move(players, enemyArr[i], foodArr);
							}
						}

						for (int i = 0; i < enemyAmount; i++) {
							for (Player* player : players)
								if (player->life == true)
									eatingEnemy(*player, enemyArr[i]);

						}

						for (Player* player : players)
							if (player->life == true)
							player->eatingFood();

						for (int i = 0; i < enemyAmount; i++) {
							if (enemyArr[i].life == true)
								enemyArr[i].eatingFood(enemyArr[i]);
						}

						for (int i = 0; i < mapHeight; i += 10 + zoom) {
							if (isItVisible(player, player.getPlayerCoordX() + 130, i)) {
								window.draw(lines);
								lines.setPosition(0, i);
								lines.setSize(Vector2f(mapWidth, lineWidth));
							}
						}

						for (int i = 0; i < mapWidth; i += 10 + zoom) {
							if (isItVisible(player, i, player.getPlayerCoordY() + 130)) {
								window.draw(columns);
								columns.setPosition(i, 0);
								columns.setSize(Vector2f(lineWidth, mapHeight));
							}
						}

						for (int i = 0; i < foodAmount; i++) {
							if (isItVisible(player, foodArr[i].x, foodArr[i].y)) {
								if (foodArr[i].life == true) {
									foodCircle.setPosition(foodArr[i].x - 2, foodArr[i].y - 2);
									foodCircle.setRadius(2);
									foodCircle.setFillColor(foodArr[i].color);
									window.draw(foodCircle);
								}
							}
						}

						for (int i = 0; i < enemyAmount; i++) {
							if (enemyArr[i].life == true) {
								window.draw(enemyArr[i].enemyBody);
							}
						}


						for (Player* player : players) {
							if (player->life == true)
								window.draw(player->playerBody);
						}

						window.display();


						//////////////////////////////////////////////////////////////////////


						enemiesPacket.clear();
						enemiesPacket << enemyAmount;

						for (int i = 0; i < enemyAmount; i++) {
							enemiesPacket << enemyArr[i].x;
							enemiesPacket << enemyArr[i].y;
							enemiesPacket << enemyArr[i].size;
							enemiesPacket << enemyArr[i].speed;
							enemiesPacket << enemyArr[i].life;

							colorTemp = enemyArr[i].enemyBody.getFillColor();
							enemiesPacket << colorTemp.toInteger();

						}

						foodPacket.clear();
						foodPacket << foodAmount;

						for (int i = 0; i < foodAmount; i++) {
							foodPacket << foodArr[i].x;
							foodPacket << foodArr[i].y;
							foodPacket << foodArr[i].life;

							colorTemp = foodArr[i].color;
							foodPacket << colorTemp.toInteger();
						}

						server.SendPacketToAllClients(enemiesPacket);
						server.SendPacketToAllClients(foodPacket);



						if (!player.life) isThePlayerDied = true;

						if (!enemyArr[0].life && !enemyArr[1].life && !enemyArr[2].life && !enemyArr[3].life && !enemyArr[4].life &&
							!enemyArr[5].life && !enemyArr[6].life && !enemyArr[7].life && !enemyArr[8].life)
						{
							isAllEnemiesDied = true;
							isThePlayerDied = true;
						}


						if (isThePlayerDied && !isAllEnemiesDied)
						{
							drawingWinOrLose(isTheEndOfPlay, isThePlayerDied, isTheEndOfProgram, player.life);
						}

						if (isThePlayerDied && isAllEnemiesDied)
						{
							drawingWinOrLose(isTheEndOfPlay, isThePlayerDied, isTheEndOfProgram, player.life);
						}
						if (window.isOpen() == false) return 0;
					}
				}
			}

			window.close();
		}

		return 0;
	}
}
