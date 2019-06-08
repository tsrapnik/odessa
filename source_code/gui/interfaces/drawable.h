#pragma once

#include "vc_gpu.h"

class drawable
{
public:
	virtual void draw(vc_gpu& a_vc_gpu) = 0;
};