#include "cagdLib.h"
#include "callbacks.h"
#include "globalVars.h"
#include <cmath>
#include <unordered_map>
/// We set the upper bound of the degree for elevating to 400

using std::vector;
using std::unordered_map;


/* De Casteljau Algorithm for generating a point from the Curve.
* r is the upper index of the point, i the lower and t the parameter
* which shows the ratio we will divide in. We use the memoization technique
* to reduce the number of recursive calls to the function. Store the already
* found points in the unordered_map foundPoints and avoid calculating them 
* more than once */
Point generatePoint(int r, int i, double t,
	unordered_map<std::pair<int, int>, Point> &foundPoints) {
	if (r == 0)
		return controlPolygon[i];

	/* In order to find the point with lower index i and upper r>0 first
	* we need to find b(i)(r-1) and b(i+1)(r-1) and then interpolate
	* between them with t -> b = (1-t)*b(i)(r-1) + t*b(i+1)(r-1)
	*/

	Point p1, p2;
	std::pair<int, int> index1, index2;
	index1 = std::make_pair(r - 1, i);
	index2 = std::make_pair(r - 1, i + 1);
	unordered_map<std::pair<int, int>, Point>::const_iterator iter1 =
		foundPoints.find(index1);
	unordered_map<std::pair<int, int>, Point>::const_iterator iter2 =
		foundPoints.find(index2);
	/* Check if we have already calculated the first point and if we
	* haven't than make a recursive call to the same function to find it.
	* If we have already calculated it just use it. */
	if (iter1 == foundPoints.end()) {
		p1 = generatePoint(r - 1, i, t, foundPoints);
		foundPoints.insert(std::make_pair(index1, p1));
	}
	else {
		p1 = iter1->second;
	}
	/* Check if we have already calculated the second point and if we
	* haven't than make a recursive call to the same function to find it.
	* If we have already calculated it just use it. */
	if (iter2 == foundPoints.end()) {
		p2 = generatePoint(r - 1, i + 1, t, foundPoints);
		foundPoints.insert(std::make_pair(index2, p2));
	}
	else {
		p2 = iter2->second;
	}

	/// Interpolate between the already found points
	Point current;

	current.x = (1 - t) * p1.x + t * p2.x;
	current.y = (1 - t) * p1.y + t * p2.y;

	return current;
}

/// Make the current point(pointed by movingIndex) follow the Cursor
void movePoint(GLFWwindow *window) {
	Point cursorPos = getCursorPos(window);
	controlPolygon[movingIndex] = cursorPos;
}

/// This function draw the points from the control Polygon
void drawControlPoints(size_t numControlPoits) {
	/// If the background is black -> white points, else -> black control points
	glColor3f(fabs(backgroundColor[0] - 0.9), fabs(backgroundColor[1] - 0.9),
		fabs(backgroundColor[2] - 0.9));
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(12);
	glBegin(GL_POINTS);
	for (int i = 0; i < numControlPoits; i++) {
		glVertex3f(controlPolygon[i].x, controlPolygon[i].y, 0);
	}
	glEnd();
}

/// This function draws the controlPolygon of the non-elevated Bezier curve
void drawControlPolygon(size_t numControlPoints) {
	glColor3f(0.0f, 0.0f, 1.0f);
	glLineWidth(6);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < numControlPoints; i++) {
		glVertex3f(controlPolygon[i].x, controlPolygon[i].y, 0);
	}
	glEnd();
}

