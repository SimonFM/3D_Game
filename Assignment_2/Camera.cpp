#include "maths_funcs.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
	// position
	vec3 position = vec3(0, 0, 5);
	// horizontal angle : toward -Z
	float horizontalAngle = 3.14f;
	// vertical angle : 0, look at the horizon
	float verticalAngle = 0.0f;
	 // Initial Field of View
	float initialFoV = 45.0f;
	
	float speed = 3.0f; // 3 units / second
	float mouseSpeed = 0.005f;
	int xpos, ypos;
	
};