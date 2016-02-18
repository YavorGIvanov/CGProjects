#ifndef CALLBACKS_H
#define CALLBACKS_H
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "cagdLib.h"

extern const size_t MAXDEGREE;
extern float backgroundColor[3];
extern bool movingAPoint;
extern bool addNewPoint;
extern bool delPoint;
extern bool smoothnessChange;
extern bool showControlPolygon;
extern bool showCurve;
extern bool showLowerDegrees;
extern int movingIndex;
extern int degreeLVL;
extern int thickness;
extern double smoothness;
extern std::vector<Point> controlPolygon;
extern std::vector<Point> bezierPoints;

/// In case of error output the description of it
void error_callback(int error, const char *description);

/// Set all the mouse button callbacks
void mouse_button_callback(GLFWwindow *window, int button, int action,
	int mods);

/// Set all the key button callbacks
void key_callback(GLFWwindow *window, int key, int scancode, int action,
	int mods);

#endif