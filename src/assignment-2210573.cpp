// Lê Bùi Trung Dũng - 2210573
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <vector>

// ###########################################################
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
// ###########################################################

// ###########################################################
// ###########################################################
int WIDTH = 800;
int HEIGHT = 800;
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
	void build(const std::vector<Vertex *> &inVertices, const std::vector<std::vector<int>> &inFaces);
	void drawWire();
	void drawColor();
	void draw(bool isColour);
	~Mesh();
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

void Mesh::draw(bool isColour = false)
{
	if (isColour == true)
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
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glLineWidth(2.0f);
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
}

class MeshFactory
{
public:
	static Mesh *cube();
	static Mesh *buildShape4(float a, float b);
	static Mesh *buildShape5(float a, float b);
	static Mesh *buildShape1(float a, int n, int idx1, int idx2);
	static Mesh *buildShape2(float a, int n, int idx1, int idx2, float size, int range);
	static Mesh *buildShape3(float a);
	static Mesh *cylinder(int n);
};

Mesh *MeshFactory::cube()
{
	std::vector<Vertex *> vertices = {
		new Vertex(Vector3(0.5f, -0.5f, -0.5f), COLORMAP[0]),
		new Vertex(Vector3(0.5f, -0.5f, 0.5f), COLORMAP[1]),
		new Vertex(Vector3(-0.5f, -0.5f, 0.5f), COLORMAP[2]),
		new Vertex(Vector3(-0.5f, -0.5f, -0.5f), COLORMAP[0]),
		new Vertex(Vector3(0.5f, 0.5f, -0.5f), COLORMAP[1]),
		new Vertex(Vector3(0.5f, 0.5f, 0.5f), COLORMAP[2]),
		new Vertex(Vector3(-0.5f, 0.5f, 0.5f), COLORMAP[0]),
		new Vertex(Vector3(-0.5f, 0.5f, -0.5f), COLORMAP[1]),
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

	return mesh;
}

Mesh *MeshFactory::buildShape4(float a, float b)
{

	std::vector<Vertex *> vertices = {
		new Vertex(Vector3(0.5f, -0.5f, -0.5f), COLORMAP[0]),
		new Vertex(Vector3(0.5f, -0.5f, 0.5f), COLORMAP[0]),
		new Vertex(Vector3(-0.5f, -0.5f, 0.5f), COLORMAP[0]),
		new Vertex(Vector3(-0.5f, -0.5f, -0.5f), COLORMAP[0]),

		new Vertex(Vector3(a * 0.5f, b * -0.5f, a * -0.5f), COLORMAP[0]),
		new Vertex(Vector3(a * 0.5f, b * -0.5f, a * 0.5f), COLORMAP[0]),
		new Vertex(Vector3(-a * 0.5f, b * -0.5f, a * 0.5f), COLORMAP[0]),
		new Vertex(Vector3(-a * 0.5f, b * -0.5f, a * -0.5f), COLORMAP[0]),

		new Vertex(Vector3(0.5f, 0.5f, -0.5f), COLORMAP[0]),
		new Vertex(Vector3(0.5f, 0.5f, 0.5f), COLORMAP[0]),
		new Vertex(Vector3(-0.5f, 0.5f, 0.5f), COLORMAP[0]),
		new Vertex(Vector3(-0.5f, 0.5f, -0.5f), COLORMAP[0]),

		new Vertex(Vector3(a * 0.5f, b * 0.5f, a * -0.5f), COLORMAP[0]),
		new Vertex(Vector3(a * 0.5f, b * 0.5f, a * 0.5f), COLORMAP[0]),
		new Vertex(Vector3(-a * 0.5f, b * 0.5f, a * 0.5f), COLORMAP[0]),
		new Vertex(Vector3(-a * 0.5f, b * 0.5f, a * -0.5f), COLORMAP[0]),
	};

	std::vector<std::vector<int>> faces = {
		{0, 8, 9, 1},
		{9, 10, 2, 1},
		{2, 10, 11, 3},
		{8, 0, 3, 11},
		{8, 12, 13, 9},
		{13, 14, 10, 9},
		{15, 11, 10, 14},
		{8, 11, 15, 12},
		{1, 5, 4, 0},
		{1, 2, 6, 5},
		{2, 3, 7, 6},
		{7, 3, 0, 4},
		{12, 15, 7, 4},
		{15, 14, 6, 7},
		{14, 13, 5, 6},
		{13, 12, 4, 5},
	};

	Mesh *mesh = new Mesh();
	mesh->build(vertices, faces);
	return mesh;
}

Mesh *MeshFactory::buildShape5(float a, float b)
{
	std::vector<Vertex *> vertices = {
		new Vertex(Vector3(0.5f, -0.5f, -0.5f), COLORMAP[0]),
		new Vertex(Vector3(0.5f, -0.5f, 0.5f), COLORMAP[0]),
		new Vertex(Vector3(-0.5f, -0.5f, 0.5f), COLORMAP[0]),
		new Vertex(Vector3(-0.5f, -0.5f, -0.5f), COLORMAP[0]),

		new Vertex(Vector3(0.5f, 0.5f, -0.5f), COLORMAP[0]),
		new Vertex(Vector3(0.5f, 0.5f, a - 0.5f), COLORMAP[0]),
		new Vertex(Vector3(-0.5f, 0.5f, a - 0.5f), COLORMAP[0]),
		new Vertex(Vector3(-0.5f, 0.5f, -0.5f), COLORMAP[0]),

		new Vertex(Vector3(0.5f, b - 0.5f, 0.5f), COLORMAP[0]),
		new Vertex(Vector3(-0.5f, b - 0.5f, 0.5f), COLORMAP[0]),
	};

	std::vector<std::vector<int>> faces = {
		{4, 5, 8, 1, 0},
		{6, 7, 3, 2, 9},
		{5, 6, 9, 8},
		{8, 9, 2, 1},
		{2, 3, 0, 1},
		{4, 0, 3, 7},
		{4, 7, 6, 5},
	};

	Mesh *mesh = new Mesh();
	mesh->build(vertices, faces);

	return mesh;
}

Mesh *MeshFactory::buildShape1(float a, int n, int idx1, int idx2)
{
	std::vector<Vertex *> vertices;
	std::vector<std::vector<int>> faces;

	float alpha = 0.0f;
	float dt = 2 * M_PI / n;
	for (int i = 0; i < n; ++i)
	{
		if (i >= idx1 && i <= idx2)
		{
			vertices.push_back(new Vertex(Vector3(a * std::cos(alpha), -0.5f, a * std::sin(alpha)), COLORMAP[0]));
		}
		else
		{
			vertices.push_back(new Vertex(Vector3(std::cos(alpha), -0.5f, std::sin(alpha)), COLORMAP[0]));
		}
		alpha += dt;
	}

	alpha = 0.0f;
	for (int i = 0; i < n; ++i)
	{
		if (i >= idx1 && i <= idx2)
		{
			vertices.push_back(new Vertex(Vector3(a * std::cos(alpha), 0.5f, a * std::sin(alpha)), COLORMAP[0]));
		}
		else
		{
			vertices.push_back(new Vertex(Vector3(std::cos(alpha), 0.5f, std::sin(alpha)), COLORMAP[0]));
		}
		alpha += dt;
	}

	vertices.push_back(new Vertex(Vector3(0.0f, -0.5f, 0.0f), COLORMAP[0]));
	vertices.push_back(new Vertex(Vector3(0.0f, 0.5f, 0.0f), COLORMAP[0]));

	for (int i = 0; i < n; ++i)
	{
		std::vector<int> face = {i, (i + 1) % n, 2 * n};
		faces.push_back(face);
	}

	for (int i = n; i < 2 * n; ++i)
	{
		std::vector<int> face = {i, 2 * n + 1, (i + 1) % (2 * n)};
		faces.push_back(face);
	}

	for (int i = 0; i < n; ++i)
	{
		std::vector<int> face = {i, i + n, (i + n + 1) % n + n, (i + 1) % n};
		faces.push_back(face);
	}

	Mesh *mesh = new Mesh();
	mesh->build(vertices, faces);

	return mesh;
}

Mesh *MeshFactory::buildShape2(float a, int n, int idx1 = 45, int idx2 = 134, float size = 0.2, int range = 15)
{
	std::vector<Vertex *> vertices;
	std::vector<std::vector<int>> faces;

	float alpha = 0.0f;
	float delta = 2 * M_PI / n;
	int numberSlices = idx2 - idx1 + 1;
	vertices.resize((numberSlices + 1) * 8);

	for (int i = 0; i < n; ++i)
	{
		// loop idx2 - idx1 + 1
		if (i >= idx1 && i <= (idx2 + 1))
		{
			vertices[i - idx1] = new Vertex(Vector3(std::cos(alpha), -0.5f, std::sin(alpha)), COLORMAP[0]);
			vertices[i + numberSlices - idx1 + 1] = new Vertex(Vector3((1.0f + size) * std::cos(alpha), -0.5f, (1.0f + size) * std::sin(alpha)), COLORMAP[0]);
			vertices[i + 2 * numberSlices - idx1 + 2] = new Vertex(Vector3(a * std::cos(alpha), -0.5f, a * std::sin(alpha)), COLORMAP[0]);
			vertices[i + 3 * numberSlices - idx1 + 3] = new Vertex(Vector3((a + size) * std::cos(alpha), -0.5f, (a + size) * std::sin(alpha)), COLORMAP[0]);

			vertices[i + 4 * numberSlices - idx1 + 4] = new Vertex(Vector3(std::cos(alpha), 0.5f, std::sin(alpha)), COLORMAP[0]);
			vertices[i + 5 * numberSlices - idx1 + 5] = new Vertex(Vector3((1.0f + size) * std::cos(alpha), 0.5f, (1.0f + size) * std::sin(alpha)), COLORMAP[0]);
			vertices[i + 6 * numberSlices - idx1 + 6] = new Vertex(Vector3(a * std::cos(alpha), 0.5f, a * std::sin(alpha)), COLORMAP[0]);
			vertices[i + 7 * numberSlices - idx1 + 7] = new Vertex(Vector3((a + size) * std::cos(alpha), 0.5f, (a + size) * std::sin(alpha)), COLORMAP[0]);
		}
		alpha += delta;
	}

	for (int i = 0; i < numberSlices; ++i)
	{
		std::vector<int> face1 = {i, i + 1, i + numberSlices + 1 + 1, i + numberSlices + 1};
		std::vector<int> face2 = {i + 2 * numberSlices + 2, i + 2 * numberSlices + 2 + 1, i + 3 * numberSlices + 3 + 1, i + 3 * numberSlices + 3};
		std::vector<int> face3 = {i + 4 * numberSlices + 4, i + 4 * numberSlices + 4 + 1, i + 5 * numberSlices + 5 + 1, i + 5 * numberSlices + 5};
		std::vector<int> face4 = {i + 6 * numberSlices + 6, i + 6 * numberSlices + 6 + 1, i + 7 * numberSlices + 7 + 1, i + 7 * numberSlices + 7};

		std::vector<int> face5 = {i, i + 1, i + 4 * numberSlices + 4 + 1, i + 4 * numberSlices + 4};
		std::vector<int> face6 = {i + 1 * numberSlices + 1, i + 5 * numberSlices + 5, i + 5 * numberSlices + 5 + 1, i + 1 * numberSlices + 1 + 1};
		std::vector<int> face7 = {i + 2 * numberSlices + 2, i + 2 * numberSlices + 2 + 1, i + 6 * numberSlices + 6 + 1, i + 6 * numberSlices + 6};
		std::vector<int> face8 = {i + 3 * numberSlices + 3, i + 7 * numberSlices + 7, i + 7 * numberSlices + 7 + 1, i + 3 * numberSlices + 3 + 1};

		if ((i >= 0 && i < range) || (i >= numberSlices - range && i < numberSlices))
		{
			// 0 - 9 and 80 - 89
			std::vector<int> face1 = {i + 1 * numberSlices + 1, i + 2 * numberSlices + 2, i + 2 * numberSlices + 2 + 1, i + 1 * numberSlices + 1 + 1};
			std::vector<int> face2 = {i + 6 * numberSlices + 6, i + 5 * numberSlices + 5, i + 5 * numberSlices + 5 + 1, i + 6 * numberSlices + 6 + 1};

			faces.push_back(face1);
			faces.push_back(face2);
		}

		faces.push_back(face1);
		faces.push_back(face2);
		faces.push_back(face3);
		faces.push_back(face4);
		faces.push_back(face5);
		faces.push_back(face6);
		faces.push_back(face7);
		faces.push_back(face8);
	}

	Mesh *mesh = new Mesh();
	mesh->build(vertices, faces);

	return mesh;
}

Mesh *MeshFactory::buildShape3(float a)
{
	std::vector<Vertex *> vertices;

	vertices.push_back(new Vertex(Vector3(0.5f, -0.5f, -a / 2), COLORMAP[0]));
	vertices.push_back(new Vertex(Vector3(0.5f, -0.5f, a / 2), COLORMAP[0]));
	vertices.push_back(new Vertex(Vector3(a / 2, -0.5f, 0.5f), COLORMAP[0]));
	vertices.push_back(new Vertex(Vector3(-a / 2, -0.5f, 0.5f), COLORMAP[0]));
	vertices.push_back(new Vertex(Vector3(-0.5f, -0.5f, a / 2), COLORMAP[0]));
	vertices.push_back(new Vertex(Vector3(-0.5f, -0.5f, -a / 2), COLORMAP[0]));
	vertices.push_back(new Vertex(Vector3(-a / 2, -0.5f, -0.5f), COLORMAP[0]));
	vertices.push_back(new Vertex(Vector3(a / 2, -0.5f, -0.5f), COLORMAP[0]));

	vertices.push_back(new Vertex(Vector3(0.5f, 0.5f, -a / 2), COLORMAP[0]));
	vertices.push_back(new Vertex(Vector3(0.5f, 0.5f, a / 2), COLORMAP[0]));
	vertices.push_back(new Vertex(Vector3(a / 2, 0.5f, 0.5f), COLORMAP[0]));
	vertices.push_back(new Vertex(Vector3(-a / 2, 0.5f, 0.5f), COLORMAP[0]));
	vertices.push_back(new Vertex(Vector3(-0.5f, 0.5f, a / 2), COLORMAP[0]));
	vertices.push_back(new Vertex(Vector3(-0.5f, 0.5f, -a / 2), COLORMAP[0]));
	vertices.push_back(new Vertex(Vector3(-a / 2, 0.5f, -0.5f), COLORMAP[0]));
	vertices.push_back(new Vertex(Vector3(a / 2, 0.5f, -0.5f), COLORMAP[0]));

	std::vector<std::vector<int>> faces;
	faces.push_back(std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7});
	faces.push_back(std::vector<int>{15, 14, 13, 12, 11, 10, 9, 8});

	faces.push_back(std::vector<int>{8, 9, 1, 0});
	faces.push_back(std::vector<int>{9, 10, 2, 1});
	faces.push_back(std::vector<int>{10, 11, 3, 2});
	faces.push_back(std::vector<int>{11, 12, 4, 3});
	faces.push_back(std::vector<int>{12, 13, 5, 4});
	faces.push_back(std::vector<int>{13, 14, 6, 5});
	faces.push_back(std::vector<int>{14, 15, 7, 6});
	faces.push_back(std::vector<int>{8, 0, 7, 15});

	Mesh *mesh = new Mesh();
	mesh->build(vertices, faces);

	return mesh;
}

