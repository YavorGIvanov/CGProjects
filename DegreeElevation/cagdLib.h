#ifndef CAGDLIB_H_INCLUDED
#define CAGDLIB_H_INCLUDED
#include <GLFW/glfw3.h>
#include <utility>


extern float backgroundColor[3];

///The struct Point represents a 2D point
struct Point {
	float x, y;
	/* We implement the operations assingment, sum, difference
	* and multiplication with a scalar for the points */
	Point &operator=(const Point &other);
	Point &operator+=(const Point& other);
	Point &operator-=(const Point &other);
	Point &operator*=(float scalar);
};
Point operator+(Point left, const Point& right);
Point operator-(Point left, const Point& right);
Point operator*(Point current, double scalar);

///Generates a random float in the interval [a,b]
float random(float a, float b);

///Returns the cursor position coordinates stored in a point
Point getCursorPos(GLFWwindow* window);

///Transformations of the window on every iteration of the cycle in main
void reposition(GLFWwindow *window);

/// Finish the outputing and destroy the window
void finit(GLFWwindow *window);

void printControls();

/// Implement hash for std::pair
template <class T>
inline void hash_combine(std::size_t &seed, const T &v) {
	std::hash<T> hasher;
	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace std {
	template <typename S, typename T> struct hash<pair<S, T>> {
		inline size_t operator()(const pair<S, T> &v) const {
			size_t seed = 0;
			::hash_combine(seed, v.first);
			::hash_combine(seed, v.second);
			return seed;
		}
	};
}

#endif // CAGDLIB_H_INCLUDED