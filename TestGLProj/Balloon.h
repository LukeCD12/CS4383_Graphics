#include <stdlib.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/closest_point.hpp>
#include <random>

class Balloon {

public:
	Balloon();
	~Balloon(void);
	bool popped;
	float rate;
	float size;
	glm::vec3 pos;
	void updatePos(float y);
	void isShot(glm::vec3 pos, glm::vec3 lookat);
private:
	float MAX_Y_POS = 8.0f;
	void initBalloon();
	float distance(glm::vec3 pos1, glm::vec3 pos2);
};