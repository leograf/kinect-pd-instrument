#ifndef MUSIC_MODLE_H
#define MUSIC_MODLE_H

#include <vector>
#include "PdComunication.h"

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
	void send();

	static const float velocityThreshold;

	int notes;
	std::vector<NoteInformation> noteInformations;
};

#endif // MUSIC_MODLE_H
