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
	bool active;

	Ray(Vector2f pos, Vector2f vel): position(pos), velocity(vel), active(true){}

	void update(float dt){
		if(!active) return;
		position += velocity * dt;
		trail.push_back(Vertex(position, Color(225, 255, 0)));

		if(position.x < 0 || position.x > 800 || position.y < 0 || position.y > 600)
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
			ray.update(dt);

 		window.clear();

		SagA.draw(window);
		for(auto&ray : rays)
			ray.draw(window);

 		window.display();
	}
}
