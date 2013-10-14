#ifndef MUSIC_MODLE_H
#define MUSIC_MODLE_H

#define MOVEMENT_MIN_THRESHOLD 30.f
#define MOVEMENT_MAX_THRESHOLD 100.f


#include "tools.h"

struct NoteInformation
{
	float quantity;
	float center;

	NoteInformation()
		: quantity(0.f), center(0.f)
	{};
};

class MusicModule
{
public:
	MusicModule(int numNotes);
	~MusicModule();

	void update(float deltaTime, const std::vector< std::vector<float> >& velocityInformation);
	std::vector<NoteInformation> getNoteInformations();

private:
	int notes;
	std::vector<NoteInformation> noteInformations;
};

#endif // MUSIC_MODLE_H
