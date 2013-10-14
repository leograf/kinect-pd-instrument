#include "tools.h"

#include "PdComunication.h"
#include "KinectManager.h"
#include "DepthInformation.h"
#include "DrawingModule.h"
#include "MusicModule.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(width, height), "DT2300");
	sf::Texture texture;
	sf::Sprite sprite;

	sf::Image image;
	image.create(width, height, sf::Color::Black);

	KinectManager kinect;
	DepthInformation depthInf(width, height);
	MusicModule musicModule(10);

	// Matrix for the DepthInformation.
	std::vector< std::vector<unsigned short> > depthImage(width, std::vector<unsigned short>(height));

	DrawingModule drawing;
	PdComunication pd;

	sf::Clock clock;
	clock.restart();
	float deltaTime = 0.f;

	while (window.isOpen())
	{

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				window.close();
		}

		kinect.getKinectData(&depthImage);
		deltaTime = clock.restart().asSeconds();
		depthInf.update(deltaTime, depthImage);
		musicModule.update(deltaTime, depthInf.getVelocityInformation());

		drawing.drawImage(depthInf.getVelocityInformation(), image);

		pd.send(depthImage);

		texture.loadFromImage(image);
		sprite.setTexture(texture);

		window.clear();
		window.draw(sprite);
		drawing.drawCenters(musicModule.getNoteInformations(), window);
		window.display();
	}

	return 0;
}