Mesh *MeshFactory::cylinder(int n)
{
	float alpha = 0.0f;
	float delta = 2 * M_PI / n;
	std::vector<Vertex *> vertices;
	std::vector<std::vector<int>> faces;
	vertices.resize(n + n + 1 + 1);

	for (int i = 0; i < n; ++i)
	{
		vertices[i] = new Vertex(Vector3(std::cos(alpha), -0.5f, std::sin(alpha)), COLORMAP[0]);
		vertices[i + n] = new Vertex(Vector3(std::cos(alpha), 0.5f, std::sin(alpha)), COLORMAP[0]);

		alpha += delta;
	}

	vertices[2 * n] = (new Vertex(Vector3(0.0f, -0.5f, 0.0f), COLORMAP[0]));
	vertices[2 * n + 1] = (new Vertex(Vector3(0.0f, 0.5f, 0.0f), COLORMAP[0]));

	for (int i = 0; i < n; ++i)
	{
		std::vector<int> face1 = {i, (i + 1) % n, 2 * n};
		std::vector<int> face2 = {(i + n + 1) % n + n, i + n, 2 * n + 1};
		std::vector<int> face3 = {(i + 1) % n, i, i + n, (i + n + 1) % n + n};

		faces.push_back(face1);
		faces.push_back(face2);
		faces.push_back(face3);
	}

	Mesh *mesh = new Mesh();
	mesh->build(vertices, faces);

	return mesh;
}

