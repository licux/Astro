
#include "Arduboy.h"
#include "Scene.h"
#include "Title.h"
#include "Stage.h"

#include "Gameover.h"
#include "Global.h"

enum SCENE {
	TITLE,
	STAGE,
	GAMEOVER,
};

Arduboy arduboy;
Scene* scene;
SCENE now_scene;

void main_setup()
{
	arduboy.begin();
	arduboy.setFrameRate(60);
	arduboy.clear();
	arduboy.print("Hello World!\n");
	scene = new Title(&arduboy);
	now_scene = SCENE::TITLE;
}

void main_loop()
{
	if (!(arduboy.nextFrame()))
		return;

	if (!scene->run()) {
		delete scene;
		switch (now_scene) {
		case SCENE::TITLE:
			now_scene = SCENE::STAGE;
			scene = new Stage(&arduboy);
			break;
		case SCENE::STAGE:
			now_scene = SCENE::GAMEOVER;
			scene = new Gameover(&arduboy);
			break;
		case SCENE::GAMEOVER:
			now_scene = SCENE::TITLE;
			scene = new Title(&arduboy);
			break;
		}
	}

	arduboy.clear();
	scene->draw();
	arduboy.display();
}