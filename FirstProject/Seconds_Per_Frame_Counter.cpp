#include "Seconds_Per_Frame_Counter.h"

SPF_Counter::SPF_Counter()
{
	this->lastTime = glfwGetTime();
	this->nbFrames = 0;
}

void SPF_Counter::tick()
{
	// Measure speed (http://www.opengl-tutorial.org/miscellaneous/an-fps-counter/)
	currentTime = glfwGetTime();
	nbFrames++;
	if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago printf and reset timer
		printf("%f ms/frame\n", 1000.0 / double(nbFrames));
		nbFrames = 0;
		lastTime += 1.0;
	}
}

