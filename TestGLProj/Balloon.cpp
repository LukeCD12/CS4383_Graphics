#include "Balloon.h"

Balloon::Balloon() {
	initBalloon();
}

void Balloon::initBalloon() {
	std::random_device rd;
	std::mt19937 mt(rd());

	std::uniform_real_distribution<double> x(-7.0, std::nextafter(7.0, DBL_MAX));
	std::uniform_real_distribution<double> z(3.0, std::nextafter(15.0, DBL_MAX));
	std::uniform_real_distribution<double> scale(0.7, std::nextafter(2.0, DBL_MAX));
	std::uniform_real_distribution<double> speed(0.5, std::nextafter(2.0, DBL_MAX));
	std::uniform_real_distribution<double> r(0.0, std::nextafter(1.0, DBL_MAX));
	std::uniform_real_distribution<double> g(0.0, std::nextafter(1.0, DBL_MAX));
	std::uniform_real_distribution<double> b(0.0, std::nextafter(1.0, DBL_MAX));

	pos = glm::vec3((float)x(mt), -9.5f, (float)z(mt));
	color = glm::vec4((float)r(mt), (float)g(mt), (float)b(mt), 1.0f);
	size = (float)scale(mt);
	rate = (float)speed(mt);
	popped = false;
}

void Balloon::updatePos(float y, int difficulty) {
	difficulty += 1;
	pos.y = pos.y + y * rate * difficulty;
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