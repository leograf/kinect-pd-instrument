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
	int noteWidth = width / notes;

	// We divide the screen in *notes* columns so that we calculate the average speeds of the pixels
	// on each column separately
	// We also calcule the gravity of the pixels in the Y axis of the image so that we can know wheter
	// the movement is being done higher or lower

	for (int n = 0; n < notes; n++) {
		float sumVelocities = 0.f;
		float sumVelocitiesTimesPos = 0.f;
		float centerOfMass = 0.f;

		for (int h = 0; h < height; h++) {
			float rowSum = 0.f;

			for (int w = n * noteWidth; w < (n + 1) * noteWidth; w++) {
				float vel = abs(velocityInformation[w][h]);
				if (vel >= 50.f)
					rowSum += vel;

			}

			sumVelocities += rowSum;
			sumVelocitiesTimesPos += rowSum * (h + 1);
		} 
	
		// Average the quantity of movement of the note.
		float fa = 0.4f;
		float newQuantity = sumVelocities / (float)(height * noteWidth);
		// Map the quantity to a value between 0 and ?.
		newQuantity = std::max(0.f, (newQuantity - MOVEMENT_MIN_THRESHOLD) / MOVEMENT_MAX_THRESHOLD);
		noteInformations[n].quantity = noteInformations[n].quantity*(1.0f - fa) + newQuantity*fa;

		// See http://en.wikipedia.org/wiki/Center_of_mass#A_system_of_particles on how the gravity is calculated.
		centerOfMass = sumVelocitiesTimesPos / sumVelocities;

		// Map center to a value between -1 and 1		
		centerOfMass = (centerOfMass / (float) height) * 2.f - 1.f;

		//Boost the center of mass (After testing we ended up boosting this to get better results)
		centerOfMass *= 1.5f;
		centerOfMass = std::max(-1.f, std::min(1.f, centerOfMass));

		noteInformations[n].center = centerOfMass;
	}
}

std::vector<NoteInformation> MusicModule::getNoteInformations()
{
	return noteInformations;
}