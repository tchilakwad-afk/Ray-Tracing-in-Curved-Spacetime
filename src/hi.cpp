#include <SFML/Graphics.hpp>
#include<vector>
#include<cmath>
using namespace std;
using namespace sf;
#define PI 3.14159265358979323846
//λ

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
	bool active;

	double r; double phi;
	double dr; double dphi;
	double d2r; double d2phi;

	Ray(Vector2f pos, Vector2f vel): position(pos), velocity(vel), active(true){
		this->r = sqrt((position.x-600.f)*(position.x-600.f) + (position.y-300.f)*(position.y-300.f))*scale;
		this->phi = atan2(position.y-300.f, position.x-600.f);

		dr = (cos(phi)*velocity.x + sin(phi)*velocity.y)*scale;
		dphi = (cos(phi)*velocity.y - sin(phi)*velocity.x)*scale/r;
	}

	void update(float dt, BlackHole SagA){
		if(!active) return;

		d2r = (SagA.r_s*dr*dr)/(2*r*(r - SagA.r_s)) + (r - SagA.r_s)*dphi*dphi;
		d2phi = -2.0*dr*dphi/r;

		dr += d2r*dt;
		dphi += d2phi*dt;
		r += dr*dt;
		phi += dphi*dt;

		position.x = r*cos(phi) / scale + 600.f;
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

int main()
{
	RenderWindow window(VideoMode({800, 600}), "Window");
	BlackHole SagA(Vector2f(600.f, 300.f), 8.54e36);

	vector<Ray> rays;
	for(int i = 0; i <= 10; i++){
		Vector2f startPos(0.f, 50.f + i * 50.f);
		Vector2f vel(c_pixels, 0.f);
		rays.push_back(Ray(startPos, vel));
	}

	Clock clock;

	while(window.isOpen())
	{
		float dt = clock.restart().asSeconds()*100.f;
		while ( const optional event = window.pollEvent() )
	 	{
	 		if ( event->is<Event::Closed>() )
	 			window.close();
	 	}
		
		for(auto& ray : rays)
			ray.update(dt, SagA);

 		window.clear();

		SagA.draw(window);
		for(auto&ray : rays)
			ray.draw(window);

 		window.display();
	}
}
