#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

class EllipseShape : public Shape
{
	public :
	explicit EllipseShape(Vector2f radius = {0, 0}) : m_radius(radius)
	{
		update();
	}
	void setRadius(Vector2f radius)
	{
		m_radius = radius;
		update();
	}
	Vector2f getRadius() const
	{
		return m_radius;
	}
	size_t getPointCount() const override
	{
		return 30;
	}
	Vector2f getPoint(size_t index) const override
	{
		static constexpr float pi = 3.141592654f;

		float angle = index * 2 * pi / getPointCount() - pi / 2;
		float x = cos(angle) * m_radius.x;
		float y = sin(angle) * m_radius.y;

		return m_radius + Vector2f(x, y);
	}
	private:
	Vector2f m_radius;
};

int main()
{
	RenderWindow window(VideoMode({800, 600}), "Window");
	EllipseShape e({200.f, 150.f});
	e.setFillColor(Color::Cyan);

	while(window.isOpen())
	{
		while ( const optional event = window.pollEvent() )
	 	{
	 		if ( event->is<Event::Closed>() )
	 			window.close();
	 	}

 	window.clear();
 	window.draw(e);
 	window.display();
	}
}
