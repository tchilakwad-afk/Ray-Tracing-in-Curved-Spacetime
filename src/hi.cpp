#include <SFML/Graphics.hpp>

int main()
{
	sf ::RenderWindow window(sf::VideoMode({800, 600}), "Window");
	sf::RectangleShape line({150.f, 5.f});

	line.rotate(sf:: degrees(45));

	while(window.isOpen())
	{
		while ( const std::optional event = window.pollEvent() )
	 	{
	 		if ( event->is<sf::Event::Closed>() )
	 			window.close();
	 	}

 	window.clear();
 	window.draw(line);
 	window.display();
	}
}
