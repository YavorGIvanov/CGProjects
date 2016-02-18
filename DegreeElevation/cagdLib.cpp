#include <stdlib.h>
#include <stdio.h>
#include "cagdLib.h"

/* We implement the operations assingment, sum, difference
* and multiplication with a scalar for the points */
Point &Point::operator=(const Point &other) {
  if (this != &other) {
    x = other.x;
    y = other.y;
  }
  return *this;
}
Point &Point::operator+=(const Point &other) {
  x += other.x;
  y += other.y;
  return *this;
}
Point &Point::operator-=(const Point &other) {
  x -= other.x;
  y -= other.y;
  return *this;
}
Point &Point::operator*=(float scalar) {
  x *= scalar;
  y *= scalar;
  return *this;
}

Point operator+(Point left, const Point &right) {
  left += right;
  return left;
}
Point operator-(Point left, const Point &right) {
  left -= right;
  return left;
}
Point operator*(Point current, double scalar) {
  current *= scalar;
  return current;
}

float random(float a, float b)
{
	return (rand() / (float)RAND_MAX)*(b - a) + a;
}


Point getCursorPos(GLFWwindow* window){
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	float ratio;
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / (float)height;
	ratio *= 5;
	x = x * ratio / (width / 2) - ratio;
	y = y * 5.0 / (height / 2) - 5.0;
	y *= -1;
	Point cursorPos;
	cursorPos.x = x;
	cursorPos.y = y;
	return cursorPos;
}

void reposition(GLFWwindow *window) {
	float ratio;
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / (float)height;
	glViewport(0, 0, width, height);
	glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio * 5, ratio * 5, -5.f, 5.f, 5.f, -5.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT + GL_DEPTH_BUFFER_BIT);
}

void finit(GLFWwindow *window) {
	 glfwDestroyWindow(window);
	 glfwTerminate();
	 exit(EXIT_SUCCESS);
 }

void printControls(){
	printf("Left Click -> Draws Point\n");
	printf("Left Click and Drag -> Moves Point\n");
	printf("Right Click -> Deletes Point\n");
	printf("Up Arrow -> Increases Degree\n");
	printf("Down Arrow -> Decreases Degree\n");
	printf("Right Arrow -> Increases size of lines\n");
	printf("Left Arrow -> Decreases size of lines\n");
	printf("Key 'Q' -> Show/Hide Control Polygon and Control Points\n");
	printf("Key 'W' -> Show/Hide Control Bezier Curve \n");
	printf("Key 'E' -> Show/Hide Control Polygons of Intermidiate Degree of the Curve \n");
	printf("Key 'R' -> Resets the degree elevation to 1\n");
	printf("Key 'T' -> Sets the degree elevation to the Maximum(currently 400)\n");
	printf("Key 'B' -> Sets the background to BLACK/WHITE\n");
	printf("Key 'C' -> Clears the screen\n");
	printf("Key '1' to Key '6' -> Regulate the smoothness of the Bezier curve\nDefault value of smoothness : 2\n");
}