class MeshInstance
{
public:
	Mesh *mesh;
	Vector3 localPosition;
	Vector3 localRotation;
	Vector3 localScale;

	MeshInstance(Mesh *m) : mesh(m), localPosition(), localRotation(), localScale(1.0f, 1.0f, 1.0f) {}

	void draw(bool isColour);
};

void MeshInstance::draw(bool isColour = false)
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

	mesh->draw(isColour);
	glPopMatrix();
}

// ###########################################################
// ###########################################################
class Object
{
public:
	std::vector<MeshInstance *> parts;

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	Object();
	void add(MeshInstance *mesh);
	void draw(bool isColour);
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

void Object::draw(bool isColour = false)
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
		part->draw(isColour);
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
// ###########################################################
// ###########################################################

// ###########################################################
// ###########################################################
class Camera
{
public:
	Vector3 position;
	Vector3 target;
	Vector3 up;

	float velocity;
	float omega;
	float radius;
	float angle;
	float height;

	Camera() : position(0, 0, 5), target(0, 0, 0), up(0, 1, 0), velocity{1.0f}, radius(7.0f), angle(M_PI / 2), omega{M_PI / 8}, height{0.0f}
	{
	}

	void apply();
};

void Camera::apply()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		position.x,
		position.y,
		position.z,
		target.x,
		target.y,
		target.z,
		up.x,
		up.y,
		up.z);
}
// ###########################################################
// ###########################################################

