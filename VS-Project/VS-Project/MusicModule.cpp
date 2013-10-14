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
	int width = (int) velocityInformation.size();


	float columnSum, sumVelocities, sumVelocitiesTimesPos, centerOfMass;
	for (int n = 0; n < notes; n++) {
		sumVelocities = 0.f;
		sumVelocitiesTimesPos = 0.f;
		centerOfMass = 0.f;

		for (int w = 0; w < width; w++) {
			columnSum = 0.f;

			for (int h = n * noteHeight; h < (n+1) * noteHeight; h++) {
				columnSum += abs(velocityInformation[w][h]);
				noteInformations[n].quantity += velocityInformation[w][h];
			}

			sumVelocities += columnSum;
			sumVelocitiesTimesPos += columnSum * (w+1);
		}
	
		// Average the quantity of the movement of the note.
		noteInformations[n].quantity = noteInformations[n].quantity / (width * noteHeight),

		// See http://en.wikipedia.org/wiki/Center_of_mass#A_system_of_particles on how this is calculated.
		centerOfMass = sumVelocitiesTimesPos / sumVelocities;
		// Map center to a value between -1 and 1
		centerOfMass = (centerOfMass / (int)velocityInformation.size()) * 2 - 1;
		noteInformations[n].center = centerOfMass;
	}
}

std::vector<NoteInformation> MusicModule::getNoteInformations()
{
	return noteInformations;
}

void MusicModule::send()
{

}