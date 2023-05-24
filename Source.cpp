#include "Library.h"

using namespace sf;
using namespace std;

int main() {

	Text text("", font);
	window.setFramerateLimit(100);
	srand(time(NULL));
	setlocale(LC_ALL, "Russian");
	font.loadFromFile("resources\\impact2.ttf");


	bool isAllPlayersDied = false;
	bool isTheEndOfPlay = false;
	bool isTheEndOfProgram = false;

	int mode;
	int sock;

	struct GameState
	{
		Enemy enemies_arr[9];
		Food food_arr[300];
	};

	Uint32 colorInteger;
	Color colorTemp;

	Packet playerPacket;
	Packet playerCoordinatePacket;

	

	while (window.isOpen()) {

		while (!isTheEndOfProgram) {

			mode = drawingMenu();
			if (mode == 1) break;
			if (mode == 0) {
				//if ((sock = DrawingConnection()) == NULL) continue;
				
				isAllPlayersDied = false;

				view.reset(FloatRect(0, 0, windowWidth / 4, windowHeight / 4));
				drawingMap();

				zoom = 0;
				lineWidth = 0.2;
				view.setSize(windowWidth / 1, windowHeight / 1);
				drawingDistance = 200;

				lines.setFillColor(Color(192, 192, 192));
				columns.setFillColor(Color(192, 192, 192));

				vector<Player*> players;
				Server server;
				server.StartServer();
				server.WaitingClients();

				for (Client* client : server.clients) {
					Player* player = new Player;
					playerPacket.clear();
					playerPacket = client->GetClientPacket();
					playerPacket >> player->x >> player->y >> player->size >> player->speed >> player->life >> colorInteger;
					player->playerBody.setRadius(player->size);
					player->playerBody.setFillColor(Color(colorInteger));
					players.push_back(player);
				}

				Packet amountOfPlayers;

				int size = players.size();
				amountOfPlayers << size;
				server.SendPacketToAllClients(amountOfPlayers);

				playerPacket.clear();
				for (Client* client : server.clients) {
					for (int i = 0; i < players.size(); i++) {
						if (client->GetId() != i) {
							colorTemp = players[i]->playerBody.getFillColor();
							playerPacket << players[i]->x << players[i]->y << players[i]->size
								<< players[i]->speed << players[i]->life << colorTemp.toInteger();
							server.SendPacketToClient(playerPacket, client);
							playerPacket.clear();
						}
					}
				}


				Player player;
				player.x = rand() % 900 + 50;
				player.y = rand() % 900 + 50;
				player.size = 10;
				player.playerBody.setRadius(player.size);
				player.speed = speedP;
				player.playerBody.setFillColor(colorArray[rand() % 9]);
				player.life = true;


				for (int i = 0; i < enemyAmount; i++) {
					enemyArr[i].x = rand() % 900 + 50;
					enemyArr[i].y = rand() % 900 + 50;
					enemyArr[i].size = rand() % 5 + 5;
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


				while (!isAllPlayersDied) {

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

					playerCoordinatePacket.clear();

					for (Client* client : server.clients) {
						try {
							if (players[client->GetId()]->life == true) {
								playerCoordinatePacket = client->GetClientPacket();
								playerCoordinatePacket >> players[client->GetId()]->x;
								playerCoordinatePacket >> players[client->GetId()]->y;
								players[client->GetId()]->SetPlayerBody();
							}
						}
						catch (exception ex) {
							server.RemoveClient(client->GetId());
							continue;
						}
					}

					window.clear(Color(153, 153, 255));
					window.draw(background);

					for (int i = 0; i < enemyAmount; i++) {
						if (enemyArr[i].life == true) {
							enemyArr[i].move(players, enemyArr[i], foodArr);
						}
					}

					for (int i = 0; i < enemyAmount; i++) {
						for (Player* player : players)
							if (player->life == true) {
								eatingEnemy(*player, enemyArr[i]);
								for (Player* player2 : players) {
									if (player2 != player) {
										eatingEnemy(*player, *player2);
									}
								}
							}

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

					playerPacket.clear();
					for (Client* client : server.clients) {
						for (int i = 0; i < players.size(); i++) {
							if (client->GetId() == i) {
								playerPacket << players[i]->life << players[i]->size;
								server.SendPacketToClient(playerPacket, client);
								playerPacket.clear();
							}
						}
					}

					playerPacket.clear();

					for (Client* client : server.clients) {
						for (int i = 0; i < players.size(); i++) {
							if (client->GetId() != i) {
								playerPacket << players[i]->x << players[i]->y << players[i]->size
									<< players[i]->speed << players[i]->life;
								server.SendPacketToClient(playerPacket, client);
								playerPacket.clear();
							}
						}
					}

					enemiesPacket.clear();
					//enemiesPacket << enemyAmount;

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

					foodPacket.clear();
					//foodPacket << foodAmount;

					for (int i = 0; i < foodAmount; i++) {
						foodPacket << foodArr[i].x;
						foodPacket << foodArr[i].y;
						foodPacket << foodArr[i].life;

						colorTemp = foodArr[i].color;
						foodPacket << colorTemp.toInteger();
					}

					server.SendPacketToAllClients(foodPacket);

					for (Client* client : server.clients) {
						for (int i = 0; i < players.size(); i++) {
							if (client->GetId() == i) {
								if (players[i]->life == false) {
									server.RemoveClient(i);
								}
							}
						}
					}

					for (Player* player : players) {
						if (player->life) {
							isAllPlayersDied = false;
							break;
						}
						isAllPlayersDied = true;
					}

					if (isAllPlayersDied) {
						drawingWinOrLose(isTheEndOfProgram);
						server.ShutsownServer();
					}

					if (window.isOpen() == false) {
						server.ShutsownServer();
						return 0;
					}
				}
			}
		}
		return 0;
	}
}