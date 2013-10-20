#include "DepthInformation.h"


DepthInformation::DepthInformation(int w, int h)
: width(w), height(h)
{
	depthBuffer = std::vector<std::vector<DepthPoint> >(width, std::vector<DepthPoint>(height));
	velocityFrame = std::vector< std::vector<float> >(width, std::vector<float>(height));
}

DepthInformation::~DepthInformation()
{
}

void DepthInformation::update(float deltaTime, const std::vector< std::vector<unsigned short> >& depthImage) {
	float vel, acc, depthDiff;
	unsigned short depth;

	for (int w = 0; w < width; w++) {
		for (int h = 0; h < height; h++) {
			depth = depthImage[w][h];
			if (depth == 0) {
				// Use old depth
				depth = depthBuffer[w][h].depth;
			}

			// Calculate veclocity and acceleration
			depthDiff = (float) (depth - depthBuffer[w][h].depth);
			if (depthDiff <= -VELOCITY_THRESHOLD) {
				depthDiff = 0;
			}
			vel = depthDiff / deltaTime;
			acc = (vel - depthBuffer[w][h].velocity) / deltaTime;

			// Update depth, veclocity and acceleration
			float fa = 0.31f;
			depthBuffer[w][h].depth = depth;
			depthBuffer[w][h].velocity = depthBuffer[w][h].velocity*(1.f - fa) + vel*fa;
			depthBuffer[w][h].acceleration = depthBuffer[w][h].acceleration*(1.f - fa) + acc*fa;

			velocityFrame[w][h] = (depthBuffer[w][h].velocity > 0) ? 0 : depthBuffer[w][h].velocity; // +0.5f*depthBuffer[w][h].acceleration*deltaTime;
			// Here we could predict the next frame by using the acceleration, since kinect does not have a
			// great framerate we could always try to predict the next depth velocity using the acceleration.
			// After some testing we decided not to use it because it augments the noise
		}
	}
}


std::vector< std::vector<float> > DepthInformation::getVelocityInformation() {
	return velocityFrame;
}
