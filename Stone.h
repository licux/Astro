#ifndef __STONE_H__
#define __STONE_H__

#include "Arduboy.h"
#include "bitmaps.h"
#include "Global.h"

class Stone
{
public:

	static const int16_t STONE_SPPED = 1;
	static const int16_t EXPLOSION_MAX = 20;

	static const int16_t MOVE_TABLE[3];
	static const int16_t DURABILITY_TABLE[3];
	static const int16_t SIZE_TABLE[3];

	enum STONE_STATUS {
		NOT_LAUNCHED,
		LAUNCHED,
		EXPLOSION,
	};

	enum HIT_STATUS {
		NOT_BREAK,
		BREAK,
		SPLIT,
	};


private:
	Arduboy* arduboy;

	int16_t x;
	int16_t y;
	int16_t move_count;

	int16_t size;
	int16_t level;
	int16_t durability;

	int16_t explosion_count;

	STONE_STATUS status;

public:
	Stone(Arduboy* _arduboy);
	~Stone();

	int16_t getLeft();
	int16_t getRight();
	int16_t getTop();
	int16_t getBottom();

	STONE_STATUS getStatus();

	void reset();
	HIT_STATUS hit();
	void split(Stone* s);
	void launch(bool split = false, int16_t _x = 0, int16_t _y = 0);
	bool move();
	void draw();
};

const int16_t Stone::MOVE_TABLE[3] = { 2, 3, 5 };
const int16_t Stone::DURABILITY_TABLE[3] = { 0, 1, 2 };
const int16_t Stone::SIZE_TABLE[3] = { 8, 16, 16 };

Stone::Stone(Arduboy* _arduboy) {
	arduboy = _arduboy;

	x = 0;
	y = -10;
	move_count = 0;
	status = Stone::STONE_STATUS::NOT_LAUNCHED;

}

Stone::~Stone()
{

}

int16_t Stone::getLeft()
{
	return x;
}

int16_t Stone::getRight()
{
	return x + size;
}

int16_t Stone::getTop()
{
	return y;
}

int16_t Stone::getBottom()
{
	return y + size;
}

Stone::STONE_STATUS Stone::getStatus()
{
	return status;
}

void Stone::reset()
{
	status = Stone::STONE_STATUS::NOT_LAUNCHED;
}

Stone::HIT_STATUS Stone::hit()
{
	Stone::HIT_STATUS s = Stone::HIT_STATUS::NOT_BREAK;

	durability--;
	if (durability < 0) {
		level--;
		if (level < 0) {
			s = Stone::HIT_STATUS::BREAK;
			status = Stone::STONE_STATUS::EXPLOSION;
			explosion_count = Stone::EXPLOSION_MAX;
		}
		else {
			s = Stone::HIT_STATUS::SPLIT;
			durability = DURABILITY_TABLE[level];
			size = SIZE_TABLE[level];
		}
	}

	return s;
}

void Stone::split(Stone* s)
{
	s->launch(true, x + 10 + random(-5, 5), y);
	x -= random(0, 5);
	if (x < 0) {
		x = 0;
	}
}

void Stone::launch(bool split, int16_t _x, int16_t _y)
{
	status = Stone::STONE_STATUS::LAUNCHED;
	if (!split) {
		x = (uint16_t)random(0, GAME_PANEL_WIDTH);
		y = -10;

		level = random(0, 2);
		move_count = MOVE_TABLE[level];
		durability = DURABILITY_TABLE[level];
		size = SIZE_TABLE[level];
	}
	else {
		x = _x;
		y = _y;

		level = 0;
		durability = DURABILITY_TABLE[level];
		size = SIZE_TABLE[level];
	}

	if (x + size > GAME_PANEL_WIDTH) {
		x = GAME_PANEL_WIDTH - size;
	}
}

bool Stone::move()
{
	bool flag = false;

	if (status == Stone::STONE_STATUS::LAUNCHED) {
		move_count--;
		if (move_count < 0) {
			y += STONE_SPPED;
			if (y > SCREEN_HEIGHT) {
				status = Stone::STONE_STATUS::NOT_LAUNCHED;
				flag = true;
			}
			move_count = MOVE_TABLE[level];
		}
	}
	else if (status == Stone::STONE_STATUS::EXPLOSION) {
		if (explosion_count-- < 0) {
			status = Stone::STONE_STATUS::NOT_LAUNCHED;
		}
	}

	return flag;
}

void Stone::draw()
{
	if (status == Stone::STONE_STATUS::LAUNCHED) {
		switch (level) {
		case 0:
			arduboy->drawBitmap(x + GAME_PANEL_X, y, bitmap_stone_1, size, size, 1);
			break;
		case 1:
			arduboy->drawSlowXYBitmap(x + GAME_PANEL_X, y, bitmap_stone_2, size, size, 1);
			break;
		case 2:
			break;
		}
	}
	else if (status == Stone::STONE_STATUS::EXPLOSION) {
		arduboy->drawBitmap(x + GAME_PANEL_X, y, bitmap_explosion, 8, 8, 1);
	}
}
#endif // __STONE_H__