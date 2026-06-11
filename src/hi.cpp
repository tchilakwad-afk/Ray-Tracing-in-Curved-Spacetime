#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

int main()
{
	RenderWindow window(VideoMode({800, 600}), "Window");
	RectangleShape rect({150.f, 200.f});
	rect.setFillColor(Color::Magenta);

	while(window.isOpen())
	{
		while ( const optional event = window.pollEvent() )
	 	{
	 		if ( event->is<Event::Closed>() )
	 			window.close();
	 	}

 	window.clear();
 	window.draw(rect);
 	window.display();
	}
}
