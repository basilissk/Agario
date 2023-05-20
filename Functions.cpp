#include "Functions.h"
#include "Connection.h"
#include <cstring>
#include <thread>
#include <future>

using namespace std;

void drawingMap() {
	background.setFillColor(Color(247, 247, 247));
	background.setOutlineThickness(5);
	background.setOutlineColor(Color(70, 130, 180));

	lines.setFillColor(Color::Red);
	lines.move(10, 90);
}

void drawingResults() {
	resultsBackground.setFillColor(Color(240, 128, 128));
}

void createButton(Text& text, RectangleShape& button) {
	text.setFillColor(Color(51, 255, 153));
	text.setCharacterSize(75);
	text.setOutlineThickness(5.0);
	text.setPosition(button.getPosition());
}

void buttonNotSelected(Text& text, RectangleShape& button) {
	text.setFillColor(Color(51, 255, 153));
	text.setCharacterSize(75);
	text.setPosition(button.getPosition());
}

int drawingMenu() {
	int menuMode;
	bool game = false;
	bool rules = false;
	bool exit = false;
	enum modes { gameMode, rulesMode, exitMode, noMode };
	modes mode;
	Text menuText("", font);
	Text startText("", font);
	Text rulesText("", font);
	Text exitText("", font);
	Texture menuBackgroundT;
	Texture menuT;
	Sprite menuBackground;
	Sprite menu;
	RectangleShape startButton(Vector2f(400, 100));
	RectangleShape rulesButton(Vector2f(400, 100));
	RectangleShape exitButton(Vector2f(400, 100));

	menuBackgroundT.loadFromFile("resources\\menu.png");
	menuT.loadFromFile("resources\\menuRect.png");
	menuBackground.setTexture(menuBackgroundT);
	menu.setTexture(menuT);
	menuBackground.setScale(0.5, 0.5);
	menu.setScale(1.3, 1.3);
	menu.setColor(Color(255, 255, 255, 230));
	menu.setPosition(windowWidth / 4, windowHeight / 10);

	menuText.setString("Agar.io");
	menuText.setFillColor(Color(51, 255, 153));
	menuText.setCharacterSize(170);
	menuText.setOutlineThickness(10);
	menuText.setLetterSpacing(1.5);
	menuText.setPosition(windowWidth / 3, windowHeight / 6);

	startButton.setPosition(560, 500);
	rulesButton.setPosition(615, 600);
	exitButton.setPosition(648, 700);

	startButton.setFillColor(Color(0, 0, 0, 0));
	rulesButton.setFillColor(Color(0, 0, 0, 0));
	exitButton.setFillColor(Color(0, 0, 0, 0));

	createButton(startText, startButton);
	startText.setString(L"Начать игру");

	createButton(rulesText, rulesButton);
	rulesText.setString(L"Правила");

	createButton(exitText, exitButton);
	exitText.setString(L"Выход");

	while (!game || !rules || !exit) {
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(menuBackground);
		window.draw(menu);
		window.draw(menuText);
		window.draw(startButton);
		window.draw(startText);
		window.draw(rulesButton);
		window.draw(rulesText);
		window.draw(exitButton);
		window.draw(exitText);

		mode = noMode;


		if (IntRect(560, 500, 400, 100).contains(Mouse::getPosition(window)))
		{
			startText.setFillColor(Color(102, 178, 255));
			startText.setCharacterSize(90);
			startText.setPosition(560 - 35, 500 - 10);
			mode = gameMode;
		}
		else
		{
			buttonNotSelected(startText, startButton);
		}

		if (IntRect(560, 600, 400, 100).contains(Mouse::getPosition(window)))
		{
			rulesText.setFillColor(Color(102, 178, 255));
			rulesText.setCharacterSize(90);
			rulesText.setPosition(615 - 30, 600 - 10);
			mode = rulesMode;
		}
		else
		{
			buttonNotSelected(rulesText, rulesButton);
		}

		if (IntRect(560, 700, 400, 100).contains(Mouse::getPosition(window)))
		{
			exitText.setFillColor(Color(255, 51, 51));
			exitText.setCharacterSize(90);
			exitText.setPosition(648 - 25, 700 - 10);
			mode = exitMode;
		}
		else
		{
			buttonNotSelected(exitText, exitButton);
			exitText.setFillColor(Color(51, 255, 153));
		}

		window.display();

		if (Mouse::isButtonPressed(Mouse::Left)) {
			if (mode == gameMode) return 0;
			if (mode == exitMode) return 1;
			if (mode == rulesMode) drawingRules(menuBackground);
		}
	}
}

