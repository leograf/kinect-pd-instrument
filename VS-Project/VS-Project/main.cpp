#include "tools.h"

#include "PdComunication.h"
#include "KinectManager.h"
#include "DepthInformation.h"
#include "DrawingModule.h"
#include "MusicModule.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(width, height), "DT2300", sf::Style::Default);
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
	tools::Chord status = tools::Chord::CminArp;
	pd.changeStatus(status);

	sf::Clock clock;
	clock.restart();
	float deltaTime = 0.f;
	bool statusChanged = false;

	while (window.isOpen())
	{
		statusChanged = false;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
				else if (event.key.code == sf::Keyboard::Num1) {
					statusChanged = true;
					status = tools::Chord::CminArp;
				}
				else if (event.key.code == sf::Keyboard::Num2) {
					statusChanged = true;
					status = tools::Chord::CmajArp;
				}
				else if (event.key.code == sf::Keyboard::Num3) {
					statusChanged = true;
					status = tools::Chord::Cmaj;
				}
				else if (event.key.code == sf::Keyboard::Num4) {
					statusChanged = true;
					status = tools::Chord::C3rds;
				}
				else if (event.key.code == sf::Keyboard::Num5) {
					statusChanged = true;
					status = tools::Chord::CJazz;
				}
			}
		}

		kinect.getKinectData(&depthImage);
		deltaTime = clock.restart().asSeconds();
		depthInf.update(deltaTime, depthImage);
		musicModule.update(deltaTime, depthInf.getVelocityInformation());

		drawing.drawImage(depthInf.getVelocityInformation(), image);

		pd.send(musicModule.getNoteInformations());
		if (statusChanged)
			pd.changeStatus(status);

		texture.loadFromImage(image);
		sprite.setTexture(texture);

		window.clear();
		window.draw(sprite);
		drawing.drawCenters(musicModule.getNoteInformations(), window);
		window.display();
	}

	return 0;
}
