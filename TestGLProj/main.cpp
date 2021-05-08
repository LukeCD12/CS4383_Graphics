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

Model *plane;
Model *gun, *balloon1, *balloon2, *balloon3, *balloon4, *balloon5, *balloon6, *balloon7, *balloon8, *balloon9, *balloon10;
Model *stall;
Model* structure;
Model *post, *woodwall;
Model *rnd;

std::vector<Balloon *> balloons;
Balloon *b1, *b2, *b3, *b4, *b5, *b6, *b7, *b8, *b9, *b10;

glm::mat4 projection;
glm::mat4 view;
glm::mat4 model;
glm::vec4 lightPosition = glm::vec4(0.0f,3.0f,0.0f,1.0f);
glm::vec3 pewDir;
glm::mat4 pewModel;

float rotation = 0.0f;
bool shoot = false;
int score = 0;
int difficulty = 0;
bool d1 = true;
bool d2 = true;

QuatCamera *camera;

/* report GL errors, if any, to stderr */
void checkError(const char *functionName) {
	GLenum error;
	while (( error = glGetError() ) != GL_NO_ERROR) {
	  std::cerr << "GL error " << error << " detected in " << functionName << std::endl;
	}
}

void initShader(void) {
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
	initRendering ();
}

void dumpInfo(void) {
	printf ("Vendor: %s\n", glGetString (GL_VENDOR));
	printf ("Renderer: %s\n", glGetString (GL_RENDERER));
	printf ("Version: %s\n", glGetString (GL_VERSION));
	printf ("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));
	checkError ("dumpInfo");
}

void display(void) {
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//output score to screen
	unsigned char scoreStr[] = "Score: 9999999999999";
	sprintf((char*)scoreStr, "Score: %d", score);
	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2f(0.575f, 0.83f);
	for (unsigned char* c = scoreStr; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}

	//output difficulty to screen
	unsigned char difficultyStr[] = "Difficulty: Medium";
	switch (difficulty) {
	case 0:
		glColor3f(0.0f, 1.0f, 0.0f);
		sprintf((char*)difficultyStr, "Difficulty: Easy");
		break;
	case 1:
		glColor3f(1.0f, 1.0f, 0.0f);
		sprintf((char*)difficultyStr, "Difficulty: Medium");
		break;
	case 2:
		sprintf((char*)difficultyStr, "Difficulty: Hard");
		glColor3f(1.0f, 0.0f, 0.0f);
		break;
	}
	glRasterPos2f(0.575f, 0.9f);
	for (unsigned char* c = difficultyStr; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}


	camera->OnRender();

	view = glm::lookAt(camera->GetPos(), camera->GetLookAtPoint(), camera->GetUp());
	
	// rotation += 0.05f; // Update rotation angle if rotation is enabled.
	
	glm::vec4 lightPos = glm::rotate(rotation,0.0f, 0.0f, 1.0f) * lightPosition;
	
	shader.Activate(); // Bind shader.
	shader.SetUniform("lightPosition", view * lightPos);
	shader.SetUniform("lightDiffuse", glm::vec4(1.0, 1.0, 1.0, 1.0));
	shader.SetUniform("lightSpecular", glm::vec4(1.0, 1.0, 1.0, 1.0));
	shader.SetUniform("lightAmbient", glm::vec4(1.0, 1.0, 1.0, 1.0));
	shader.SetUniform("linearAttenuationCoefficient", 0.1f);

	//left front post
	post->render(view * glm::translate(7.6f, -3.0f, 0.5f) * glm::scale(4.0f, 4.5f, 4.0f), projection, true);
	//right front post
	post->render(view * glm::translate(-7.6f, -3.0f, 0.5f) * glm::scale(4.0f, 4.5f, 4.0f), projection, true);
	//back wall
	woodwall->render(view * glm::translate(0.0f, -1.0f, 17.3f) * glm::scale(6.0f, 4.0f, 5.0f) * glm::rotate(90.0f, 1.0f, 0.0f, 0.0f) * glm::rotate(90.0f, 0.0f, 1.0f, 0.0f), projection, true);
	//front counter
	woodwall->render(view * glm::translate(0.0f, -7.2f, 0.5f) * glm::scale(6.0f, 4.0f, 5.0f) * glm::rotate(-90.0f, 1.0f, 0.0f, 0.0f) * glm::rotate(90.0f, 0.0f, 1.0f, 0.0f), projection, true);
	//right wall
	post->render(view * glm::translate(7.7f, -4.5f, 10.0f) * glm::scale(4.0f, 9.0f, 6.0f) * glm::rotate(90.0f, 1.0f, 0.0f, 0.0f), projection, true);
	//left wall
	post->render(view * glm::translate(-7.7f, -4.5f, 10.0f) * glm::scale(4.0f, 9.0f, 6.0f) * glm::rotate(90.0f, 1.0f, 0.0f, 0.0f), projection, true);
	//roof
	post->render(view * glm::translate(0.0f, 3.75f, 0.0f) * glm::scale(5.8f, 4.0f, 4.0f) * glm::rotate(90.0f, 0.0f, 0.0f, 1.0f), projection, true);
	//left roof
	post->render(view * glm::translate(7.7f, 3.0f, 10.0f) * glm::scale(4.0f, 4.0f, 6.0f) * glm::rotate(90.0f, 1.0f, 0.0f, 0.0f), projection, true);
	//right roof
	post->render(view * glm::translate(-7.7f, 3.0f, 10.0f) * glm::scale(4.0f, 4.0f, 6.0f) * glm::rotate(90.0f, 1.0f, 0.0f, 0.0f), projection, true);

	//left stall
	stall->render(view * glm::translate(14.5f, -5.5f, 6.0f) * glm::scale(60.0f, 65.0f, 135.0f), projection, true);
	//right stall
	stall->render(view * glm::translate(-14.5f, -5.5f, 6.0f) * glm::scale(60.0f, 65.0f, 135.0f), projection, true);

	gun->render(glm::translate(0.75f,-1.0f,-2.0f)* glm::scale(.05f, .05f, .05f) *glm::rotate(-180.0f,0.0f,1.0f,0.0f) , projection, true);
	
	b1->updatePos(0.0025f, difficulty);
	balloon1->render(view * glm::translate(b1->pos) * glm::scale(b1->size, b1->size, b1->size), projection, false);

	b2->updatePos(0.0025f, difficulty);
	balloon2->render(view * glm::translate(b2->pos) * glm::scale(b2->size, b2->size, b2->size), projection, false);

	b3->updatePos(0.0025f, difficulty);
	balloon3->render(view * glm::translate(b3->pos) * glm::scale(b3->size, b3->size, b3->size), projection, false);

	b4->updatePos(0.0025f, difficulty);
	balloon4->render(view * glm::translate(b4->pos) * glm::scale(b4->size, b4->size, b4->size), projection, false);

	b5->updatePos(0.0025f, difficulty);
	balloon5->render(view * glm::translate(b5->pos) * glm::scale(b5->size, b5->size, b5->size), projection, false);

	if (difficulty >= 1) {
		b6->updatePos(0.0025f, difficulty);
		balloon6->render(view * glm::translate(b6->pos) * glm::scale(b6->size, b6->size, b6->size), projection, true);

		b7->updatePos(0.0025f, difficulty);
		balloon7->render(view * glm::translate(b7->pos) * glm::scale(b7->size, b7->size, b7->size), projection, true);

		balloon6->setOverrideDiffuseMaterial(b6->color);
		balloon7->setOverrideDiffuseMaterial(b7->color);

		balloon6->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
		balloon6->setOverrideSpecularShininessMaterial(300.0f);
		balloon7->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
		balloon7->setOverrideSpecularShininessMaterial(300.0f);
	}

	if (difficulty >= 2) {
		b8->updatePos(0.0025f, difficulty);
		balloon8->render(view * glm::translate(b8->pos) * glm::scale(b8->size, b8->size, b8->size), projection, true);

		b9->updatePos(0.0025f, difficulty);
		balloon9->render(view * glm::translate(b9->pos) * glm::scale(b9->size, b9->size, b9->size), projection, true);

		b10->updatePos(0.0025f, difficulty);
		balloon10->render(view * glm::translate(b10->pos) * glm::scale(b10->size, b10->size, b10->size), projection, true);

		balloon8->setOverrideDiffuseMaterial(b8->color);
		balloon9->setOverrideDiffuseMaterial(b9->color);
		balloon10->setOverrideDiffuseMaterial(b10->color);

		balloon8->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
		balloon8->setOverrideSpecularShininessMaterial(300.0f);
		balloon9->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
		balloon9->setOverrideSpecularShininessMaterial(300.0f);
		balloon10->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
		balloon10->setOverrideSpecularShininessMaterial(300.0f);
	}

	balloon1->setOverrideDiffuseMaterial(b1->color);
	balloon2->setOverrideDiffuseMaterial(b2->color);
	balloon3->setOverrideDiffuseMaterial(b3->color);
	balloon4->setOverrideDiffuseMaterial(b4->color);
	balloon5->setOverrideDiffuseMaterial(b5->color);

	balloon1->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	balloon1->setOverrideSpecularShininessMaterial(300.0f);
	balloon2->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	balloon2->setOverrideSpecularShininessMaterial(300.0f);
	balloon3->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	balloon3->setOverrideSpecularShininessMaterial(300.0f);
	balloon4->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	balloon4->setOverrideSpecularShininessMaterial(300.0f);
	balloon5->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	balloon5->setOverrideSpecularShininessMaterial(300.0f);

	plane->setOverrideDiffuseMaterial(glm::vec4(1.0, 0.0, 0.0, 1.0));
	plane->setOverrideAmbientMaterial(glm::vec4(0.2 , 0.0, 0.0, 1.0));
	plane->setOverrideSpecularMaterial(glm::vec4(1.0, 1.0, 1.0, 1.0));
	plane->setOverrideSpecularShininessMaterial(90.0f);
	plane->setOverrideEmissiveMaterial(glm::vec4(0.0, 0.0, 0.0, 1.0));
	plane->render(view * glm::translate(0.0f, -6.0f, 0.0f) * glm::scale(20.0f, 20.0f, 20.0f), projection, false);

	glutSwapBuffers(); // Swap the buffers.
	checkError ("display");
}

void idle() {
	float currTime = glutGet(GLUT_ELAPSED_TIME);
	int sec = (int)currTime / 1000;
	if (sec == 60 && d1) {
		difficulty = 1;
		b6 = new Balloon();
		b7 = new Balloon();
		balloons.push_back(b6);
		balloons.push_back(b7);
		d1 = false;
	}
	else if (sec == 120 && d2) {
		difficulty = 2;
		b8 = new Balloon();
		b9 = new Balloon();
		b10 = new Balloon();
		balloons.push_back(b8);
		balloons.push_back(b9);
		balloons.push_back(b10);
		d2 = false;
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
		if (curr->isShot(pos, lookat)) {
			score++;
		}
	}
	printf("num balloons: %d\n", balloons.size());
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
	balloon6 = new Model(&shader, "models/Balloon.obj", "models/");
	balloon7 = new Model(&shader, "models/Balloon.obj", "models/");
	balloon8 = new Model(&shader, "models/Balloon.obj", "models/");
	balloon9 = new Model(&shader, "models/Balloon.obj", "models/");
	balloon10 = new Model(&shader, "models/Balloon.obj", "models/");

	rnd = new Model(&shader, "models/shpere.obj", "models/");

	stall = new Model(&shader, "models/stall.obj", "models/");

	plane = new Model(&shader,"models/plane.obj",  "models/");
	gun = new Model(&shader,"models/pistola.obj", "models/");

	structure = new Model(&shader, "models/cubeaxisaligned.obj", "models/");
	woodwall = new Model(&shader, "models/woodwall.obj", "models/");
	post = new Model(&shader, "models/post.obj", "models/");

	glutMainLoop();

   return 0;
}