// ###########################################################
// ###########################################################
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
// ###########################################################
// ###########################################################

// ###########################################################
// ###########################################################
class Scene
{
public:
	std::vector<Object *> objects;
	Camera *camera;
	bool isColour;

	Scene();
	void add(Object *object);
	void drawAxes();
	void draw();
	void update();
	void init();
	~Scene();
};

Scene::Scene() : isColour{false}, camera{new Camera()}
{
}

void Scene::add(Object *object)
{
	this->objects.push_back(object);
}

void Scene::drawAxes()
{
	glPushMatrix();

	GLfloat length = 10.0f;
	glLineWidth(5.0f);
	glBegin(GL_LINES);

	// X axis (red)
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-length, 0.0f, 0.0f);
	glVertex3f(length, 0.0f, 0.0f);

	// Y axis (green)
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -length, 0.0f);
	glVertex3f(0.0f, length, 0.0f);

	// Z axis (blue)
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -length);
	glVertex3f(0.0f, 0.0f, length);

	glEnd();

	glPopMatrix();
}

void Scene::draw()
{
	// set up background color
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// set up projection method
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(
		60.0f,
		(float)WIDTH / (float)HEIGHT,
		0.1f,
		100.0f);

	// set up current camera
	this->camera->apply();

	// start drawing
	drawAxes();

	for (Object *object : objects)
	{
		object->draw(this->isColour);
	}

	glutSwapBuffers();
}

