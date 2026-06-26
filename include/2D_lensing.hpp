#pragma once

#include <SFML/Graphics.hpp>
#include<vector>
#include<cmath>


using namespace std;
using namespace sf;

#define PI 3.14159265358979323846

double c = 299792458.0;
double G = 6.67430e-11;
double scale = 1e8;
double c_pixels = c / scale;

struct Ray;
struct BlackHole;
void geodesic(double& d2t, double& d2r, double& d2phi, Ray ray, BlackHole SagA);
void runge_kutta4(double dlambda, Ray& ray, BlackHole SagA);

struct BlackHole{
	Vector2f position;
	double mass;
	double r_s;

	BlackHole(Vector2f pos, float m);

	void draw(RenderWindow& window);
};

struct Ray{
	Vector2f position;
	Vector2f velocity;
	vector<Vertex> trail;
	float currentTime = 0.0f;
	bool active;

	double r; double phi; double t;
	double dr; double dphi; double dt;

	Ray(Vector2f pos, Vector2f vel, BlackHole SagA);

	void update(float dlambda, BlackHole SagA);

	void draw(RenderWindow& window);
};