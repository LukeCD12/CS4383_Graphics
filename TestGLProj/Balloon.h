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
	glm::vec4 color;
	glm::vec3 pos;
	int missed;
	void updatePos(float y, int difficulty);
	bool isShot(glm::vec3 pos, glm::vec3 lookat);
private:
	float MAX_Y_POS = 5.5f;
	void initBalloon();
	float distance(glm::vec3 pos1, glm::vec3 pos2);
};