void Scene::init()
{
	// set up the initial objects
	Mesh *cube = MeshFactory::cube();
	Object *cubeObj = new Object();
	cubeObj->add(new MeshInstance(cube));
	cubeObj->scale.set(0.5f, 0.5f, 0.5f);
	cubeObj->position.set(-3.0f, 0.0f, -3.0f);

	Mesh *shape4 = MeshFactory::buildShape4(0.8f, 0.8f);
	Object *longTunnel = new Object();
	longTunnel->add(new MeshInstance(shape4));
	longTunnel->scale.set(1.0f, 2.0f, 1.0f);
	longTunnel->position.set(-3.0f, 0.0f, 3.0f);

	Mesh *shape5 = MeshFactory::buildShape5(0.8f, 0.95f);
	Object *longTube = new Object();
	longTube->add(new MeshInstance(shape5));
	longTube->scale.set(0.5f, 5.0f, 0.5f);
	longTube->position.set(2.0f, 0.0f, -4.0f);

	Mesh *shape1 = MeshFactory::buildShape1(1.5f, 36, 3, 9);
	Object *fan = new Object();
	fan->add(new MeshInstance(shape1));
	fan->scale.set(1.5f, 0.25f, 1.5f);
	fan->position.set(-4.0, 0.0f, -4.0f);

	Mesh *shape2 = MeshFactory::buildShape2(1.5f, 36, 4, 12, 0.2f, 3);
	Object *wheel = new Object();
	wheel->add(new MeshInstance(shape2));
	wheel->position.set(2.0f, 0.0f, 2.0f);
	wheel->scale.set(1.0f, 0.25f, 1.0f);
	wheel->rotation.set(0.0, 0.0f, 0.0f);

	Mesh *cylinder = MeshFactory::cylinder(10);
	Object *cyl = new Object();
	cyl->add(new MeshInstance(cylinder));
	cyl->scale.set(0.5f, 3.0f, 0.5f);
	cyl->position.set(4.0f, 1.0f, 2.0f);

	Mesh *shape3 = MeshFactory::buildShape3(0.7f);
	Object *magicBox = new Object();
	magicBox->add(new MeshInstance(shape3));
	magicBox->scale.set(1.0f, 0.4f, 1.0f);
	magicBox->rotation.set(0.0f, 30.0f, 0.0f);
	magicBox->position.set(1.0f, 0.0f, 5.0f);

	// add objects to the initial scene
	this->add(cubeObj);
	this->add(longTunnel);
	this->add(longTube);
	this->add(fan);
	this->add(wheel);
	this->add(cyl);
	this->add(magicBox);

	float angle = M_PI / 2;
	float height = 0.0f;
	float radius = 7.0f;

	// initial state
	this->camera->angle = angle;
	this->camera->height = height;
	this->camera->radius = radius;

	this->camera->position.set(radius * std::cos(angle), height, radius * std::sin(angle));
	// fixed
	this->camera->target.set(0.0f, 0.0f, 0.0f);
	this->camera->up.set(0.0f, 1.0f, 0.0f);
}

