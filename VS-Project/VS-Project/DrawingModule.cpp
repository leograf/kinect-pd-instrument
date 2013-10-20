#include "DrawingModule.h"

void DrawingModule::drawImage(std::vector< std::vector<float> > velocityInfo, sf::Image& image) {
	image.create(width, height, sf::Color::Black);

	float minDrawingVelocity = -50.0f; // Minimum velocity threshold to actually draw the pixel on green scale

	for (int w = 0; w < (int)velocityInfo.size(); w++) {
		for (int h = 0; h < (int)velocityInfo[w].size(); h++) {
			sf::Color color(sf::Color::Black);

			if (velocityInfo[w][h] < minDrawingVelocity) {
				color.g = (int)(255.0f*((velocityInfo[w][h] - minDrawingVelocity) / -VELOCITY_THRESHOLD));
			}

			// Draw pixel on image
			image.setPixel(w, h, color);
		}
	}
}

void DrawingModule::drawCenters(std::vector<NoteInformation> noteInformations, sf::RenderWindow& window) {
	int noteWidth = width / (int)noteInformations.size();

	sf::CircleShape shape(5);
	shape.setFillColor(sf::Color::Blue);

	for (int i = 0; i < (int)noteInformations.size(); i++) {
		int x = noteWidth * i + (noteWidth / 2);
		int y = (int)(((float)noteInformations[i].center + 1.f) / 2.f * (float)height);
		float scale = std::min(2.0f, noteInformations[i].quantity) * noteWidth/2.f + 1;
		
		shape.setRadius(scale);
		shape.setPosition(x - shape.getRadius(), y - shape.getRadius());
		
		if (noteInformations[i].quantity >= 0.0000001) // We dont want 0's
			shape.setFillColor(sf::Color::Red);
		else
			shape.setFillColor(sf::Color::Blue);

		window.draw(shape);
	}
}