#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <vector>

// VECTOR3
// ###########################################################
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

Vector3::Vector3(Vector3 &v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
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

void Vector3::set(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
// ###########################################################

// GLOBAL
// ###########################################################
int WIDTH = 500;
int HEIGHT = 500;
const char *ID = "Le Bui Trung Dung - 2210573";
Vector3 *COLORMAP;

void globalInit()
{
	COLORMAP = new Vector3[100];
	COLORMAP[0] = {1.0f, 0.0f, 0.0f}; // red
	COLORMAP[1] = {0.0f, 1.0f, 0.0f}; // green
	COLORMAP[2] = {0.0f, 0.0f, 1.0f}; // blue
}

void clean()
{
	delete[] COLORMAP;
}
// ###########################################################

class Vertex
{
public:
	Vector3 position;
	Vector3 normal;
	Vector3 color;
	int colorIndex;
	// Vector2 texcoord;

	Vertex();
	Vertex(Vector3 position);
	Vertex(Vector3 position, Vector3 color);
	Vertex(Vector3 position, Vector3 normal, Vector3 color);
	Vertex(Vector3 position, int colorIndex);
};

Vertex::Vertex(Vector3 position)
{
	this->position = position;
}

Vertex::Vertex(Vector3 position, Vector3 color)
{
	this->position = position;
	this->color = color;
}

Vertex::Vertex(Vector3 position, int colorIndex)
{
	this->position = position;
	this->colorIndex = colorIndex;
}

class Material
{
public:
	float r, g, b;

	Material(float r = 1, float g = 1, float b = 1) : r(r), g(g), b(b) {}

	void apply() const
	{
		glColor3f(r, g, b);
	}
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

	void draw();
};

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
	for (int i = 0; i < this->vertices.size(); ++i)
	{
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

	for (const std::vector<int> &face : faces)
	{
		glBegin(GL_POLYGON);
		for (int idx : face)
		{
			Vertex *v = vertices[idx];
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
			// get the vertex from the set of vertex
			Vertex *v = vertices[idx];
			glColor3f(v->color.x, v->color.y, v->color.z);
			glNormal3f(v->normal.x, v->normal.y, v->normal.z);
			glVertex3f(v->position.x, v->position.y, v->position.z);
		}
		glEnd();
	}
}

void Mesh::draw()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for (const std::vector<int> &face : faces)
	{
		glBegin(GL_POLYGON);
		for (int idx : face)
		{
			Vertex *v = vertices[idx];
			glVertex3f(v->position.x, v->position.y, v->position.z);
		}
		glEnd();
	}
}

class MeshInstance
{
public:
	Mesh *mesh;
	Vector3 localPosition;
	Vector3 localRotation;
	Vector3 localScale;

	MeshInstance(Mesh *m) : mesh(m), localPosition(), localRotation(), localScale(1, 1, 1) {}

	void draw();
};

void MeshInstance::draw()
{
	glPushMatrix();

	// moving
	glTranslatef(localPosition.x, localPosition.y, localPosition.z);
	// rotating
	glRotatef(localRotation.x, 1, 0, 0);
	glRotatef(localRotation.y, 0, 1, 0);
	glRotatef(localRotation.z, 0, 0, 1);
	// scaling
	glScalef(localScale.x, localScale.y, localScale.z);

	mesh->draw();
	glPopMatrix();
}

class Object
{
public:
	std::vector<MeshInstance *> parts;

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	Object();
	void add(MeshInstance *mesh);
	void draw();
	~Object();
};

Object::Object()
{
	this->position.set(0.0f, 0.0f, 0.0f);
	this->rotation.set(0.0f, 0.0f, 0.0f);
	this->scale.set(1.0f, 1.0f, 1.0f);
}

void Object::add(MeshInstance *iMesh)
{
	this->parts.push_back(iMesh);
}

void Object::draw()
{
	glPushMatrix();
	// moving
	glTranslatef(position.x, position.y, position.z);
	// rotating
	glRotatef(rotation.x, 1, 0, 0);
	glRotatef(rotation.y, 0, 1, 0);
	glRotatef(rotation.z, 0, 0, 1);
	// scaling
	glScalef(scale.x, scale.y, scale.z);
	for (auto &part : parts)
	{
		part->draw();
	}

	glPopMatrix();
}

Object::~Object()
{
	for (MeshInstance *part : this->parts)
	{
		delete part;
	}
}

/*
############ OBJECT MODELING
*/

/*
############ CAMERA
*/
class Camera
{
public:
	Vector3 position;
	Vector3 target;
	Vector3 up;

	Camera() : position(0, 0, 5), target(0, 0, 0), up(0, 1, 0) {}

	void apply()
	{
		gluLookAt(position.x, position.y, position.z,
				  target.x, target.y, target.z,
				  up.x, up.y, up.z);
	}
};

/*
############ CAMERA
*/

/*
############ LIGHT
*/
class Light
{
public:
	void apply()
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		GLfloat pos[] = {1, 2, 3, 1};
		glLightfv(GL_LIGHT0, GL_POSITION, pos);
	}
};
/*
############ LIGHT
*/

