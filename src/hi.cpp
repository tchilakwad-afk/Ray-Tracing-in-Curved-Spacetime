#include <SFML/Graphics.hpp>

int main()
{
	sf ::RenderWindow window(sf::VideoMode({800, 600}), "Window");
	sf ::ConvexShape convex;

	convex.setPointCount(5);
	convex.setPoint(0, {50.f, 50.f});
	convex.setPoint(1, {200.f, 60.f});
	convex.setPoint(2, {170.f, 140.f});
	convex.setPoint(3, {80.f, 150.f});
	convex.setPoint(4, {50.f, 100.f});
	convex.setFillColor(sf::Color(100, 12, 100));

	while(window.isOpen())
	{
		while ( const std::optional event = window.pollEvent() )
	 	{
	 		if ( event->is<sf::Event::Closed>() )
	 			window.close();
	 	}

 	window.clear();
 	window.draw(convex);
 	window.display();
	}
}
