#include <SFML/Graphics.hpp>

int main()
{
	// sf::RenderWindow window( sf::VideoMode( { 800, 600 } ), "Hello world!" );
	// sf::CircleShape shape( 100.f );
	// shape.setFillColor( sf::Color::Blue );

	// while ( window.isOpen() )
	// {
	// 	while ( const std::optional event = window.pollEvent() )
	// 	{
	// 		if ( event->is<sf::Event::Closed>() )
	// 			window.close();
	// 	}

	// 	window.clear();
	// 	window.draw( shape );
	// 	window.display();
	// }
	sf ::RenderWindow window(sf::VideoMode({800, 600}), "Window");
	sf ::RectangleShape rectangle({120.f, 50.f});
	rectangle.setFillColor(sf::Color::Cyan);
	rectangle.setOutlineThickness(-12.f);
	rectangle.setOutlineColor(sf::Color::Magenta);

	while(window.isOpen())
	{
		while ( const std::optional event = window.pollEvent() )
	 	{
	 		if ( event->is<sf::Event::Closed>() )
	 			window.close();
	 	}

 	window.clear();
 	window.draw( rectangle );
 	window.display();
	}
}
