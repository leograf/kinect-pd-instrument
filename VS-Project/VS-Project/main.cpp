#include <assert.h>

#define OSCPKT_OSTREAM_OUTPUT
#include "oscpkt/oscpkt.hh"
#include "oscpkt/udp.hh"

#include <iostream>

#include <SFML/Graphics.hpp>

#include <Ole2.h>

#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>

#include "DepthInformation.h"

#define ADDRESS "127.0.0.1"
#define PORT 7000

const int width = 640;
const int height = 480;

// Kinect variables
INuiSensor* sensor;
HANDLE depthStream;

bool initKinect() {
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


void getKinectData(std::vector< std::vector<unsigned short> >* depthImage) {
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

void drawImage(std::vector< std::vector<float> > velocityInfo, sf::Image* image) {
	(*image).create(width, height, sf::Color::Black);
	for (int w = 0; w < (int) velocityInfo.size(); w++) {
		for (int h = 0; h < (int) velocityInfo[w].size(); h++) {
			sf::Color color(sf::Color::Black);
			/*if (velocityInfo[w][h] > 100) {
				color.r = 255;
			}*/
			if (velocityInfo[w][h] < -100) {
				color.g = 255;
			}

			// Draw pixel on image
			(*image).setPixel(w, h, color);

		}
	}
}

int main()
{
	if (!initKinect()) {
		printf("No Kinect found!\n");
		return 42;
	}

	sf::RenderWindow window(sf::VideoMode(width, height), "DT2300");
	sf::Texture texture;
	sf::Sprite sprite;

	sf::Image image;
	image.create(width, height, sf::Color::Black);

	DepthInformation depthInf(width, height);
	// Matrix for the DepthInformation.
	std::vector< std::vector<unsigned short> > depthImage(width, std::vector<unsigned short>(height));

	sf::Clock clock;
	clock.restart();
	float deltaTime = 0.f;

	// OSC
	oscpkt::UdpSocket sock;
	sock.connectTo(ADDRESS, PORT);
	if (!sock.isOk()) {
		std::cerr << "Error connection to port " << PORT << ": " << sock.errorMessage() << "\n";
		return -1;
	}
	//
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		getKinectData(&depthImage);
		deltaTime = clock.restart().asSeconds();
		depthInf.update(deltaTime, depthImage);

		drawImage(depthInf.getVelocityInformation(), &image);

		//Send PD
		if (sock.isOk()) {
			oscpkt::Message msg("/pixeltest"); msg.pushFloat((float)image.getPixel(0, 0).r);
			oscpkt::PacketWriter pw;
			pw.startBundle().startBundle().addMessage(msg).endBundle().endBundle();
			if (!sock.sendPacket(pw.packetData(), pw.packetSize())) {
				std::cout << "Could not sent packet" << std::endl;
			}
		}
		//

		texture.loadFromImage(image);
		sprite.setTexture(texture);

		window.clear();
		window.draw(sprite);
		window.display();
	}


	NuiShutdown();
	return 0;
}
