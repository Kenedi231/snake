#include <iostream>
#include <conio.h>
#include <windows.h>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


using namespace sf;
// Свойство головы змейки
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

// Габаритные размеры карты
const int mapwidth = 20;
const int mapheight = 20;

const int size = mapwidth * mapheight;

// Размеры окна
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

// Основная функция игры
void run()
{
	// Инициализация окна
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	RenderWindow window(VideoMode(screenWidth, screenHeight), "Snake", Style::Close, settings);
	window.setKeyRepeatEnabled(false);
	// Сообщение о конце игры(инициализация)
	Font font;
	font.loadFromFile("resourses/font.ttf");
	Text text("", font, 20);
	text.setFillColor(Color::Black);
	std::ostringstream playerScoreString;
	text.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
	// Проверка на запуск игры
	bool gameOver = false;
	// Кол-во еды у змейки (Длина тела)
	int food = 3;

	// Инициализация карты и змейки
	int map[size] = { 0 };
	properties snakeHead;
	initMap(map, snakeHead);
	while (window.isOpen()) {
		// Инициализация собитыя
		Event event;
		while (window.pollEvent(event))
		{
			// Окно закрывается при выходе
			if (event.type == Event::Closed)
				window.close();
			// Изменение направления взависимости от нажатой клавиши
			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == (Keyboard::Up)) if (snakeHead.direction != DIRECTION_DOWN) snakeHead.direction = DIRECTION_UP;
				if (event.key.code == (Keyboard::Right)) if (snakeHead.direction != DIRECTION_LEFT) snakeHead.direction = DIRECTION_RIGTH;
				if (event.key.code == (Keyboard::Down)) if (snakeHead.direction != DIRECTION_UP) snakeHead.direction = DIRECTION_DOWN;
				if (event.key.code == (Keyboard::Left)) if (snakeHead.direction != DIRECTION_RIGTH) snakeHead.direction = DIRECTION_LEFT;
			}
		}
		// Обновление карты
		update(&gameOver, map, &food, snakeHead);

		// Очистка экрана
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

		// Отрисовка карты
		printMap(map, window);
		Sleep(500);
	}
}

// Перемещение головы змея в новое место
void move(int dx, int dy, bool * gameOver, int * map, int * food, properties& snakeHead) {
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
		*gameOver = true;
	}

	// Перемещение головы в новое место
	snakeHead.headxpos = newx;
	snakeHead.headypos = newy;
	map[snakeHead.headxpos + snakeHead.headypos * mapwidth] = *food + 1;

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
void update(bool* gameOver, int * map, int * food, properties& snakeHead) {
	// Перемещение в указаном направлении
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
void printMap(int * map, RenderWindow& window)
{
	// Создаём блоки для карты
	RectangleShape block(Vector2f(widthCell, heightCell));
	block.setFillColor(Color(Color::Black));
	int tempVar;
	for (int y = 0; y < mapwidth; ++y) {
		for (int x = 0; x < mapheight; ++x) {
			// Печать значений на x, y
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
