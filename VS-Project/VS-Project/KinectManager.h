#ifndef KINECT_MANAGER_H
#define KINECT_MANAGER_H

#include "tools.h"

#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>

class KinectManager {
public:
	KinectManager();
	~KinectManager();

	void getKinectData(std::vector< std::vector<unsigned short> >* depthImage);

private:
	// Kinect variables
	INuiSensor* sensor;
	HANDLE depthStream;

	bool initKinect();
};

#endif //KINECT_MANAGER_H