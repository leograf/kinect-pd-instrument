#include <vector>

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
	static const int BUFFER_SIZE;

	int width, height;

	unsigned int bufferPos;
	std::vector< std::vector< std::vector<DepthPoint> > > depthBuffer;
	std::vector< std::vector<float> > velocityFrame;
};

