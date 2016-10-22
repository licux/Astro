#ifndef __STAGE_H__
#define __STAGE_H__

#include "Arduboy.h"
#include "Scene.h"
#include "Rocket.h"
#include "Laser.h"
#include "Stone.h"
#include "Global.h"

#include <math.h>

extern int16_t score;

class Stage : public Scene
 {
private:


	static const int16_t LIFE_MAX = 3;
	static const int16_t PLANET_LIFE_MAX = 100;
	static const int16_t STONE_MAX = 10;
	static const int16_t STONE_LAUNCH_COUNT = 100;

	Stone* stone[STONE_MAX];
	Rocket* rocket;

	int level;
	int remaining_stone;

	bool isPause;
	bool isPausePushed;
	bool isContinue;
	int life;
	int planet_life;
	int stone_launch_count;

	char text[16];

public:
	Stage(Arduboy* _arduboy);
	~Stage();


	void init();
	void initLevel(int _level);
	void clearLevel();
	void reset();
	void HitCheck();
	bool HitCheck_Laser_Stone(Laser* l, Stone* s);
	bool HitCheck_Rocket_Stone(Rocket* r, Stone* s);
	bool run();
	void draw();

};

Stage::Stage(Arduboy* _arduboy)
{
	arduboy = _arduboy;
	init();
}

Stage::~Stage()
{

}

void Stage::init()
{
	life = Stage::LIFE_MAX;  
	planet_life = Stage::PLANET_LIFE_MAX;
	for (int i = 0; i < STONE_MAX; i++) {
		stone[i] = new Stone(arduboy);
	}
	rocket = new Rocket(arduboy);
	score = 0;
	stone_launch_count = 10;
	isPause = false;
	isPausePushed = false;
	isContinue = true;

	initLevel(1);
}

void Stage::initLevel(int _level)
{
	level = _level;
	remaining_stone = 30;
}

void Stage::clearLevel()
{
	delay(500);
	reset();
	initLevel(level + 1);
}

void Stage::reset()
{
	for (int i = 0; i < STONE_MAX; i++) {
		stone[i]->reset();
	}
	rocket->reset();
}

void Stage::HitCheck()
{
	int i = 0;
	for (i = 0; i < STONE_MAX; i++) {
		  HitCheck_Laser_Stone(rocket->getLaser(), stone[i]);
		  HitCheck_Rocket_Stone(rocket, stone[i]);
	}
}

bool Stage::HitCheck_Laser_Stone(Laser* l, Stone* s)
{
	bool flag = false;
	if (s->getStatus() == Stone::STONE_STATUS::LAUNCHED
		&& l->getStatus() == Laser::LASER_STATUS::EMITTED
		&& l->getLeft() <= s->getRight()
		&& l->getTop() <= s->getBottom()
		&& l->getRight() >= s->getLeft()
		&& l->getBottom() >= s->getTop()) {
		l->hit();
		Stone::HIT_STATUS status = s->hit();
		if (status == Stone::HIT_STATUS::SPLIT) {
			// splitting
			for (int i = 0; i < STONE_MAX; i++) {
				if (stone[i]->getStatus() == Stone::STONE_STATUS::NOT_LAUNCHED) {
					s->split(stone[i]);
					break;
				}
			}
			score += 10;
			if (music) {
				arduboy->tunes.tone(250, 150);
			}
		}
		else if (status == Stone::HIT_STATUS::BREAK){
			score += 10;
			remaining_stone--;
			if (music) {
				arduboy->tunes.tone(250, 150);
			}
		}

		flag = true;
	}
	return flag;
}

bool Stage::HitCheck_Rocket_Stone(Rocket* r, Stone* s)
{
	bool flag = false;
	if (s->getStatus() == Stone::STONE_STATUS::LAUNCHED
		&& r->getLeft() <= s->getRight()
		&& r->getTop() <= s->getBottom()
		&& r->getRight() >= s->getLeft()
		&& r->getBottom() >= s->getTop()) {
		r->crash();
		if (--life < 0) {
			isContinue = false;
		}
		reset();
	}
	return flag;
}

