#ifndef __SCENE_H__
#define __SCENE_H__

#include "Arduboy.h"
#include "bitmaps.h"

class Scene
{
protected:
	Arduboy* arduboy;

public:
	virtual bool run() = 0;
	virtual void draw() = 0;

};



#endif // __SCENE_H__