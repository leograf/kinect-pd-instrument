#include <vector>

#define VELOCITY_THRESHOLD 700.f

struct DepthPoint
{
	unsigned short depth;
	float velocity;
	float acceleration;

	DepthPoint()
		: depth(0), velocity(0.f), acceleration(0.f)
	{
	};
};

class DepthInformation
{
public:
	DepthInformation(int width, int height);
	~DepthInformation();

	void update(float deltaTime, const std::vector< std::vector<unsigned short> >& depthImage);
	std::vector< std::vector<float> > getVelocityInformation();

private:
	int width, height;

	std::vector< std::vector<DepthPoint> > depthBuffer;
	std::vector< std::vector<float> > velocityFrame;
};

