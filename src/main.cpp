#include "Car.hpp"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <tests.hpp>

int WIDTH = 500;
int HEIGHT = 500;
int WINDOW_X = 500;
int WINDOW_Y = 0;

int X = 0;
int Y = 0;

void move(int x, int y)
{
    X = x;
    Y = y;
    glutPostRedisplay();
}

int main(int argc, char **argv)
{

    // set up the OpenGL context with glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(WINDOW_X, WINDOW_Y);
    glutCreateWindow("Test with camera");

    // register callback for display
    // and idle to update the game logic
    glutDisplayFunc(testCameraDisplay);
    glutPassiveMotionFunc(move);

    // start the whole loop
    // normally, this loop will re-render
    // on demand
    glutMainLoop();

    return 0;
}