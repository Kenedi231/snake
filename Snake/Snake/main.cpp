#include <iostream>
#include <conio.h>
#include <windows.h>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


using namespace sf;
// �������� ������ ������
struct properties
{
	int headxpos;
	int headypos;
	int direction;
};

void run();
void printMap(int * map, RenderWindow& window);
void initMap(int * map, properties& snakeHead);
void move(int dx, int dy, bool * gameOver, int * map, int * food, properties& snakeHead);
void update(bool* gameOver, int * map, int * food, properties& snakeHead);
void generateFood(int * map);

// ���������� ������� �����
const int mapwidth = 20;
const int mapheight = 20;

const int size = mapwidth * mapheight;

// ������� ����
const int screenWidth = 600;
const int screenHeight = 600;
const int widthCell = screenWidth / mapwidth;
const int heightCell = screenHeight / mapheight;

enum Direction
{
	DIRECTION_LEFT = 3,
	DIRECTION_RIGTH = 1,
	DIRECTION_UP = 0,
	DIRECTION_DOWN = 2,
};

int main()
{
	run();
	return 0;
}

// �������� ������� ����
void run()
{
	// ������������� ����
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	RenderWindow window(VideoMode(screenWidth, screenHeight), "Snake", Style::Close, settings);
	window.setKeyRepeatEnabled(false);
	// ��������� � ����� ����(�������������)
	Font font;
	font.loadFromFile("resourses/font.ttf");
	Text text("", font, 20);
	text.setFillColor(Color::Black);
	std::ostringstream playerScoreString;
	text.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
	// �������� �� ������ ����
	bool gameOver = false;
	// ���-�� ��� � ������ (����� ����)
	int food = 3;

	// ������������� ����� � ������
	int map[size] = { 0 };
	properties snakeHead;
	initMap(map, snakeHead);
	while (window.isOpen()) {
		// ������������� �������
		Event event;
		while (window.pollEvent(event))
		{
			// ���� ����������� ��� ������
			if (event.type == Event::Closed)
				window.close();
			// ��������� ����������� ������������ �� ������� �������
			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == (Keyboard::Up)) if (snakeHead.direction != DIRECTION_DOWN) snakeHead.direction = DIRECTION_UP;
				if (event.key.code == (Keyboard::Right)) if (snakeHead.direction != DIRECTION_LEFT) snakeHead.direction = DIRECTION_RIGTH;
				if (event.key.code == (Keyboard::Down)) if (snakeHead.direction != DIRECTION_UP) snakeHead.direction = DIRECTION_DOWN;
				if (event.key.code == (Keyboard::Left)) if (snakeHead.direction != DIRECTION_RIGTH) snakeHead.direction = DIRECTION_LEFT;
			}
		}
		// ���������� �����
		update(&gameOver, map, &food, snakeHead);

		// ������� ������
		window.clear(Color::White);
		if (gameOver)
		{
			playerScoreString << food;
			text.setString("\tGame Over! \nYour score is " + playerScoreString.str());
			window.draw(text);
			window.display();
			Sleep(3000);
			window.close();
		}

		// ��������� �����
		printMap(map, window);
		Sleep(500);
	}
}

// ����������� ������ ���� � ����� �����
void move(int dx, int dy, bool * gameOver, int * map, int * food, properties& snakeHead) {
	// ���������� ��� �������� ������
	int newx = snakeHead.headxpos + dx;
	int newy = snakeHead.headypos + dy;

	// �������� ���� �� ��� �� �����
	if (map[newx + newy * mapwidth] == -2) {
		// ��������� ���-�� ��� (����� ������)
		*food = *food + 1;

		// ��������� ����� ��� �� �����
		generateFood(map);
	}

	// �������� ����� �� �����������
	else if (map[newx + newy * mapwidth] != 0) {
		*gameOver = true;
	}

	// ����������� ������ � ����� �����
	snakeHead.headxpos = newx;
	snakeHead.headypos = newy;
	map[snakeHead.headxpos + snakeHead.headypos * mapwidth] = *food + 1;

}

// ��������� ����� ��� �� �����
void generateFood(int * map) {
	int x = 0;
	int y = 0;
	do {
		// ��������� ��������� x � y 
		x = rand() % (mapwidth - 2) + 1;
		y = rand() % (mapheight - 2) + 1;

		// ���� ����� �� �������� �� �������� �����
	} while (map[x + y * mapwidth] != 0);

	// ���������� ���
	map[x + y * mapwidth] = -2;
}

// ���������� �����
void update(bool* gameOver, int * map, int * food, properties& snakeHead) {
	// ����������� � �������� �����������
	switch (snakeHead.direction) {
	case DIRECTION_LEFT: move(-1, 0, gameOver, map, food, snakeHead);
		break;
	case DIRECTION_UP: move(0, -1, gameOver, map, food, snakeHead);
		break;
	case DIRECTION_RIGTH: move(1, 0, gameOver, map, food, snakeHead);
		break;
	case DIRECTION_DOWN: move(0, 1, gameOver, map, food, snakeHead);
		break;
	}

	// �������� ������ ���� ��� ���� ������ �� 1
	for (int i = 0; i < size; i++) {
		if (map[i] > 0) map[i]--;
	}
}

// ������������� �����
void initMap(int * map, properties& snakeHead)
{
	// ���������� ������ �� �������� �����
	snakeHead.direction = 0;
	snakeHead.headxpos = mapwidth / 2;
	snakeHead.headypos = mapheight / 2;
	map[snakeHead.headxpos + snakeHead.headypos * mapwidth] = 1;

	// ���������� ������ � ����� ����
	for (int x = 0; x < mapwidth; ++x) {
		map[x] = -1;
		map[x + (mapheight - 1) * mapwidth] = -1;
	}

	// ���������� ����� � ������ ����
	for (int y = 0; y < mapheight; y++) {
		map[0 + y * mapwidth] = -1;
		map[(mapwidth - 1) + y * mapwidth] = -1;
	}

	// ��������� ������ ���
	generateFood(map);
}

// ������ ����� � �������
void printMap(int * map, RenderWindow& window)
{
	// ������ ����� ��� �����
	RectangleShape block(Vector2f(widthCell, heightCell));
	block.setFillColor(Color(Color::Black));
	int tempVar;
	for (int y = 0; y < mapwidth; ++y) {
		for (int x = 0; x < mapheight; ++x) {
			// ������ �������� �� x, y
			tempVar = map[x + y * mapwidth];
			if (tempVar != 0)
			{
				block.setPosition(x * widthCell, y * heightCell);
				if (tempVar > 0)
				{
					block.setFillColor(Color(Color::Green));
				}
				else if (tempVar == -1)
				{
					block.setFillColor(Color(Color::Black));
				}
				else if (tempVar == -2)
				{
					block.setFillColor(Color(Color::Red));
				}
				window.draw(block);
			}
		}
	}
	window.display();
}
