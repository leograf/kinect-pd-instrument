#include "MusicModule.h"

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
	int noteHeight = height / notes;

	for (int n = 0; n < notes; n++) {
		float sumVelocities = 0.f;
		float sumVelocitiesTimesPos = 0.f;
		float centerOfMass = 0.f;

		for (int w = 0; w < width; w++) {
			float columnSum = 0.f;

			for (int h = n * noteHeight; h < (n+1) * noteHeight; h++) {
				float vel = abs(velocityInformation[w][h]);
				if (vel >= 50.f)
					columnSum += vel;
				//noteInformations[n].quantity += abs(velocityInformation[w][h]);

			}

			sumVelocities += columnSum;
			sumVelocitiesTimesPos += columnSum * (w+1);
		} 
	
		// Average the quantity of movement of the note.
		float fa = 0.4f;
		float newQuantity = sumVelocities / (float)(width * noteHeight);
		// Map the quantity to a value between 0 and ?.
		newQuantity = std::max(0.f, (newQuantity - MOVEMENT_MIN_THRESHOLD) / MOVEMENT_MAX_THRESHOLD);
		noteInformations[n].quantity = noteInformations[n].quantity*(1.0f - fa) + newQuantity*fa;

		// See http://en.wikipedia.org/wiki/Center_of_mass#A_system_of_particles on how this is calculated.
		centerOfMass = sumVelocitiesTimesPos / sumVelocities;

		// Map center to a value between -1 and 1		
		centerOfMass = (centerOfMass / (float) width) * 2.f - 1.f;

		//Boost the center of mass
		centerOfMass *= 1.5f;
		centerOfMass = std::max(-1.f, std::min(1.f, centerOfMass));

		noteInformations[n].center = centerOfMass;
	}
}

std::vector<NoteInformation> MusicModule::getNoteInformations()
{
	return noteInformations;
}