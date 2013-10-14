#include "MusicModule.h"

const float MusicModule::velocityThreshold = 50;

MusicModule::MusicModule(int numNotes)
: notes(numNotes)
{
	noteInformations = std::vector<NoteInformation>(notes);
}

MusicModule::~MusicModule()
{
}

void MusicModule::update(float deltaTime, const std::vector< std::vector<float> >& velocityInformation)
{
	int noteHeight = (int) velocityInformation[0].size() / notes;

	int note = 0;
	for (int n = 0; n < notes; n++) {
		for (int w = 0; w < (int)velocityInformation.size(); w++) {
			for (int h = n * noteHeight; h < (n+1) * noteHeight; h++) {
			}
		}

	}


	//int note = -1; // -1 because it will be changed to 0 on the first iteration of the loop.
	//float sumWeight = 0.f;
	//float sumWeightTimesCoord = 0.f;
	//for (int h = 0; h < (int) velocityInformation[0].size(); h++) {
	//	if ((h % noteHeight) == 0) {
	//		noteInformations[note].center = (sumWeightTimesCoord / ((float) velocityInformation.size()) / sumWeight;
	//		note++;
	//	}

	//	for (int w = 0; w < (int)velocityInformation.size(); w++) {
	//		noteInformations[note].movementValue += velocityInformation[w][h];
	//		sumWeight += velocityInformation[w][h];
	//		sumWeightTimesCoord += (w + 1) *  velocityInformation[w][h];
	//	}
	//}
}

void MusicModule::send()
{

}