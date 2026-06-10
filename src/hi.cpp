#include <SFML/Graphics.hpp>

int main()
{
	sf ::RenderWindow window(sf::VideoMode({800, 600}), "Window");
	sf:: CircleShape circle(200.f);
	circle.setRadius(40.f);
	circle.setPointCount(3); //using a circle command and changing no. of sides to obtain necessary shape
	circle.setFillColor(sf::Color::Cyan);

	while(window.isOpen())
	{
		while ( const std::optional event = window.pollEvent() )
	 	{
	 		if ( event->is<sf::Event::Closed>() )
	 			window.close();
	 	}

 	window.clear();
 	window.draw(circle);
 	window.display();
	}
}
