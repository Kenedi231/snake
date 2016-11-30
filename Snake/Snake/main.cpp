#include <iostream>
#include <conio.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#pragma warning(disable:4996) // POSIX name deprecated
// Свойство головы змейки
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

// Габаритные размеры карты
const int mapwidth = 20;
const int mapheight = 20;

const int size = mapwidth * mapheight;

// Размеры окна
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

// Основная функция игры
void run()
{
	// Инициализация окна
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Analog Clock", sf::Style::Close, settings);
	// Проверка на запуск игры
	bool running = true;

	// Кол-во еды у змейки (Длина тела)
	int food = 3;

	// Инициализация карты и змейки
	int map[size] = { 0 };
	properties snakeHead;
	initMap(map, snakeHead);
	while (running) {
		// Если нажата клавиша
		if (kbhit()) {
			// Изменение направления взависимости от нажатой клавиши
			changeDirection(getch(), snakeHead);
		}
		// Обновление карты
		update(&running, map, &food, snakeHead);

		// Очистка экрана
		clearScreen();

		// Отрисовка карты
		printMap(map);

		// Задержка в 0.1 секунд
		_sleep(100);
	}

	// Печать текста о завершении игры
	std::cout << "\t\t!!!Game over!" << std::endl << "\t\tYour score is: " << food;

	// Запрещает консоли закрыться мгновенно
	std::cin.ignore();
}

// Меняет направления от клавиши
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

// Перемещение головы змея в новое место
void move(int dx, int dy, bool * running, int * map, int * food, properties& snakeHead) {
	// Определить где находися голова
	int newx = snakeHead.headxpos + dx;
	int newy = snakeHead.headypos + dy;

	// Проверка есть ли еда на карте
	if (map[newx + newy * mapwidth] == -2) {
		// Повышение кол-ва еды (Длины змейки)
		*food = *food + 1;

		// Генерация новой еды на карте
		generateFood(map);
	}

	// Проверка места на свободность
	else if (map[newx + newy * mapwidth] != 0) {
		*running = false;
	}

	// Перемещение головы в новое место
	snakeHead.headxpos = newx;
	snakeHead.headypos = newy;
	map[snakeHead.headxpos + snakeHead.headypos * mapwidth] = *food + 1;

}

// Очистка экрана
void clearScreen() {
	// Очистка экрана
	system("cls");
}

// Генерация новой еды на карте
void generateFood(int * map) {
	int x = 0;
	int y = 0;
	do {
		// Генерация координат x и y 
		x = rand() % (mapwidth - 2) + 1;
		y = rand() % (mapheight - 2) + 1;

		// Если место не свободно то попробуй снова
	} while (map[x + y * mapwidth] != 0);

	// Размещение еды
	map[x + y * mapwidth] = -2;
}

// Обновление карты
void update(bool* running, int * map, int * food, properties& snakeHead) {
	// Перемещение в указаном направлении
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

	// Удаление старых мест где была змейка на 1
	for (int i = 0; i < size; i++) {
		if (map[i] > 0) map[i]--;
	}
}

// Инициализация карты
void initMap(int * map, properties& snakeHead)
{
	// Размещение головы на середине карты
	snakeHead.direction = 0;
	snakeHead.headxpos = mapwidth / 2;
	snakeHead.headypos = mapheight / 2;
	map[snakeHead.headxpos + snakeHead.headypos * mapwidth] = 1;

	// Размещение вверху и внизу стен
	for (int x = 0; x < mapwidth; ++x) {
		map[x] = -1;
		map[x + (mapheight - 1) * mapwidth] = -1;
	}

	// Размещение слева и справа стен
	for (int y = 0; y < mapheight; y++) {
		map[0 + y * mapwidth] = -1;
		map[(mapwidth - 1) + y * mapwidth] = -1;
	}

	// Генерация первой еды
	generateFood(map);
}

// Печать карты в консоли
void printMap(int * map)
{
	for (int y = 0; y < mapwidth; ++y) {
		for (int x = 0; x < mapheight; ++x) {
			// Печать значений на x, y
			std::cout << getMapValue(map[x + y * mapwidth]);
		}
		// Завершает линию для следующего значения x
		std::cout << std::endl;
	}
}

// Возращает графический символ для отображение его на карте
char getMapValue(int value)
{
	// Возращает часть тела змейки
	if (value > 0) return 'o';

	switch (value) {
		// Возращает стену
	case -1: return 'X';
		// Возращает еду
	case -2: return 'O';
	}

	return ' ';
}