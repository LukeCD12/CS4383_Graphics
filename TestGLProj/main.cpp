#include <GL/glew.h>
#include <GL/freeglut.h>

//glm library
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Balloon.h"
#include "Model.h"
#include "Shader.h"
#include "QuatCamera.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Shader shader;
Shader water;

Model *plane;
Model *gun, *balloon1, *balloon2, *balloon3, *balloon4, *balloon5;
Model *structure, *pole;
Model *rnd;
Model *banner;

std::vector<Balloon *> balloons;
Balloon *b1, *b2, *b3, *b4, *b5;

glm::mat4 projection;
glm::mat4 view;
glm::mat4 model;
glm::vec4 lightPosition = glm::vec4(0.0f,3.0f,0.0f,1.0f);
glm::vec3 pewDir;
glm::mat4 pewModel;

float rotation = 0.0f;
float mtime = 0.0f;
float lastTime = 0.0f;
bool shoot = false;

QuatCamera *camera;

/* report GL errors, if any, to stderr */
void checkError(const char *functionName) {
	GLenum error;
	while (( error = glGetError() ) != GL_NO_ERROR) {
	  std::cerr << "GL error " << error << " detected in " << functionName << std::endl;
	}
}

void initShader(void) {
	water.InitializeFromFile("shaders/water.vert", "shaders/water.frag");
	shader.InitializeFromFile("shaders/phong3.vert", "shaders/phong3.frag");
	checkError("initShader");
}

void initRendering(void) {
	glClearColor(0.117f, 0.565f, 1.0f, 0.0f); // Dodger Blue
	checkError("initRendering");
}

void init(void)  {	
	// View  positioned back -5 on the z axis, looking into the screen.
	glm::vec3 initpos = glm::vec3(0.0f, 0.0f, -10.0f);
	glm::vec3 initlookatpnt = glm::vec3(.0f, .0f, -1.0f);
	camera = new QuatCamera(800, 600, initpos, initlookatpnt, glm::vec3(0.0f, 1.0f, 0.0f));
	// Perspective projection matrix.
	projection = glm::perspective(45.0f, 800.0f/600.0f, 1.0f, 1000.0f);

	// Load identity matrix into model matrix.
	model = glm::mat4();

	b1 = new Balloon();
	b2 = new Balloon();
	b3 = new Balloon();
	b4 = new Balloon();
	b5 = new Balloon();

	balloons.push_back(b1);
	balloons.push_back(b2);
	balloons.push_back(b3);
	balloons.push_back(b4);
	balloons.push_back(b5);

	initShader();
	initRendering();
}

