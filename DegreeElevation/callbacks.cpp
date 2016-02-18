#include "callbacks.h"

void mouse_button_callback(GLFWwindow *window, int button, int action,
	int mods) {
	/// Left click is for adding a new point and draging a old one
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		Point cursorPos = getCursorPos(window);
		if (!movingAPoint) {
			float eps = 0.15;
			Point difference;
			for (int i = 0; i < controlPolygon.size() && !movingAPoint; i++) {
				difference = cursorPos - controlPolygon[i];
				if (fabs(difference.x) <= eps && fabs(difference.y) <= eps) {
					movingAPoint = true;
					movingIndex = i;
				}
			}
		}
		if (!movingAPoint) {
			addNewPoint = true;
			controlPolygon.push_back(cursorPos);
		}
	}
	/// If we have moved a point and now released the left button we stop the
	/// moving
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		if (movingAPoint) {
			movingAPoint = false;
			movingIndex = -1;
		}
	}
	/// The right button is used to delete a point.
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		if (!movingAPoint) {
			Point cursorPos = getCursorPos(window);
			Point difference;
			double eps = 0.15;
			for (int i = 0; i < controlPolygon.size() && !delPoint; i++) {
				difference = cursorPos - controlPolygon[i];
				if (fabs(difference.x) <= eps && fabs(difference.y) <= eps) {
					delPoint = true;
					controlPolygon.erase(controlPolygon.begin() + i);
				}
			}
		}
	}
}

/// Set all the key button callbacks
void key_callback(GLFWwindow *window, int key, int scancode, int action,
	int mods) {
	/// Escape is for quitting the program
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	/// Pressing the key 'C' clears the board
	else if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2],
			1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		bezierPoints.clear();
		controlPolygon.clear();
		degreeLVL = 0;
		addNewPoint = false;
		movingAPoint = false;
		delPoint = false;
		movingIndex = -1;
	}
	/// The Left and Right arrow are used for managing the thickness level
	else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		if (thickness < 6) {
			thickness++;
		}
	}
	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		if (thickness > 3) {
			thickness--;
		}
	}
	/// The numbers from 1 to 6 are used for setting the smoothness level
	else if (key >= 49 && key <= 54 && action == GLFW_PRESS) {
		smoothness = 0.05 / (key - 48);
		smoothnessChange = true;
	}
	/// The UP arrow is used for increasing the degree of the Bezier Curve
	else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		if (controlPolygon.size() > 2 && degreeLVL < 400) {
			degreeLVL++;
		}
	}
	/// The DOWN arrow is used for decreasing the degree of the Bezier Curve
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		if (controlPolygon.size() > 2 && degreeLVL > 0) {
			degreeLVL--;
		}
	}
	/// With Q we can hide/show the control Polygon
	else if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		showControlPolygon = !showControlPolygon;
	}
	/// With W we can hide/show the Bezier Curve
	else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		showCurve = !showCurve;
	}
	/// With E we hide all but the last Degree Elevation step
	else if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		showLowerDegrees = !showLowerDegrees;
	}
	/// With R we reset to the first Degree Elevation step
	else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		degreeLVL = 1;
	}
	/// With T we set the degree of elevation to the maximum
	else if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		degreeLVL = MAXDEGREE;
	}
	/// With B we set the background color to WHITE/BLACK
	else if (key == GLFW_KEY_B && action == GLFW_PRESS) {
		if (backgroundColor[0] == 1.0f) {
			backgroundColor[0] = 0.0f;
			backgroundColor[1] = 0.0f;
			backgroundColor[2] = 0.0f;
		}
		else {
			backgroundColor[0] = 1.0f;
			backgroundColor[1] = 1.0f;
			backgroundColor[2] = 1.0f;
		}
	}
}

void error_callback(int error, const char *description) {
	fputs(description, stderr);
}