class Scene
{
public:
	std::vector<Object *> objects;

	void add(Object *object);

	void draw();

	void update();

	void init();

	~Scene();
};

void Scene::add(Object *object)
{
	this->objects.push_back(object);
}

void Scene::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(
		60.0f,
		(float)WIDTH / (float)HEIGHT,
		0.1f,
		100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(
		5.0f, 5.0f, 5.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);

	for (Object *object : objects)
	{
		object->draw();
	}

	glutSwapBuffers();
}

void Scene::init()
{
	// object
	float a = 1.0f;
	float b = 5.0f;
	float c = 1.0f;

	std::vector<Vertex *> vertices = {
		new Vertex(Vector3(a / 2, -b / 2, -c / 2), COLORMAP[0]),
		new Vertex(Vector3(a / 2, -b / 2, c / 2), COLORMAP[1]),
		new Vertex(Vector3(-a / 2, -b / 2, c / 2), COLORMAP[2]),
		new Vertex(Vector3(-a / 2, -b / 2, -c / 2), COLORMAP[0]),
		new Vertex(Vector3(a / 2, b / 2, -c / 2), COLORMAP[1]),
		new Vertex(Vector3(a / 2, b / 2, c / 2), COLORMAP[2]),
		new Vertex(Vector3(-a / 2, b / 2, c / 2), COLORMAP[0]),
		new Vertex(Vector3(-a / 2, b / 2, -c / 2), COLORMAP[1]),
	};

	std::vector<std::vector<int>> faces = {
		{0, 1, 2, 3},
		{7, 6, 5, 4},
		{4, 5, 1, 0},
		{6, 7, 3, 2},
		{5, 6, 2, 1},
		{7, 4, 0, 3},
	};

	Mesh *mesh = new Mesh();
	mesh->build(vertices, faces);

	Object *cube = new Object();
	cube->add(new MeshInstance(mesh));
	cube->rotation.set(90.0f, 0.0f, 0.0f);

	add(cube);
}

Scene::~Scene()
{
	for (Object *object : this->objects)
	{
		delete object;
	}
}

class Game;

extern Game gGame;

class Game
{
public:
	Scene *scene;

	Game();
	void init();
	void run(int argc, char **argv);
	~Game();

	static void displayCallback()
	{
		// draw the scene again
		gGame.scene->draw();
	}

	static void idleCallback()
	{
		glutPostRedisplay();
	}
};

Game::Game()
{
	this->scene = new Scene();
}

void Game::init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	this->scene->init();
}

void Game::run(int argc, char **argv)
{
	// set up the OpenGL context with glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(ID);

	// init game state -> init the scene
	init();

	glutDisplayFunc(displayCallback);

	glutMainLoop();
}

Game::~Game()
{
	delete this->scene;
}

Game gGame;

int main(int argc, char **argv)
{
	globalInit();

	// run the game
	gGame.run(argc, argv);

	return 0;
}