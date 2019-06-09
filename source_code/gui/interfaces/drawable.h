#pragma once

#include "scene_2d.h"

class drawable
{
public:
	virtual void draw(scene_2d& scene) = 0;
	virtual ~drawable(){}
};