#include <cstdlib>
#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

GLint window = 0;
GLfloat seconds = 0.0f;
GLuint rendering_program;
GLuint vertex_array_object;

void create_glut_window();
void init_OpenGL();
void create_glut_callbacks();
void init_program();
void exit_glut();
GLuint compile_shaders();

//Glut callback functions
void display();
void idle();

using namespace std;

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);

	create_glut_window();
	init_OpenGL();
	init_program();

	create_glut_callbacks();
	glutMainLoop();

	exit_glut();

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

void init_program() {
	rendering_program = compile_shaders();
	//In an Intel GPU you cannot bind a vao if you dont pass data
	//To it. i. e. No empthy array objects 
	//glCreateVertexArrays(1, &vertex_array_object);
	//glBindVertexArray(vertex_array_object);
}

void display() {
	glm::vec4 color = glm::vec4(0.0f, 0.2f, 0.0f, 1.0f);
	glClearBufferfv(GL_COLOR, 0, glm::value_ptr(color));

	// Use the program object we created earlier for rendering
	glUseProgram(rendering_program);

	// Draw one point
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glutSwapBuffers();
}

GLuint compile_shaders(void)
{
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;

	// Source code for vertex shader
	string vertex_shader_source =

		"#version 440 core                                  \n"
		"                                                   \n"
		"void main(void)                                    \n"
		"{                                                  \n"
		"    // Declare a hard-coded array of positions     \n"
		"	 const vec4 vertices[3] = vec4[3](vec4(0.25, -0.25, 0.5, 1.0),       \n"
		"                                     vec4(-0.25, -0.25, 0.5, 1.0),       \n"
		"	                                  vec4(0.25, 0.25, 0.5, 1.0));      \n"
		"                                                   \n"
		"    // Index into our array using gl_VertexID      \n"
		"    gl_Position = vertices[gl_VertexID];           \n"
		"}                                                  \n";

	// Source code for fragment shader
	string fragment_shader_source =
		"#version 440 core                                  \n"
		"                                                   \n"
		"out vec4 color;                                    \n"
		"                                                   \n"
		"void main(void)                                    \n"
		"{                                                  \n"
		"   color = vec4(sin(gl_FragCoord.x * 0.25) * 0.5 + 0.5,\n"
		"   cos(gl_FragCoord.y * 0.25) * 0.5 + 0.5,         \n"
		"   sin(gl_FragCoord.x * 0.15) * cos(gl_FragCoord.y * 0.15),\n"
		"   1.0);                                           \n"
		"}                                                  \n";

	// Create and compile vertex shader
	int status;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	const char* start = &vertex_shader_source[0];
	glShaderSource(vertex_shader, 1, &start, nullptr);
	glCompileShader(vertex_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		cerr << "Vertex shader was not compiled!!" << endl;
	}
	// Create and compile fragment shader
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	start = &fragment_shader_source[0];
	glShaderSource(fragment_shader, 1, &start, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		cerr << "Fragment shader was not compiled!!" << endl;
	}
	// Create program, attach shaders to it, and link it
	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		cerr << "OpenGL program was not linked!!" << endl;
	}
	// Delete the shaders as the program has them now
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return program;
}

void exit_glut() {
	glDeleteVertexArrays(1, &vertex_array_object);
	glDeleteProgram(rendering_program);
}