Scene::~Scene()
{
	for (Object *object : this->objects)
	{
		delete object;
	}

	delete camera;
}
// ###########################################################
// ###########################################################

// ###########################################################
// ###########################################################
class Game
{
public:
	enum Action
	{
		ACTION_NONE = 0,
		ACTION_MOVE_CAM_CCW,
		ACTION_MOVE_CAM_CW,
		ACTION_MOVE_CAM_UP,
		ACITON_MOVE_CAM_DOWN,
		ACTION_MOVE_CAM_FORWARD,
		ACTION_MOVE_CAM_BACKWARD,
		ACTION_COLOUR
	};
	Scene *scene;
	double lastTime;
	bool *actions;

	Game();
	void init();
	void run(int argc, char **argv);
	void update();
	void render();
	~Game();
};

Game::Game()
{
	this->scene = new Scene();
	this->actions = new bool[100];
	for (int i = 0; i < 100; ++i)
	{
		this->actions[i] = false;
	}
}

void Game::init()
{
	// init game
	this->lastTime = 0.0;

	// init the initial scene and meshes
	this->scene->init();
}

void Game::update()
{
	// use glut utility to get elapse time
	double now = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	// compute the delta
	double dt = now - lastTime;

	// avoid large delta time
	if (dt < 0.0 || dt > 0.5)
		dt = 0.0;

	if (this->actions[ACTION_MOVE_CAM_UP])
	{
		this->scene->camera->height += dt * this->scene->camera->velocity;
		this->scene->camera->position.y = this->scene->camera->height;
	}

	if (this->actions[ACITON_MOVE_CAM_DOWN])
	{
		this->scene->camera->height -= dt * this->scene->camera->velocity;
		this->scene->camera->position.y = this->scene->camera->height;
	}

	if (this->actions[ACTION_MOVE_CAM_FORWARD])
	{
		float angle = this->scene->camera->angle;
		this->scene->camera->radius -= dt * this->scene->camera->velocity;
		float radius = this->scene->camera->radius;

		this->scene->camera->position.x = radius * std::cos(angle);
		this->scene->camera->position.z = radius * std::sin(angle);
	}

	if (this->actions[ACTION_MOVE_CAM_BACKWARD])
	{
		float angle = this->scene->camera->angle;
		this->scene->camera->radius += dt * this->scene->camera->velocity;
		float radius = this->scene->camera->radius;

		this->scene->camera->position.x = radius * std::cos(angle);
		this->scene->camera->position.z = radius * std::sin(angle);
	}

	if (this->actions[ACTION_MOVE_CAM_CW])
	{
		float radius = this->scene->camera->radius;
		this->scene->camera->angle += dt * this->scene->camera->omega;
		float angle = this->scene->camera->angle;
		this->scene->camera->position.x = radius * std::cos(angle);
		this->scene->camera->position.z = radius * std::sin(angle);
	}

	if (this->actions[ACTION_MOVE_CAM_CCW])
	{
		float radius = this->scene->camera->radius;
		this->scene->camera->angle -= dt * this->scene->camera->omega;
		float angle = this->scene->camera->angle;
		this->scene->camera->position.x = radius * std::cos(angle);
		this->scene->camera->position.z = radius * std::sin(angle);
	}

	this->scene->isColour = actions[ACTION_COLOUR];

	// update lastTime
	this->lastTime = now;
}

