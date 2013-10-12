#include "DepthInformation.h"

#define VELOCITY_THRESHOLD 500.f

const int DepthInformation::BUFFER_SIZE = 5;

DepthInformation::DepthInformation(int w, int h)
: width(w), height(h), bufferPos(0)
{
	depthBuffer = std::vector< std::vector< std::vector<DepthPoint> > >(BUFFER_SIZE, std::vector<std::vector<DepthPoint> >(width, std::vector<DepthPoint>(height)));
	velocityFrame = std::vector< std::vector<float> >(width, std::vector<float>(height));
}

DepthInformation::~DepthInformation()
{
}


void DepthInformation::update(float deltaTime, const std::vector< std::vector<unsigned short> >& depthImage) {
	float vel, acc, avgVel, avgAcc, depthDiff;
	unsigned short depth;
	int nextBufferPos = (bufferPos + 1) % BUFFER_SIZE;
	for (int w = 0; w < width; w++) {
		for (int h = 0; h < height; h++) {
			depth = depthImage[w][h];
			if (depth == 0) {
				// Use old depth
				depth = depthBuffer[bufferPos][w][h].depth;
			}

			// Calculate veclocity and acceleration
			depthDiff = (float) (depth - depthBuffer[bufferPos][w][h].depth);
			if (depthDiff <= -VELOCITY_THRESHOLD)
				depthDiff = 0;
			vel = depthDiff / deltaTime;
			acc = (vel - depthBuffer[bufferPos][w][h].velocity) / deltaTime;

			// Update depth, veclocity and acceleration
			depthBuffer[nextBufferPos][w][h].depth = depth;
			depthBuffer[nextBufferPos][w][h].velocity = vel;
			depthBuffer[nextBufferPos][w][h].acceleration = acc;

			// Calculate average velocity and acceleration
			avgVel = 0.f;
			avgAcc = 0.f;

			for (int i = 0; i < BUFFER_SIZE; i++) {
				avgVel += depthBuffer[i][w][h].velocity;
				avgAcc += depthBuffer[i][w][h].acceleration;
			}

			avgVel /= (float)BUFFER_SIZE;
			avgAcc /= (float)BUFFER_SIZE;
			velocityFrame[w][h] = avgVel;
		}
	}

	// Update buffer position
	bufferPos = nextBufferPos;
}


std::vector< std::vector<float> > DepthInformation::getVelocityInformation() {
	return velocityFrame;
}
