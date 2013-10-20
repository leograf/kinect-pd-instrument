#include "tools.h"

#include "PdComunication.h"
#include "KinectManager.h"
#include "DepthInformation.h"
#include "DrawingModule.h"
#include "MusicModule.h"

/*
 * Kinect puredata music instrument by Ferran Bertomeu Castells & Leonard Graf
 *
 * Source code avaliable at https://github.com/leograf/kinect-pd-instrument
 *
 * Its indended to work with the "instrument.pd" found in the repository
 */

int main()
{
	// SFML init
	sf::RenderWindow window(sf::VideoMode(width, height), "DT2300", sf::Style::Default);
	sf::Texture texture;
	sf::Sprite sprite;
	// Image to render in screen
	sf::Image image;
	image.create(width, height, sf::Color::Black);

	// Matrix for the DepthInformation.
	std::vector< std::vector<unsigned short> > depthImage(width, std::vector<unsigned short>(height));

	// Modules and classes init
	KinectManager kinect;
	DepthInformation depthInf(width, height);
	MusicModule musicModule(10);
	DrawingModule drawing;
	PdComunication pd;
	tools::Chord status; // Chord status of the "instrument.pd"
	bool statusChanged = false;
		status = tools::Chord::CminArp;
		pd.changeStatus(status);

	sf::Clock clock;
	clock.restart();
	float deltaTime = 0.f;

	// Main Loop
	while (window.isOpen())
	{
		statusChanged = false;
		// Event polling
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

		// Update modules
		deltaTime = clock.restart().asSeconds();

		kinect.getKinectData(&depthImage);
		depthInf.update(deltaTime, depthImage);
		musicModule.update(deltaTime, depthInf.getVelocityInformation());
		drawing.drawImage(depthInf.getVelocityInformation(), image);

		// Send info to pd
		pd.send(musicModule.getNoteInformations());
		if (statusChanged)
			pd.changeStatus(status);

		// Drawing
		texture.loadFromImage(image);
		sprite.setTexture(texture);

		window.clear();
		window.draw(sprite);
		drawing.drawCenters(musicModule.getNoteInformations(), window);
		window.display();
	}

	return 0;
}
