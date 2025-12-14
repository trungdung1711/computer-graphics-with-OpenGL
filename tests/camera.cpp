#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

extern int X;
extern int Y;

void testCameraDisplay()
{
    // glViewport(0, 0, 500, 500);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    glTranslatef(X / 200.0f, -Y / 200.0f, 0.0f);

    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidCube(1.0f);

    glutSwapBuffers();
}