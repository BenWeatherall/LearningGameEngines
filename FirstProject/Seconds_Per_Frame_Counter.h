#pragma once

#include <cstdio>
#include <GLFW/glfw3.h>

class SPF_Counter {
public:
	SPF_Counter();
	void tick();
private:
	double currentTime;
	double lastTime;
	int nbFrames;

};
