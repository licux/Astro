#ifndef __TITLE_H__
#define __TITLE_H__

#include "Arduboy.h"
#include "Scene.h"
#include "bitmaps.h"
#include "Global.h"

class Title : public Scene
{
private:
	enum MENU {
		PLAY = 0,
		MUSIC = 1,
		HIGHSCORE = 2,
	};

	const int8_t CURSOR_PLAY = 34;
	const int8_t CURSOR_MUSIC = 44;
	const int8_t CURSOR_HIGHSCORE = 54;

	MENU select_menu;
	int8_t cursor_y;

	bool cursor_pressed;
	bool button_pressed;
	bool showHighscore;
	
	char text[16];
public:
	Title(Arduboy* _arduoy);
	~Title();

	bool run();
	void draw();

};
Title::Title(Arduboy* _arduboy)
{
	arduboy = _arduboy;
	select_menu = Title::MENU::PLAY;
	cursor_y = CURSOR_PLAY;
	music = false;
	cursor_pressed = true;
	button_pressed = true;
	showHighscore = false;
}

Title::~Title()
{

}

bool Title::run()
{
	bool flag = true;

	if (showHighscore) {
		if (arduboy->pressed(A_BUTTON) || arduboy->pressed(B_BUTTON)) {
			if (!button_pressed) {
				showHighscore = !showHighscore;
			}
			button_pressed = true;
		}
		else {
			button_pressed = false;
		}
		return flag;
	}

	if (arduboy->pressed(DOWN_BUTTON)) {
		if (!cursor_pressed){
			switch (select_menu) {
			case Title::MENU::PLAY:
				select_menu = Title::MENU::MUSIC;
				cursor_y = Title::CURSOR_MUSIC;
				break;
			case Title::MENU::MUSIC:
				select_menu = Title::MENU::HIGHSCORE;
				cursor_y = Title::CURSOR_HIGHSCORE;
				break;
			case Title::MENU::HIGHSCORE:
				select_menu = Title::MENU::PLAY;
				cursor_y = Title::CURSOR_PLAY;
				break;
			}
			cursor_pressed = true;
		}
	}else if (arduboy->pressed(UP_BUTTON)) {
		if (!cursor_pressed) {
			switch (select_menu) {
			case Title::MENU::PLAY:
				select_menu = Title::MENU::HIGHSCORE;
				cursor_y = Title::CURSOR_HIGHSCORE;
				break;
			case Title::MENU::MUSIC:
				select_menu = Title::MENU::PLAY;
				cursor_y = Title::CURSOR_PLAY;
				break;
			case Title::MENU::HIGHSCORE:
				select_menu = Title::MENU::MUSIC;
				cursor_y = Title::CURSOR_MUSIC;
				break;
			}
			cursor_pressed = true;
		}
	} else {
		cursor_pressed = false;
	}

	if (arduboy->pressed(A_BUTTON) || arduboy->pressed(B_BUTTON)) {
		if (!button_pressed) {
			switch (select_menu) {
			case Title::MENU::PLAY:
				flag = false;
				break;
			case Title::MENU::MUSIC:
				music = !music;
				if (music) {
					arduboy->tunes.tone(300, 300);
				}
				break;
			case Title::MENU::HIGHSCORE:
				showHighscore = !showHighscore;
				break;
			}
		}
		button_pressed = true;
	}
	else {
		button_pressed = false;
	}

	return flag;
}

void Title::draw()
{
	if (showHighscore) {
		for (int i = 0; i < Highscore::MEMORY_MAX; i++) {
			arduboy->setCursor(40, 10 + i * 10);
			sprintf(text, "%d : %04d", i + 1, highscore.readScore(i));
			arduboy->print(text);
		}
	}
	else {

		arduboy->drawSlowXYBitmap(22, 6, bitmap_title, 80, 16, 1);
		arduboy->setCursor(20, cursor_y);
		arduboy->print(">");
		arduboy->setCursor(35, Title::CURSOR_PLAY);
		arduboy->print("PLAY");
		arduboy->setCursor(35, Title::CURSOR_MUSIC);
		arduboy->print("MUSIC:");
		if (music) {
			arduboy->print("ON");
		}
		else {
			arduboy->print("OFF");
		}
		arduboy->setCursor(35, Title::CURSOR_HIGHSCORE);
		arduboy->print("HIGHSCORE");
	}
}


#endif // __TITLE_H__