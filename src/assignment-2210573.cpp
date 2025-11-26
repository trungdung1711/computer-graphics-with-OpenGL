#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <vector>

/*
############ OBJECT MODELING
*/
class Vector3
{
public:
	float x, y, z;

	Vector3();
	Vector3(float x, float y, float z);
	Vector3(Vector3 &v);
	void set(float x, float y, float z);
	void set(Vector3 &v);
	void flip();
	void normalize();
	Vector3 cross(Vector3 &b);
	float dot(Vector3 &b);
};

Vector3::Vector3()
{
	this->x = 0.0;
	this->y = 0.0;
	this->z = 0.0;
}

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vector3::normalize()
{
	float len = std::sqrt(x * x + y * y + z * z);

	if (len > 0.0f)
	{
		this->x /= len;
		this->y /= len;
		this->z /= len;
	}
}

Vector3 Vector3::cross(Vector3 &b)
{
	return Vector3(
		y * b.z - z * b.y,
		z * b.x - x * b.z,
		x * b.y - y * b.x);
}

float Vector3::dot(Vector3 &b)
{
	return this->x * b.x + this->y * b.y + this->z * b.z;
}

struct Vertex
{
	Vector3 position;
	Vector3 normal;
	Vector3 color;
	// Vector2 texcoord;
};

class Mesh
{
public:
	std::vector<Vertex *> vertices;
	std::vector<std::vector<int>> faces;

public:
	Mesh();
	~Mesh();

	void build(const std::vector<Vertex *> &inVertices, const std::vector<std::vector<int>> &inFaces);

	void drawWire();

	void drawColor();
};

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
	for (int i = 0; i < this->vertices.size(); ++i)
	{
		// deallocate the memory
		delete vertices[i];
	}
}

void Mesh::build(const std::vector<Vertex *> &inVertices, const std::vector<std::vector<int>> &inFaces)
{
	vertices = inVertices;
	faces = inFaces;
}

void Mesh::drawWire()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (int i = 0; i < this->faces.size(); ++i)
	{
		std::vector<int> face = this->faces[i];
		glBegin(GL_POLYGON);
		for (int j = 0; j < face.size(); ++j)
		{
			Vertex *v = vertices[j];
			glColor3f(v->color.x, v->color.y, v->color.z);
			glVertex3f(v->position.x, v->position.y, v->position.z);
		}
		glEnd();
	}
}

void Mesh::drawColor()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for (std::vector<int> &f : faces)
	{
		glBegin(GL_POLYGON);
		for (int idx : f)
		{
			Vertex *v = vertices[idx];
			glColor3f(v->color.x, v->color.y, v->color.z);
			glNormal3f(v->normal.x, v->normal.y, v->normal.z);
			glVertex3f(v->position.x, v->position.y, v->position.z);
		}
		glEnd();
	}
}

/*
############ OBJECT MODELING
*/

/*
############ GLOBAL
*/
int width = 500;
int height = 500;
/*
############ GLOBAL
*/

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0, 1, 0);

	// object
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();
	glFlush();
	glViewport(0, 0, width, height);
	glutSwapBuffers();

	std::cout << "Hello~";
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Assignment");

	glutDisplayFunc(display);

	glutMainLoop();
	return 0;
}

/*
	while (true) {
		processEvents();     // mouse, keyboard, resize
		if (needToRedraw) {
			display_callback();  // <== your render() function
		}
	}
*/