// the main display function
void Game::render()
{
	// map the whole viewport
	glViewport(0, 0, WIDTH, HEIGHT);
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// drawing vertices to pixels
	scene->draw();

	// swap buffer
	glutSwapBuffers();
}

Game::~Game()
{
	delete this->scene;
}
// ###########################################################
// ###########################################################

static Game gGame;

// ###########################################################
// ###########################################################
void displayCallback()
{
	gGame.render();
}

void idleCallback()
{
	// update game logic
	gGame.update();
	// tell OpenGL to redraw the scene's vertices
	glutPostRedisplay();
}

void onKeyDown(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '+':
		gGame.actions[Game::ACTION_MOVE_CAM_FORWARD] = true;
		break;
	case '-':
		gGame.actions[Game::ACTION_MOVE_CAM_BACKWARD] = true;
		break;
	case 'w':
	case 'W':
		gGame.actions[Game::ACTION_COLOUR] = !gGame.actions[Game::ACTION_COLOUR];
		break;
	}
}

void onKeyUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '+':
		gGame.actions[Game::ACTION_MOVE_CAM_FORWARD] = false;
		break;
	case '-':
		gGame.actions[Game::ACTION_MOVE_CAM_BACKWARD] = false;
		break;
	}
}

void onSpecialDown(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		gGame.actions[Game::ACTION_MOVE_CAM_UP] = true;
		break;
	case GLUT_KEY_DOWN:
		gGame.actions[Game::ACITON_MOVE_CAM_DOWN] = true;
		break;
	case GLUT_KEY_LEFT:
		gGame.actions[Game::ACTION_MOVE_CAM_CW] = true;
		break;
	case GLUT_KEY_RIGHT:
		gGame.actions[Game::ACTION_MOVE_CAM_CCW] = true;
		break;
	}
}

void onSpecialUp(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		gGame.actions[Game::ACTION_MOVE_CAM_UP] = false;
		break;
	case GLUT_KEY_DOWN:
		gGame.actions[Game::ACITON_MOVE_CAM_DOWN] = false;
		break;
	case GLUT_KEY_LEFT:
		gGame.actions[Game::ACTION_MOVE_CAM_CW] = false;
		break;
	case GLUT_KEY_RIGHT:
		gGame.actions[Game::ACTION_MOVE_CAM_CCW] = false;
		break;
	}
}
// ###########################################################
// ###########################################################

int main(int argc, char **argv)
{
	// global initialization
	globalInit();

	// set up the OpenGL context with glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(ID);

	// init game
	gGame.init();

	// register callback for display
	// and idle to update the game logic
	glutDisplayFunc(displayCallback);
	glutIdleFunc(idleCallback);

	// register callback for inputs
	glutKeyboardFunc(&onKeyDown);
	glutKeyboardUpFunc(&onKeyUp);
	glutSpecialFunc(&onSpecialDown);
	glutSpecialUpFunc(&onSpecialUp);

	// start the whole loop
	// normally, this loop will re-render
	// on demand
	glutMainLoop();

	return 0;
}