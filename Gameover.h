#ifndef __GAMEOVER_H__
#define __GAMEOVER_H__

#include "Arduboy.h"
#include "Scene.h"
#include "Global.h"

extern int16_t score;

class Gameover : public Scene
{
private:
	enum DISPLAY_TYPE {
		RESULT,
		HIGHSCORE,
	};

	int16_t last_score;
	DISPLAY_TYPE type;
	bool button_pressed;
	char text[16];


public:
	Gameover(Arduboy* _arduboy);
	~Gameover();

	bool run();
	void draw();
};

Gameover::Gameover(Arduboy* _arduboy)
{
	arduboy = _arduboy;
	last_score = score;
	type = Gameover::DISPLAY_TYPE::RESULT;
	button_pressed = false;
}

Gameover::~Gameover()
{

}

bool Gameover::run() 
{
	bool flag = true;

	if (arduboy->pressed(A_BUTTON) | arduboy->pressed(B_BUTTON)) {
		if (!button_pressed) {
			if (type == Gameover::DISPLAY_TYPE::RESULT) {
				type = Gameover::DISPLAY_TYPE::HIGHSCORE;
				highscore.writeScore(score);
			}
			else if (type == Gameover::DISPLAY_TYPE::HIGHSCORE) {
				flag = false;
			}
		}
		button_pressed = true;
	}
	else {
		button_pressed = false;
	}

	return flag;

}

void Gameover::draw()
{
	if (type == Gameover::DISPLAY_TYPE::RESULT) {
		arduboy->setCursor(40, 20);
		arduboy->print("Gameover");
		arduboy->setCursor(30, 40);
		sprintf(text, "SCORE:%06d", score);
		arduboy->print(text);
	} else if (type == Gameover::DISPLAY_TYPE::HIGHSCORE){
		for (int i = 0; i < Highscore::MEMORY_MAX; i++) {
			arduboy->setCursor(40, 10 + i * 10);
			sprintf(text, "%d : %04d", i + 1, highscore.readScore(i));
			arduboy->print(text);
		}
	}
}

#endif // !__GAMEOVER_H__
