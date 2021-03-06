#include "KinectManager.h"

KinectManager::KinectManager() {
	
	if (!initKinect()) {
		std::cout << "No Kinect found!" << std::endl;
	}
}

KinectManager::~KinectManager() {
	NuiShutdown();
}

bool KinectManager::initKinect() {
	// Get a working kinect sensor
	int numSensors;
	if (NuiGetSensorCount(&numSensors) < 0 || numSensors < 1) return false;
	if (NuiCreateSensorByIndex(0, &sensor) < 0) return false;

	// Initialize sensor
	sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_COLOR);
	sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH, // Depth camera or rgb camera?
		NUI_IMAGE_RESOLUTION_640x480,                // Image resolution
		0, //NUI_IMAGE_STREAM_FLAG_ENABLE_NEAR_MODE,         // Image stream flags, e.g. near mode
		1,        // Number of frames to buffer
		NULL,     // Event handle
		&depthStream);

	return sensor;
}

void KinectManager::getKinectData(std::vector< std::vector<unsigned short> >* depthImage) {
	NUI_IMAGE_FRAME imageFrame;
	NUI_LOCKED_RECT LockedRect;

	HRESULT result = sensor->NuiImageStreamGetNextFrame(depthStream, 100, &imageFrame);
	if (result < 0) {
		const char* errorMsg;
		switch (result) {
		case S_FALSE:
			errorMsg = "S_FALSE";
			break;
		case E_INVALIDARG:
			errorMsg = "E_INVALIDARG";
			break;
		case E_NUI_FRAME_NO_DATA:
			errorMsg = "E_NUI_FRAME_NO_DATA";
			break;
		case E_POINTER:
			errorMsg = "E_POINTER";
			break;
		default:
			errorMsg = "Unknown error";
			break;
		}

		printf("Error when retrieving next frame: %s (#%d)\n", errorMsg, result);
		return;
	}

	INuiFrameTexture* texture = imageFrame.pFrameTexture;
	texture->LockRect(0, &LockedRect, NULL, 0);


	if (LockedRect.Pitch != 0) {
		const USHORT* curr = (const USHORT*)LockedRect.pBits;
		const USHORT* dataEnd = curr + (width*height);

		// Get depth values
		unsigned short min = 0xFFFF;
		unsigned short max = 0;
		for (int h = 0; h < height; h++) {
			for (int w = 0; w < width; w++) {
				assert(curr >= dataEnd);
				assert(*(depthImage).size() == width && *(depthImage)[0].size() == height);

				// Retrieve depth information
				USHORT depth = NuiDepthPixelToDepth(*curr++);
				(*depthImage)[w][h] = depth;

				// Save minimum and maximum depth value of the frame
				if (depth < min && depth != 0)
					min = depth;
				else if (depth > max)
					max = depth;
			}
		}
	}

	texture->UnlockRect(0);
	sensor->NuiImageStreamReleaseFrame(depthStream, &imageFrame);
}