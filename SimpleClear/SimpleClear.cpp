#include <cstdlib>
#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

GLint window = 0;
GLfloat seconds = 0.0f;

void create_glut_window();
void init_OpenGL();
void create_glut_callbacks();

//Glut callback functions
void display();
void idle();

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);

	create_glut_window();
	init_OpenGL();


	create_glut_callbacks();
	glutMainLoop();

	return EXIT_SUCCESS;
}


void create_glut_window() {
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	window = glutCreateWindow("OpenGL SuperBible Example");
}


void init_OpenGL() {
	using std::cout;
	using std::cerr;
	using std::endl;
	/************************************************************************/
	/*                    Init OpenGL context                               */
	/************************************************************************/
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		cerr << "Error: " << glewGetErrorString(err) << endl;
	}
	cout << "Hardware specification: " << endl;
	cout << "Vendor: " << glGetString(GL_VENDOR) << endl;
	cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
	cout << "Software specification: " << endl;
	cout << "Using GLEW " << glewGetString(GLEW_VERSION) << endl;
	cout << "Using OpenGL " << glGetString(GL_VERSION) << endl;
	cout << "Using GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	int ver = glutGet(GLUT_VERSION);
	cout << "Using freeglut version: " << ver / 10000 << "." << (ver / 100) % 100 << "." << ver % 100 << endl;

}


void create_glut_callbacks() {
	glutDisplayFunc(display);
	
	glutIdleFunc(idle);
}

void idle() {
	static int last_time = 0;
	int time = glutGet(GLUT_ELAPSED_TIME);
	int elapsed = time - last_time;
	last_time = time;
	float delta_seconds = 0.001f * elapsed;
	seconds += delta_seconds;
	glutPostRedisplay();
}


void display() {
	glm::vec4 color = glm::vec4(glm::sin(seconds) * 0.5f + 0.5f,
							  glm::cos(seconds) * 0.5f + 0.5f,
							  0.0f, 1.0f);
	glClearBufferfv(GL_COLOR, 0, glm::value_ptr(color));
	


	glutSwapBuffers();
}