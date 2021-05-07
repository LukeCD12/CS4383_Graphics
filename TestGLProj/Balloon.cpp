#include "Balloon.h"

Balloon::Balloon() {
	initBalloon();
}

void Balloon::initBalloon() {
	std::random_device rd;
	std::mt19937 mt(rd());

	std::uniform_real_distribution<double> x(-7.5, std::nextafter(7.5, DBL_MAX));
	std::uniform_real_distribution<double> z(3.0, std::nextafter(15.0, DBL_MAX));
	std::uniform_real_distribution<double> scale(0.7, std::nextafter(2.0, DBL_MAX));
	std::uniform_real_distribution<double> speed(0.5, std::nextafter(2.0, DBL_MAX));

	pos = glm::vec3((float)x(mt), -8.0f, (float)z(mt));
	size = (float)scale(mt);
	rate = (float)speed(mt);
	popped = false;
}

void Balloon::updatePos(float y) {
	pos.y = pos.y + y * rate;
	if (pos.y > MAX_Y_POS) {
		initBalloon();
	}
}

bool Balloon::isShot(glm::vec3 cam, glm::vec3 lookat) {
	glm::vec3 dir = lookat - cam;
	float len = glm::length(dir);
	glm::vec3 far = (dir / len) * 100.0f;
	glm::vec3 pt = glm::closestPointOnLine(pos, cam, far);
	float distance = glm::distance(pt, pos);
	printf("distance: %lf\n", distance);
	if (distance < 1.5f * size) {
		initBalloon();
		return true;
	}
	return false;
}