#pragma once
#include "Actor.h"

class Scene {
public:
	Scene(std::string SceneCSV);
	~Scene();
	uint64_t attachActor();
	uint64_t attachStatic();
	uint64_t attachLight();
	
	void removeActor(uint64_t);
	void removeStatic(uint64_t);
	void removeLight(uint64_t);

	void setSkybox(void); // Pass texture?
	
	void draw();
	void tick(GLfloat delta); // Update All Actors
private:
	// TODO: Create the following
	// SkyBox
	// Ambient Lighting
	// Lights Vector
	/** DO AS MAP **/
	// Static Mesh Vector
		// Complex shapes with no Update() functionality
	// Actors Vector
		// Complex shapes with Update()
	// AnimationTimeline
		// Per Actor Animation
		// Interpolate between initial rotation and final rotation
		// Determine change per tick
		// Allow animation layering?
	// Height map()?
};