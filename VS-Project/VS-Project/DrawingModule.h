#ifndef DRAWING_MODULE_H
#define DRAWING_MODULE_H

#include "tools.h"
#include "MusicModule.h"

class DrawingModule {
public:
	void drawImage(std::vector< std::vector<float> > velocityInfo, sf::Image& image);
	void drawCenters(std::vector<NoteInformation> noteInformations, sf::RenderWindow& window);

private:

};

#endif // DRAWING_MODULE_H