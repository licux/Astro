#ifndef __LASER_H__
#define __LASER_H__

#include "Arduboy.h"
#include "Global.h"

class Laser
{
public:
	const int LASER_SPEED = 3;

	enum LASER_STATUS {
		NOT_EMITTED,
		EMITTED,
	};

private:
	int16_t x;
	int16_t y;
	LASER_STATUS status;

	Arduboy* arduboy;

public:
	Laser(Arduboy* arduboy);

	~Laser();

	int16_t getLeft();
	int16_t getRight();
	int16_t getTop();
	int16_t getBottom();
	LASER_STATUS getStatus();

	void reset();
	void shot(uint16_t _x, uint16_t y);
	void hit();
	void move();
	void draw();
};

Laser::Laser(Arduboy* _arduboy) 
	: status(LASER_STATUS::NOT_EMITTED)
	, arduboy(_arduboy)
{
}

Laser::~Laser()
{

}

int16_t Laser::getLeft()
{
	return x;
}

int16_t Laser::getRight()
{
	return x;
}
int16_t Laser::getTop()
{
	return y;
}

int16_t Laser::getBottom()
{
	return y + 3;
}

Laser::LASER_STATUS Laser::getStatus()
{
	return status;
}

void Laser::reset()
{
	status = LASER_STATUS::NOT_EMITTED;
}

void Laser::shot(uint16_t _x, uint16_t _y)
{
	x = _x;
	y = _y;
	status = Laser::LASER_STATUS::EMITTED;
}

void Laser::hit()
{
	status = Laser::LASER_STATUS::NOT_EMITTED;
}

void Laser::move()
{
	if (status == Laser::LASER_STATUS::NOT_EMITTED)
		return;

	y -= LASER_SPEED;
	if (y < 0) {
		status = Laser::LASER_STATUS::NOT_EMITTED;
	}
}

void Laser::draw()
{
	if (status == Laser::LASER_STATUS::EMITTED) {
		arduboy->drawRect(x + GAME_PANEL_X, y, 1, 3, 1);
	}
}

#endif __LASER_H__