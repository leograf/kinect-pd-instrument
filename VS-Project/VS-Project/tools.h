#ifndef TOOLS_H
#define TOOLS_H

#define WIN32_LEAN_AND_MEAN

#include <assert.h>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <Ole2.h>
#include <algorithm>
#include <sstream>
#include <string>


#define ADDRESS "127.0.0.1"
#define PORT 7000
#define VELOCITY_THRESHOLD 700.f


const int width = 640;
const int height = 480;

namespace tools {
	enum Chord {
		CminArp,
		CmajArp,
		Cmaj,
		C3rds,
		CJazz
	};
};
#endif // TOOLS_H