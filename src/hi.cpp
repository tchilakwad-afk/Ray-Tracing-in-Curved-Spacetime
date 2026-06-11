#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

int main()
{
	RenderWindow window(VideoMode({800, 600}), "Window");
	array line = 
	{
		Vertex{Vector2f(10.f, 10.f)},
		Vertex{Vector2f(790.f, 590.f)}
	};
	

	while(window.isOpen())
	{
		while ( const optional event = window.pollEvent() )
	 	{
	 		if ( event->is<Event::Closed>() )
	 			window.close();
	 	}

 	window.clear();
 	window.draw(line.data(), line.size(), PrimitiveType::Lines);
 	window.display();
	}
}
