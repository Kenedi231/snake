#include <iostream>
#include <conio.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#pragma warning(disable:4996) // POSIX name deprecated
// �������� ������ ������
struct properties
{
	int headxpos;
	int headypos;
	int direction;
};

void run();
void printMap(int * map);
void initMap(int * map, properties& snakeHead);
void move(int dx, int dy, bool * running, int * map, int * food, properties& snakeHead);
void update(bool* running, int * map, int * food, properties& snakeHead);
void changeDirection(char key, properties& snakeHead);
void clearScreen();
void generateFood(int * map);

char getMapValue(int value);

// ���������� ������� �����
const int mapwidth = 20;
const int mapheight = 20;

const int size = mapwidth * mapheight;

// ������� ����
const int screenWidth = 800;
const int screenHeight = 600;

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
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Analog Clock", sf::Style::Close, settings);
	// �������� �� ������ ����
	bool running = true;

	// ���-�� ��� � ������ (����� ����)
	int food = 3;

	// ������������� ����� � ������
	int map[size] = { 0 };
	properties snakeHead;
	initMap(map, snakeHead);
	while (running) {
		// ���� ������ �������
		if (kbhit()) {
			// ��������� ����������� ������������ �� ������� �������
			changeDirection(getch(), snakeHead);
		}
		// ���������� �����
		update(&running, map, &food, snakeHead);

		// ������� ������
		clearScreen();

		// ��������� �����
		printMap(map);

		// �������� � 0.1 ������
		_sleep(100);
	}

	// ������ ������ � ���������� ����
	std::cout << "\t\t!!!Game over!" << std::endl << "\t\tYour score is: " << food;

	// ��������� ������� ��������� ���������
	std::cin.ignore();
}

// ������ ����������� �� �������
void changeDirection(char key, properties& snakeHead) {
	switch (key) {
	case 'w':
		if (snakeHead.direction != DIRECTION_DOWN) snakeHead.direction = DIRECTION_UP;
		break;
	case 'd':
		if (snakeHead.direction != DIRECTION_LEFT) snakeHead.direction = DIRECTION_RIGTH;
		break;
	case 's':
		if (snakeHead.direction != DIRECTION_UP) snakeHead.direction = DIRECTION_DOWN;
		break;
	case 'a':
		if (snakeHead.direction != DIRECTION_RIGTH) snakeHead.direction = DIRECTION_LEFT;
		break;
	}
}

// ����������� ������ ���� � ����� �����
void move(int dx, int dy, bool * running, int * map, int * food, properties& snakeHead) {
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
		*running = false;
	}

	// ����������� ������ � ����� �����
	snakeHead.headxpos = newx;
	snakeHead.headypos = newy;
	map[snakeHead.headxpos + snakeHead.headypos * mapwidth] = *food + 1;

}

// ������� ������
void clearScreen() {
	// ������� ������
	system("cls");
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
void update(bool* running, int * map, int * food, properties& snakeHead) {
	// ����������� � �������� �����������
	switch (snakeHead.direction) {
	case DIRECTION_LEFT: move(-1, 0, running, map, food, snakeHead);
		break;
	case DIRECTION_UP: move(0, -1, running, map, food, snakeHead);
		break;
	case DIRECTION_RIGTH: move(1, 0, running, map, food, snakeHead);
		break;
	case DIRECTION_DOWN: move(0, 1, running, map, food, snakeHead);
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
void printMap(int * map)
{
	for (int y = 0; y < mapwidth; ++y) {
		for (int x = 0; x < mapheight; ++x) {
			// ������ �������� �� x, y
			std::cout << getMapValue(map[x + y * mapwidth]);
		}
		// ��������� ����� ��� ���������� �������� x
		std::cout << std::endl;
	}
}

// ��������� ����������� ������ ��� ����������� ��� �� �����
char getMapValue(int value)
{
	// ��������� ����� ���� ������
	if (value > 0) return 'o';

	switch (value) {
		// ��������� �����
	case -1: return 'X';
		// ��������� ���
	case -2: return 'O';
	}

	return ' ';
}