int DrawingConnection() {
	bool backPressed = false;
	bool connected = false;
	Texture rulesT;
	Texture backT;
	Texture menuT;
	Texture menuBackgroundT;
	Sprite back;
	Sprite menu;
	Sprite menuBackground;
	Text text("", font);
	backT.loadFromFile("resources\\back.png");
	menuT.loadFromFile("resources\\menuRect.png");
	menuBackgroundT.loadFromFile("resources\\menu.png");
	menuBackground.setTexture(menuBackgroundT);
	menuBackground.setScale(0.5, 0.5);
	menu.setTexture(menuT);
	menu.setScale(1.3, 1.3);
	menu.setColor(Color(255, 255, 255, 230));
	menu.setPosition(windowWidth / 4, windowHeight / 10);
	back.setTexture(backT);
	back.setScale(0.2, 0.2);
	back.setPosition(30, 30);
	text.setPosition(580, 500);
	text.setFillColor(Color(0, 0, 0, 255));

	wstring inscriptions[4] = { L"Подключение к серверу...", L"Ошибка подключения", L"Подключено игроков 1/2...", L"Подключено игроков 2/2!" };

	enum conditions {Connecting, Error, Connection, Connection2};
	conditions condition = Connecting;

	bool all_players_connected = false;

	text.setString(inscriptions[0]);

	sock_sockaddr_in* sock_and_addr = CreateSocket();
	if (sock_and_addr == NULL) {
		// Обработка ошибки
	}
	
	thread tryingConnect([&]()
		{
			if (TryingToConnect(sock_and_addr->sock, sock_and_addr->serv_addr) == true) {
				sock_and_addr->sock;
				condition = Connection;
			}
			else
				condition = Error;
		});

	/*thread waitingPlayers([&]()
		{
			if (WaitingForPlayers(sock_and_addr->sock)) {
				all_players_connected = true;
				condition = Connection2;
			};
		});*/
	

	while (!backPressed) {

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		switch (condition)
		{
		case Connecting:
			text.setString(inscriptions[0]);
			break;
		case Error:
			text.setString(inscriptions[1]);
			break;
		case Connection:
			text.setString(inscriptions[2]);
			break;
		case Connection2:
			text.setString(inscriptions[3]);
			break;
		default:
			break;
		}
		
		if (condition == Connection && !all_players_connected) {
			//thread waitingPlayers([&]()
			//	{
					if (WaitingForPlayers(sock_and_addr->sock)) {
						all_players_connected = true;
						condition = Connection2;
					};
			//	});
		}

		window.clear();
		window.draw(menuBackground);
		window.draw(menu);
		window.draw(back);
		window.draw(text);
		window.display();

		if (IntRect(30, 30, 130, 130).contains(Mouse::getPosition(window)))
		{
			back.setScale(0.25, 0.25);
			back.setPosition(18, 18);
			if (Mouse::isButtonPressed(Mouse::Left)) { 
				backPressed = true;
			};
		}
		else
		{
			back.setScale(0.2, 0.2);
			back.setPosition(30, 30);
		}
		if (Keyboard::isKeyPressed(Keyboard::BackSpace)) backPressed = true;

		if (all_players_connected) { 
			tryingConnect.join();
			return sock_and_addr->sock; 
		}

	}
	tryingConnect.join();
	return NULL;
}


