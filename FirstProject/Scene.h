#pragma once
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<string>
#include<vector>

#include "StaticMesh.h"
// #include "SceneLoader.h"

class Scene {
public:
	friend class SceneLoader;

	Scene(std::string scene_file);
	~Scene();
	
	bool attachStatic(std::string);
	
	void removeStatic(StaticMesh*); // Chance to Complex in the long run
	void draw();
	void tick(GLfloat delta); // Update All Actors

	// TODO
	// bool attachActor();
	// uint64_t attachLight();
	// void removeActor(Actor*);
	// void removeLight();
	// void setSkybox(void); // Pass texture?
private:
	std::string scene_name;
	std::vector<StaticMesh*>* statics;

	// TODO: Create the following
	// SkyBox
	// Ambient Lighting
	// Lights Vector
	// std::vector<ComplexMesh> Static;
	// std::vector<Actor> Actors;
	// Actors Vector
		// Complex shapes with Update()
	// AnimationTimeline
		// Per Actor Animation
		// Interpolate between initial rotation and final rotation
		// Determine change per tick
		// Allow animation layering?
	// Height map()?
};