void dumpInfo(void) {
	printf ("Vendor: %s\n", glGetString (GL_VENDOR));
	printf ("Renderer: %s\n", glGetString (GL_RENDERER));
	printf ("Version: %s\n", glGetString (GL_VERSION));
	printf ("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));
	checkError ("dumpInfo");
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	camera->OnRender();

	view = glm::lookAt(camera->GetPos(), camera->GetLookAtPoint(), camera->GetUp());
	
	glm::vec4 lightPos = glm::rotate(rotation,0.0f, 0.0f, 1.0f) * lightPosition;
	
	shader.Activate(); // Bind shader.
	shader.SetUniform("lightPosition", view * lightPos);
	shader.SetUniform("lightDiffuse", glm::vec4(1.0, 1.0, 1.0, 1.0));
	shader.SetUniform("lightSpecular", glm::vec4(1.0, 1.0, 1.0, 1.0));
	shader.SetUniform("lightAmbient", glm::vec4(1.0, 1.0, 1.0, 1.0));
	shader.SetUniform("linearAttenuationCoefficient", 0.3f);

	water.Activate();
	water.SetUniform("time", mtime);

	//front banner
	banner->render(view * glm::translate(0.0f, 3.0f, -0.8f) * glm::rotate(90.0f, 1.0f, 0.0f, 0.0f) * glm::scale(7.0f, 1.0f, 1.0f), projection, false);

	//left front post
	pole->render(view * glm::translate(8.0f, -3.0f, 0.0f) * glm::scale(1.0f, 7.0f, 1.0f), projection, false);
	//right front post
	pole->render(view * glm::translate(-8.0f, -3.0f, 0.0f) * glm::scale(1.0f, 7.0f, 1.0f), projection, false);
	//front counter
	structure->render(view * glm::translate(0.0f, -6.0f, 0.0f) * glm::scale(7.5f, 3.0f, 0.7f), projection, true);
	//right wall
	structure->render(view * glm::translate(-8.0f, -6.0f, 9.0f) * glm::scale(0.7f, 3.0f, 9.0f), projection, false);
	//left wall
	structure->render(view * glm::translate(8.0f, -6.0f, 9.0f) * glm::scale(0.7f, 3.0f, 9.0f), projection, false);
	//back wall
	structure->render(view * glm::translate(0.0f, -4.0f, 17.3f) * glm::scale(8.5f, 8.0f, 0.7f), projection, true);
	//roof
	structure->render(view * glm::translate(0.0f, 3.75f, 0.0f) * glm::scale(7.5f, 0.2f, 0.7f), projection, false);
	//left roof
	structure->render(view * glm::translate(8.0f, 3.75f, 9.0f) * glm::scale(0.7f, 0.2f, 9.0f), projection, false);
	//right roof
	structure->render(view * glm::translate(-8.0f, 3.75f, 9.0f) * glm::scale(0.7f, 0.2f, 9.0f), projection, false);

	gun->render(glm::translate(0.75f,-1.0f,-2.0f)* glm::scale(.05f, .05f, .05f) *glm::rotate(-180.0f,0.0f,1.0f,0.0f) , projection, true);
	
	b1->updatePos(0.0025f);
	balloon1->render(view * glm::translate(b1->pos) * glm::scale(b1->size, b1->size, b1->size), projection, true);

	b2->updatePos(0.0025f);
	balloon2->render(view * glm::translate(b2->pos) * glm::scale(b2->size, b2->size, b2->size), projection, true);

	b3->updatePos(0.0025f);
	balloon3->render(view * glm::translate(b3->pos) * glm::scale(b3->size, b3->size, b3->size), projection, true);

	b4->updatePos(0.0025f);
	balloon4->render(view * glm::translate(b4->pos) * glm::scale(b4->size, b4->size, b4->size), projection, true);

	b5->updatePos(0.0025f);
	balloon5->render(view * glm::translate(b5->pos) * glm::scale(b5->size, b5->size, b5->size), projection, true);

	plane->setOverrideDiffuseMaterial(glm::vec4(1.0, 0.0, 0.0, 1.0));
	plane->setOverrideAmbientMaterial(glm::vec4(0.2 , 0.0, 0.0, 1.0));
	plane->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	plane->setOverrideSpecularShininessMaterial(90.0f);
	plane->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	plane->render(view * glm::translate(0.0f, -6.0f, 0.0f) * glm::scale(20.0f, 20.0f, 20.0f), projection, false);

	glutSwapBuffers(); // Swap the buffers.
	checkError("display");
}

void idle() {
	float currTime = glutGet(GLUT_ELAPSED_TIME);
	if (currTime - lastTime > 1.0f) {
		mtime = currTime;
		lastTime = currTime;
	}
	glFlush();
	glutPostRedisplay();
}


void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	checkError("reshape");
}

void specialKeyboard(int Key, int x, int y) {
		camera->OnKeyboard(Key);
}

void keyboard(unsigned char key, int x, int y){
	switch(key) {
		case 27: //ESC key
			exit(0);
			break;
   }
}

static void passiveMouse(int x, int y) {
  camera->OnMouse(x, y);
}

void pew(int x, int y) {
	//printf("pew");
	glm::vec3 pos = camera->GetPos();
	glm::vec3 lookat = camera->GetLookAtPoint();
	for (auto curr : balloons) {
		curr->isShot(pos, lookat);
	}
	printf("-----------------------------------\n");
}

static void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		pew(x, y);
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	//glutInitContextVersion(1,5);
	//glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE );
	glutInitDisplayMode (GLUT_DOUBLE| GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600); 
	glutInitWindowPosition(100, 100);
	glutCreateWindow("prj");
	
	//glutGameModeString("800x600:8");
	//glutEnterGameMode();
	glewInit();
	dumpInfo();
	init();

	glutDisplayFunc(display); 
	glutIdleFunc(idle); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);
    glutPassiveMotionFunc(passiveMouse);
	glutMouseFunc(mouse);
	glutSetCursor(GLUT_CURSOR_FULL_CROSSHAIR);

	glEnable(GL_DEPTH_TEST);

	balloon1 = new Model(&shader, "models/Balloon.obj", "models/");
	balloon2 = new Model(&shader, "models/Balloon.obj", "models/");
	balloon3 = new Model(&shader, "models/Balloon.obj", "models/");
	balloon4 = new Model(&shader, "models/Balloon.obj", "models/");
	balloon5 = new Model(&shader, "models/Balloon.obj", "models/");

	rnd = new Model(&shader, "models/shpere.obj", "models/");

	plane = new Model(&shader,"models/plane.obj",  "models/");
	banner = new Model(&water, "models/planehires.obj");
	gun = new Model(&shader,"models/pistola.obj", "models/");

	structure = new Model(&shader, "models/cubeaxisaligned.obj", "models/");
	pole = new Model(&shader, "models/cylinder.obj", "models/");

	glutMainLoop();

   return 0;
}