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

struct BlackHole{
	Vector2f position;
	double mass;
	double r_s;

	BlackHole(Vector2f pos, float m) : position(pos), mass(m){
		r_s = 2.0 * G * mass / (c*c);
	}

	void draw(RenderWindow& window){
		CircleShape circle(r_s / scale);
		circle.setFillColor(Color::Red);
		circle.setPosition(position - Vector2f(r_s/scale, r_s/scale));
		
		window.draw(circle);
	}
};

struct Ray{
	Vector2f position;
	Vector2f velocity;
	vector<Vertex> trail;
	float currentTime = 0.0f;
	bool active;

	double r; double phi; double t;
	double dr; double dphi; double dt;

	Ray(Vector2f pos, Vector2f vel, BlackHole SagA): position(pos), velocity(vel), active(true){
		this->r = sqrt((position.x-400.f)*(position.x-400.f) + (position.y-300.f)*(position.y-300.f))*scale;
		this->phi = atan2(position.y-300.f, position.x-400.f);
		t = 0.0;

		dr = (cos(phi)*velocity.x + sin(phi)*velocity.y)*scale;
		dphi = (cos(phi)*velocity.y - sin(phi)*velocity.x)*scale/r;
		dt = sqrt((dr*dr/(1-SagA.r_s/r) + r*r*dphi*dphi)/(1-SagA.r_s/r))/c;
	}

	void update(float dlambda, BlackHole SagA){
		if(!active) return;

		runge_kutta4(dlambda, *this, SagA);

		position.x = r*cos(phi) / scale + 400.f;
		position.y = r*sin(phi) / scale + 300.f;

		trail.push_back(Vertex(position, Color(225, 255, 0)));

		if(position.x < 0 || position.x > 800 || position.y < 0 || position.y > 600)
			active = false;
		if(r < SagA.r_s)
			active = false;
	}

	void draw(RenderWindow& window){
		if(trail.size() >= 2)
			window.draw(trail.data(), trail.size(), PrimitiveType::LineStrip);
	}
};