/// This function finds the points from the Bezier curve and then draws it
void drawBezier(size_t numControlPoints) {
	/// We calculate the points from the curve only if some object
	/// was added, removed or changed
	if (addNewPoint || movingAPoint || delPoint || smoothnessChange) {
		bezierPoints.clear();
		if (numControlPoints > 2) {
			Point oldPoint = controlPolygon[0];
			bezierPoints.push_back(oldPoint);
			unordered_map<std::pair<int, int>, Point> found;
			for (double t = 0.0; t <= 1.0; t += smoothness) {
				/// numPoints-1 because the power of the curve is
				/// the number of the points minus one
				Point newPoint = generatePoint(numControlPoints - 1, 0, t, found);
				bezierPoints.push_back(newPoint);
				oldPoint = newPoint;
				found.clear();
			}
		}
		if (addNewPoint) {
			addNewPoint = false;
		}
		if (delPoint) {
			delPoint = false;
		}
		if (smoothnessChange) {
			smoothnessChange = false;
		}
	}
	/// Draw the curve with already calculated points. Stored in bezierPoints
	size_t numBezierPoints = bezierPoints.size();
	glLineWidth(thickness);
	glColor3f(1.0f, 0.0f, 0.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < numBezierPoints; i++) {
		glVertex3f(bezierPoints[i].x, bezierPoints[i].y, 0);
	}
	glEnd();
}

/* This function draws the polygon of the degree elevated Bezier curve.
* It also draws the new control points if we are below 100 degree. */
void drawNewPolygon() {
	double numPoints = controlPolygon.size();
	if (numPoints > 2) {
		glLineWidth(thickness);
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		vector<Point> lastPolygon(controlPolygon), currentPolygon;
		double coef;
		Point newPoint;
		/* We draw successively the points and polygons of the elevated
		* Bezier curve for each degree in the interval (0...degreeLVL] */
		for (size_t i = 0; i < degreeLVL; i++) {
			/// for j = 0 we always get the first point of the last polygon
			currentPolygon.push_back(lastPolygon[0]);

			/// In the for below we calculate the points of the new polygon
			for (size_t j = 1; j < numPoints; j++) {
				coef = j / numPoints;
				/* Interpolate between the j-1 and j point from the last drawn polygon
				* with coef = j/size, where j is the lower index of the current point
				* and numPoints is the number of points from the control
				* polygon of
				* the new curve */
				newPoint = lastPolygon[j - 1] * coef + lastPolygon[j] * (1 - coef);
				currentPolygon.push_back(newPoint);
			}
			/// for j = n+1 we always get the last point of the last polygon
			currentPolygon.push_back(lastPolygon.back());
			/// Always increment the number of Points
			numPoints++;

			/// Draw the control polygon of the elevated curve
			if (showLowerDegrees || i == degreeLVL - 1) {
				glColor3f(0.1, 0.6, 0.3);
				glBegin(GL_LINE_STRIP);
				for (size_t j = 0; j < numPoints; j++) {
					glVertex3f(currentPolygon[j].x, currentPolygon[j].y, 0);
				}
				glEnd();

				/// We stop drawing the points when we reach degree 100
				if (i < 100 && i == degreeLVL - 1) {
					glPointSize(7.5 * thickness / 3);
					glColor3f(0.2, 1.0, 0.2);
					glBegin(GL_POINTS);
					for (size_t j = 0; j < numPoints; j++) {
						glVertex3f(currentPolygon[j].x, currentPolygon[j].y, 0);
					}
					glEnd();
				}
			}
			/* Save the last polygon so we can use it in the calculations
			* of the points from the next degree */
			lastPolygon = currentPolygon;
			currentPolygon.clear();
		}
	}
}

/* We use this method to draw everything in the output screen.
* It is separated in a couple more  functions for the different
* parts of the output */
void drawOutput() {

	size_t numControlPoints = controlPolygon.size();
	/// Draw the Bezier Curve
	if (showCurve) {
		drawBezier(numControlPoints);
	}
	/// Draw the control Polygon
	if (showControlPolygon) {
		drawControlPolygon(numControlPoints);
	}
	/// Draw the Points from the control polygon
	if (showControlPolygon) {
		drawControlPoints(numControlPoints);
	}
	/// Draw the elevated polygon
	drawNewPolygon();
}

int main() {
	// init window
	GLFWwindow *window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(800, 600, "Degree Elevation", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	// Set callbacks for the mouse and keyboard
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);
	// end of initiation
	printControls();
	// start Outputing
	while (!glfwWindowShouldClose(window)) {
		reposition(window);
		
		if (movingAPoint) {
			movePoint(window);
		}

		drawOutput(); ///< draw the current Output

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// End outputting
	finit(window);
}