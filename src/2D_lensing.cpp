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

void geodesic(double& d2t, double& d2r, double& d2phi, Ray ray, BlackHole SagA){
	double cs_t[2]; double cs_r[3]; double cs_phi[2];
	
	cs_t[0] = cs_t[1] = SagA.r_s / (2*ray.r*(ray.r - SagA.r_s));

	cs_r[0] = (SagA.r_s*(ray.r - SagA.r_s))/(2*ray.r*ray.r*ray.r);
	cs_r[1] = (SagA.r_s)/(2*ray.r*(SagA.r_s - ray.r));
	cs_r[2] = SagA.r_s - ray.r;

	cs_phi[0] = cs_phi[1] = 1 / ray.r;

	d2t = (-2)*cs_t[0]*ray.dt*ray.dr;
	d2r = (-1)*(cs_r[0]*ray.dt*ray.dt + cs_r[1]*ray.dr*ray.dr + cs_r[2]*ray.dphi*ray.dphi);
	d2phi = (-2)*cs_phi[0]*ray.dr*ray.dphi;
}

void runge_kutta4(double dlambda, Ray& ray, BlackHole SagA){
	double rk_t[4]; double rk_r[4]; double rk_phi[4];
	geodesic(rk_t[0], rk_r[0], rk_phi[0], ray, SagA);

	Ray temp = ray;
	temp.dt += rk_t[0] * dlambda/2;
	temp.dr += rk_r[0] * dlambda/2;
	temp.dphi += rk_phi[0] * dlambda/2;
	temp.t += ray.dt * dlambda/2;
	temp.r += ray.dr * dlambda/2;
	temp.phi += ray.dphi * dlambda/2;
	geodesic(rk_t[1], rk_r[1], rk_phi[1], temp, SagA);

	temp.dt += rk_t[1] * dlambda/2;
	temp.dr += rk_r[1] * dlambda/2;
	temp.dphi += rk_phi[1] * dlambda/2;
	temp.t += ray.dt * dlambda/2;
	temp.r += ray.dr * dlambda/2;
	temp.phi += ray.dphi * dlambda/2;
	geodesic(rk_t[2], rk_r[2], rk_phi[2], temp, SagA);

	temp.dt += rk_t[2] * dlambda;
	temp.dr += rk_r[2] * dlambda;
	temp.dphi += rk_phi[2] * dlambda;
	temp.t += ray.dt * dlambda;
	temp.r += ray.dr * dlambda;
	temp.phi += ray.dphi * dlambda;
	geodesic(rk_t[3], rk_r[3], rk_phi[3], temp, SagA);

	ray.dt += (dlambda/6) * (rk_t[0] + 2*rk_t[1] + 2*rk_t[2] + rk_t[3]);
	ray.dr += (dlambda/6) * (rk_r[0] + 2*rk_r[1] + 2*rk_r[2] + rk_r[3]);
	ray.dphi += (dlambda/6) * (rk_phi[0] + 2*rk_phi[1] + 2*rk_phi[2] + rk_phi[3]);

	ray.t += ray.dt * dlambda;
	ray.r += ray.dr * dlambda;
	ray.phi += ray.dphi * dlambda;
}

int main()
{
	RenderWindow window(VideoMode({800, 600}), "Window");
	BlackHole SagA(Vector2f(400.f, 300.f), 8.54e36);

	vector<Ray> rays;
	for(int i = 0; i <= 10; i++){
		Vector2f startPos(0.f, 50.f + i * 50.f);
		Vector2f vel(c_pixels, 0.f);
		rays.push_back(Ray(startPos, vel, SagA));
	}

	Clock clock;

	while(window.isOpen())
	{
		float dlambda = clock.restart().asSeconds()*100.f;
		while ( const optional event = window.pollEvent() )
	 	{
	 		if ( event->is<Event::Closed>() )
	 			window.close();
	 	}
		
		for(auto& ray : rays)
			ray.update(dlambda, SagA);

 		window.clear();

		SagA.draw(window);
		for(auto&ray : rays)
			ray.draw(window);

 		window.display();
	}
}
