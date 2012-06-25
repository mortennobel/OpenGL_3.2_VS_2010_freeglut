#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Angel.h"

using namespace std;
using namespace Angel;

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 800;

GLuint shaderProgram;
GLuint positionUniform;
GLuint colourAttribute, positionAttribute;
GLuint vertexArrayObject,
	vertexBuffer;

typedef struct
{
    vec4 position;
    vec4 colour;
} Vertex;

void loadShader();
void display();

void loadBufferData(){
	 Vertex vertexData[4] = {
        { vec4(-0.5, -0.5, 0.0, 1.0 ), vec4( 1.0, 0.0, 0.0, 1.0 ) },
        { vec4(-0.5,  0.5, 0.0, 1.0 ), vec4( 0.0, 1.0, 0.0, 1.0 ) },
        { vec4( 0.5,  0.5, 0.0, 1.0 ), vec4( 0.0, 0.0, 1.0, 1.0 ) },
        { vec4( 0.5, -0.5, 0.0, 1.0 ), vec4( 1.0, 1.0, 1.0, 1.0 ) }
    };
    
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertexData, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(positionAttribute);
    glEnableVertexAttribArray(colourAttribute);
    glVertexAttribPointer(positionAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)0);
    glVertexAttribPointer(colourAttribute  , 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)sizeof(vec4));
}

void loadShader(){
	shaderProgram = InitShader("beelsebob.vert",  "beelsebob.frag", "fragColour");
	positionUniform = glGetUniformLocation(shaderProgram, "p");
	if (positionUniform < 0) {
		cerr << "Shader did not contain the 'p' uniform."<<endl;
	}
	colourAttribute = glGetAttribLocation(shaderProgram, "colour");
	if (colourAttribute < 0) {
		cerr << "Shader did not contain the 'colour' attribute." << endl;
	}
	positionAttribute = glGetAttribLocation(shaderProgram, "position");
	if (positionAttribute < 0) {
		cerr << "Shader did not contain the 'position' attribute." << endl;
	}
}

void display() {	
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	const float timeScale = 0.008f;
	
    glUseProgram(shaderProgram);

    GLfloat timeValue = glutGet(GLUT_ELAPSED_TIME)*timeScale; 
    vec2 p( 0.5f * sinf(timeValue), 0.5f * cosf(timeValue) );
    glUniform2fv(positionUniform, 1, (const GLfloat *)&p);
    
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
	glutSwapBuffers();
}

void reshape(int W, int H) {
    WINDOW_WIDTH = W;
	WINDOW_HEIGHT = H;
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void animate() {	
	glutPostRedisplay();
}

void visible(int vis) {
	if (vis == GLUT_VISIBLE)
		glutIdleFunc(animate);
	else
		glutIdleFunc(0);
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    
	glutSetOption(
        GLUT_ACTION_ON_WINDOW_CLOSE,
        GLUT_ACTION_GLUTMAINLOOP_RETURNS
    );

	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
	glutCreateWindow("02561");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutVisibilityFunc(visible);
	glutIdleFunc(animate);
	glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);

	GLint GlewInitResult = glewInit();
	if (GlewInitResult != GLEW_OK) {
		printf("ERROR: %s\n", glewGetErrorString(GlewInitResult));
	}

	glEnable(GL_DEPTH_TEST);

	loadShader();
    loadBufferData();

	glutMainLoop();
}