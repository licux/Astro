#ifndef __ROCKET_H__
#define __ROCKET_H__

#include "Arduboy.h"
#include "bitmaps.h"
#include "Laser.h"
#include "Global.h"

class Rocket {
private:
	const int16_t ROCKET_SPPED = 1;

	int16_t x;
	int16_t y;
	Laser* laser;
	bool isEmit;

	Arduboy* arduboy;

public:
	Rocket(Arduboy* _arduboy);

	~Rocket();
	
	Laser* getLaser();

	int16_t getLeft();
	int16_t getRight();
	int16_t getTop();
	int16_t getBottom();

	void reset();
	void move();
	void crash();
	void shot();
	void draw();
};


Rocket::Rocket(Arduboy* _arduboy)
{
	x = 10;
	y = 54;
	isEmit = false;
	arduboy = _arduboy;
	laser = new Laser(arduboy);
}

Rocket::~Rocket()
{

}

Laser* Rocket::getLaser()
{
	return laser;
}

int16_t Rocket::getLeft()
{
	return x;
}

int16_t Rocket::getRight()
{
	return x + 8;
}

int16_t Rocket::getTop()
{
	return y;
}

int16_t Rocket::getBottom()
{
	return y + 8;
}

void Rocket::reset()
{
	x = 10;
	y = 54;
	isEmit = false;
	laser->reset();
}

void Rocket::move()
{
	// move Rocekt
	if (arduboy->pressed(LEFT_BUTTON)) {
		x -= ROCKET_SPPED;
		if (x < 0) {
			x = 0;
		}
	}
	else if (arduboy->pressed(RIGHT_BUTTON)) {
		x += ROCKET_SPPED;
		if (x > GAME_PANEL_WIDTH - 8) {
			x = GAME_PANEL_WIDTH - 8;
		}
	}

	laser->move();
	// Shot Laser
	if (laser->getStatus() == Laser::LASER_STATUS::NOT_EMITTED) {
		if (arduboy->pressed(A_BUTTON) | arduboy->pressed(B_BUTTON)) {
			if (isEmit) {
				shot();
				isEmit = false;
			}
		}
		else {
			isEmit = true;
		}
	}

}

void Rocket::crash()
{
	arduboy->drawRect(x + GAME_PANEL_X, y, 8, 8, 0);
	arduboy->drawBitmap(x + GAME_PANEL_X, y, bitmap_explosion, 8, 8, 1);
	arduboy->display();
	if (music) {
		arduboy->tunes.tone(300, 500);
	}
	delay(500);
}

void Rocket::shot()
{
	laser->shot(x + 4, y);
	if (music) {
		arduboy->tunes.tone(400, 100);
	}
}

void Rocket::draw()
{
	arduboy->drawBitmap(x + GAME_PANEL_X, y, bitmap_rocket, 8, 8, 1);
	laser->draw();
}

#endif __ROCKET_H__