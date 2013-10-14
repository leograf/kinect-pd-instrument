#include "DrawingModule.h"

void DrawingModule::drawImage(std::vector< std::vector<float> > velocityInfo, sf::Image& image) {
	image.create(width, height, sf::Color::Black);
	for (int w = 0; w < (int)velocityInfo.size(); w++) {
		for (int h = 0; h < (int)velocityInfo[w].size(); h++) {
			sf::Color color(sf::Color::Black);
			/*if (velocityInfo[w][h] > 100) {
			color.r = 255;
			}*/
			if (velocityInfo[w][h] < -50.0f) {
				color.g = (int)(255.0f*((velocityInfo[w][h] + 50.0f) / -VELOCITY_THRESHOLD));
			}

			// Draw pixel on image
			image.setPixel(w, h, color);
		}
	}
}

void DrawingModule::drawCenters(std::vector<NoteInformation> noteInformations, sf::RenderWindow& window) {
	int noteHeight = height / (int)noteInformations.size();

	sf::CircleShape shape(5);
	shape.setFillColor(sf::Color::Blue);

	for (int i = 0; i < (int)noteInformations.size(); i++) {
		int x = (int) (((float) noteInformations[i].center + 1.f) / 2.f * (float) width);		
		int y = noteHeight * i + (noteHeight / 2);
		shape.setPosition(x, y);

		if (noteInformations[i].quantity >= 0.0000001)
			shape.setFillColor(sf::Color::Red);
		else
			shape.setFillColor(sf::Color::Blue);

		window.draw(shape);
	}
}