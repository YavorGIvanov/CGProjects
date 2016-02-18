#ifndef GLOBALVARS_H
#define GLOBALVARS_H
#include <vector>
#include "cagdLib.h"

std::vector<Point>
controlPolygon; ///< contains the points from the controlPolygon
std::vector<Point>
bezierPoints; ///< containt the points of the Bezier Curve
float backgroundColor[3] = { 1.0f, 1.0f, 1.0f }; ///< holds the current background Color
const size_t MAXDEGREE = 400;    ///< holds the maximum degree elevation leve
bool addNewPoint = false;        ///< keeps track if we just added a new point
bool movingAPoint = false;       ///< keeps track if we are moving a point
bool delPoint = false;           ///< keeps track if we just deleted a point
int movingIndex = -1;            ///< holds the index to the point we move
int degreeLVL = 0;               ///< holds the current degree Level
double smoothness = 0.02;        ///< holds the current smoothness level
int thickness = 4;               ///< holds the current thickness level

/// keeps track if we just changed the smoothness
bool smoothnessChange = false;
/// true -> show the bezier curve, false-> don't
bool showCurve = true;
/// true -> show the control polygon, false-> don't
bool showControlPolygon = true;
/// true -> show the intermidiate steps of the degrees, false -> just show the
/// last
bool showLowerDegrees = true;

#endif