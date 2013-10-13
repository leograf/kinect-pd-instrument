#include "DrawingModule.h"

void DrawingModule::drawImage(std::vector< std::vector<float> > velocityInfo, sf::Image* image) {
	(*image).create(width, height, sf::Color::Black);
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
			(*image).setPixel(w, h, color);

		}
	}
}