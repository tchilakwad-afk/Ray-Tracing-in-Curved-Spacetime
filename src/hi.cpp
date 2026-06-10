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
	sf ::RenderWindow window( sf::VideoMode( { 800, 600 } ), "My window" );
	sf ::RenderTexture renderTexture({500, 500});
	sf ::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Magenta);

	renderTexture.clear();
	renderTexture.draw(shape);
	renderTexture.display();

	const sf::Texture& texture = renderTexture.getTexture();

	sf::Sprite sprite(texture);
	window.clear();
	window.draw(sprite);
	window.display();

}