void drawingRules(Sprite menuBackground)
{
	bool backPressed = false;
	Texture rulesT;
	Texture backT;
	Sprite back;
	Sprite rules;
	rulesT.loadFromFile("resources\\rules.png");
	rules.setTexture(rulesT);
	rules.setPosition(200, 90);
	backT.loadFromFile("resources\\back.png");
	back.setTexture(backT);
	back.setScale(0.2, 0.2);
	back.setPosition(30, 30);

	while (!backPressed) {
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		window.clear();
		window.draw(menuBackground);
		window.draw(rules);
		window.draw(back);
		window.display();

		if (IntRect(30, 30, 130, 130).contains(Mouse::getPosition(window)))
		{
			back.setScale(0.25, 0.25);
			back.setPosition(18, 18);
			if (Mouse::isButtonPressed(Mouse::Left)) backPressed = true;
		}
		else
		{
			back.setScale(0.2, 0.2);
			back.setPosition(30, 30);
		}
		if (Keyboard::isKeyPressed(Keyboard::BackSpace)) backPressed = true;
	}
}

void drawingWinOrLose(bool& is_the_end_of_play, bool& is_the_player_death, bool& is_the_end_of_program, bool win_or_Lose)
{
	bool flag = false;
	Text text("", font);

	while (!flag) {
		view.reset(FloatRect(0, 0, windowWidth, windowHeight));

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		if (!win_or_Lose) {
			window.clear(Color(240, 128, 128));
			text.setString(L"Вас съели!");
			text.setCharacterSize(150);
			text.setFillColor(Color::Red);
			text.setPosition(view.getCenter().x - 340, view.getCenter().y - 280);
		}
		else
		{
			window.clear(Color(176, 224, 230));
			text.setString(L"Вы победили!");
			text.setCharacterSize(130);
			text.setFillColor(Color::Blue);
			text.setPosition(view.getCenter().x - 380, view.getCenter().y - 280);
		}

		text.setOutlineThickness(7);


		window.setView(view);
		window.draw(text);

		text.setString(L"Нажмите R чтобы начать новую игру");
		text.setCharacterSize(40);
		text.setFillColor(Color::Black);
		text.setOutlineThickness(0);
		text.setPosition(view.getCenter().x - 300, view.getCenter().y - 70);
		window.draw(text);

		text.setString(L"Нажмите пробел чтобы вернуться в меню");
		text.setPosition(view.getCenter().x - 300, view.getCenter().y);
		window.draw(text);

		text.setString(L"Нажмите Esc чтобы выйти");
		text.setPosition(view.getCenter().x - 300, view.getCenter().y + 70);
		window.draw(text);

		window.display();

		if (Keyboard::isKeyPressed(Keyboard::R))
		{
			is_the_end_of_play = false;
			flag = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			is_the_end_of_play = true;
			flag = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			is_the_end_of_program = true;
			is_the_end_of_play = true;
			is_the_player_death = true;
			flag = true;
		}
	}
}

bool isItVisible(Player p, float X, float Y) {
	float pX = p.getPlayerCoordX();
	float pY = p.getPlayerCoordY();

	if (((X >= pX - drawingDistance && X <= pX) || (X <= pX + drawingDistance && X >= pX))
		&& ((Y >= pY - drawingDistance * 0.65 && Y <= pY) || (Y <= pY + drawingDistance * 0.65 && Y >= pY))) return true;
	else return false;
}

void eatingEnemy(Player& p, Enemy& e) {

	float X1 = p.getPlayerCoordX();
	float Y1 = p.getPlayerCoordY();
	float X2 = e.getPlayerCoordX();
	float Y2 = e.getPlayerCoordY();
	float S1 = p.getPlayerSize();
	float S2 = e.getPlayerSize();

	if (S1 > S2) {
		if ((((X2 <= X1) && (X2 >= X1 - S1)) || ((X2 >= X1) && (X2 <= X1 + S1))) &&
			(((Y2 <= Y1) && (Y2 >= Y1 - S1)) || ((Y2 >= Y1) && (Y2 <= Y1 + S1))))
		{
			if (e.life == true) p.size += e.size / 3;
			p.playerBody.setRadius(p.size);
			p.playerBody.setPosition(p.x - p.size, p.y - p.size);
			e.life = false;
		}
	}

	if (S1 < S2) {
		if ((((X1 <= X2) && (X1 >= X2 - S2)) || ((X1 >= X2) && (X1 <= X2 + S2))) &&
			(((Y1 <= Y2) && (Y1 >= Y2 - S2)) || ((Y1 >= Y2) && (Y1 <= Y2 + S2))))
			p.life = false;
	}
}