bool Stage::run()
{
	if(arduboy->pressed(UP_BUTTON + A_BUTTON)
		|| arduboy->pressed(UP_BUTTON + B_BUTTON)
		|| arduboy->pressed(DOWN_BUTTON + A_BUTTON)
		|| arduboy->pressed(DOWN_BUTTON + B_BUTTON)) {
		if (!isPausePushed) {
			isPause = !isPause;
			isPausePushed = true;
		}
	}
	else {
		isPausePushed = false;
	}

	if (isPause) {
		return isContinue;
	}

	if (remaining_stone < 0) {
		clearLevel();
	}
	HitCheck();

	bool isLaunch = false;
	stone_launch_count--;
	if (stone_launch_count < 0) {
		isLaunch = true;
		stone_launch_count = STONE_LAUNCH_COUNT;
	}

	for (int i = 0; i < STONE_MAX; i++) {
		if (stone[i]->getStatus() != Stone::STONE_STATUS::NOT_LAUNCHED) {
			if (stone[i]->move()) {
				planet_life -= 5;
				if (music) {
					arduboy->tunes.tone(200, 100);
				}
				if (planet_life < 0) {
					planet_life = 0;
					isContinue = false;
				}
			}
		}
		else if (stone[i]->getStatus() == Stone::STONE_STATUS::NOT_LAUNCHED) {
			if (isLaunch) {
				stone[i]->launch();
				isLaunch = false;
			}
		}
	}

	rocket->move();

	return isContinue;
}

void Stage::draw()
{
	int i = 0;

// Drawing Lines for Separating AREA
	arduboy->drawLine(GAME_PANEL_X, 0, GAME_PANEL_X, SCREEN_HEIGHT, 1);
	arduboy->drawLine(STAGE_INFO_PANEL_X, 0, STAGE_INFO_PANEL_X, SCREEN_HEIGHT, 1);

// Drawing ROCKET AREA
	//Drawing Level	
	arduboy->drawBitmap(1 + ROCKET_INFO_PANEL_X, 3, bitmap_level, 8, 5, 1);
	arduboy->drawBitmap(9 + ROCKET_INFO_PANEL_X, 3, bitmap_digit[level / 10], 3, 5, 1);
	arduboy->drawBitmap(13 + ROCKET_INFO_PANEL_X, 3, bitmap_digit[level % 10], 3, 5, 1);
	//Drawing life
	arduboy->drawBitmap(1 + ROCKET_INFO_PANEL_X, 12, bitmap_icon, 5, 5, 1);
	arduboy->drawBitmap(10 + ROCKET_INFO_PANEL_X, 12, bitmap_digit[life], 3, 5, 1);
	//Drawing Score
	int tmp = score;
	for (i = 0; i < 4; i++) {
		arduboy->drawBitmap(12 - i * 4 + ROCKET_INFO_PANEL_X, 22, bitmap_digit[tmp % 10], 3, 5, 1);
		tmp /= 10;
	}

// Drawing STAGE AREA
	for (i = 0; i < remaining_stone / 10; i++) {
		arduboy->drawCircle(3 + STAGE_INFO_PANEL_X, 2 + i * 4, 1, 1);
	}
	for (i = 0; i < remaining_stone % 10; i++) {
		arduboy->drawPixel(7 + STAGE_INFO_PANEL_X, 2 + i * 2, 1);
	}
	arduboy->drawRect(3 + STAGE_INFO_PANEL_X, 23, 7, 40, 1);
	arduboy->fillRect(3 + STAGE_INFO_PANEL_X, 23 + 40.0 * (float)(Stage::PLANET_LIFE_MAX - planet_life) / (float)PLANET_LIFE_MAX,
		7, ceil(40.0 * (float)planet_life / (float)PLANET_LIFE_MAX), 1);

// Drawing GAME AREA
	for (i = 0; i < STONE_MAX; i++) {
		stone[i]->draw();
	}
	rocket->draw();

	if (isPause) {
		arduboy->setCursor(50, 20);
		arduboy->print("PAUSE");
	}

}

#endif