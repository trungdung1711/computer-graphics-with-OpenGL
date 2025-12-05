// Lê Bùi Trung Dũng - 2210573
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <map>

// ###########################################################
// ###########################################################
class Vector3
{
public:
	float x, y, z;

	Vector3();
	Vector3(float x, float y, float z);
	Vector3(Vector3 &v);
	Vector3(const Vector3 &v);
	void set(float x, float y, float z);
	void set(Vector3 &v);
	void flip();
	void normalize();
	Vector3 cross(Vector3 &b);
	float dot(Vector3 &b);
	Vector3 operator+(Vector3 v);
	Vector3 operator-(const Vector3 &v);
	Vector3 &operator=(const Vector3 &v);
	static Vector3 cross(Vector3 a, Vector3 b);
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

Vector3 Vector3::operator+(Vector3 v)
{
	return Vector3(this->x + v.x, this->y + v.y, this->z + v.z);
}

Vector3 Vector3::operator-(const Vector3 &v)
{
	return Vector3(this->x - v.x, this->y - v.y, this->z - v.z);
}

Vector3::Vector3(const Vector3 &v) : x{v.x}, y{v.y}, z{v.z}
{
}

Vector3 &Vector3::operator=(const Vector3 &other)
{
	if (this != &other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
	}

	return *this;
}

Vector3 Vector3::cross(Vector3 a, Vector3 b)
{
	return Vector3(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - b.y * b.x);
}

class Vector4
{
public:
	float x, y, z, w;

	Vector4()
		: x(0), y(0), z(0), w(1) {}

	Vector4(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w) {}

	Vector4(const Vector4 &v)
		: x(v.x), y(v.y), z(v.z), w(v.w) {}

	void set(float x, float y, float z, float w);
	void set(const Vector4 &v);

	const float *data() const;
};

void Vector4::set(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

void Vector4::set(const Vector4 &v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	this->w = v.w;
}

const float *Vector4::data() const
{
	return &x;
}
// ###########################################################
// ###########################################################

// ###########################################################
// ###########################################################
int WINDOW_X = 1200;
int WINDOW_Y = 0;

int WIDTH = 800;
int HEIGHT = 800;
std::map<std::string, Vector3 *> COLORS;
const char *ID = "Le Bui Trung Dung - 2210573";
Vector3 *COLORMAP;

bool segments[10][7] = {
	// A, B, C, D, E, F, G
	{1, 1, 1, 1, 1, 1, 0}, // 0
	{0, 1, 1, 0, 0, 0, 0}, // 1
	{1, 1, 0, 1, 1, 0, 1}, // 2
	{1, 1, 1, 1, 0, 0, 1}, // 3
	{0, 1, 1, 0, 0, 1, 1}, // 4
	{1, 0, 1, 1, 0, 1, 1}, // 5
	{1, 0, 1, 1, 1, 1, 1}, // 6
	{1, 1, 1, 0, 0, 0, 0}, // 7
	{1, 1, 1, 1, 1, 1, 1}, // 8
	{1, 1, 1, 1, 0, 1, 1}, // 9
};

void globalInit()
{
	COLORMAP = new Vector3[100];
	COLORMAP[0] = {1.0f, 0.0f, 0.0f}; // red
	COLORMAP[1] = {0.0f, 1.0f, 0.0f}; // green
	COLORMAP[2] = {0.0f, 0.0f, 1.0f}; // blue
	COLORS["red"] = new Vector3{1.0f, 0.0f, 0.0f};
}

void clean()
{
	delete[] COLORMAP;
	for (auto color : COLORS)
	{
		delete color.second;
	}
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

Vertex::Vertex(Vector3 position, Vector3 normal)
{
	this->position = position;
	this->normal = normal;
}

Vertex::Vertex(Vector3 position, int colorIndex)
{
	this->position = position;
	this->colorIndex = colorIndex;
}

class Material
{
public:
	Vector4 ambient;
	Vector4 diffuse;
	Vector4 specular;
	Vector4 emission;
	float shininess;

	Material();
	void apply();
};

Material::Material()
{
	// default material
	ambient.set(0.2f, 0.2f, 0.2f, 1.0f);
	diffuse.set(0.8f, 0.8f, 0.8f, 1.0f);
	specular.set(0.5f, 0.5f, 0.5f, 1.0f);
	emission.set(0.0f, 0.0f, 0.0f, 1.0f);

	shininess = 32.0f;
}

void Material::apply()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient.data());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse.data());
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular.data());
	glMaterialfv(GL_FRONT, GL_EMISSION, emission.data());
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

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

	glLineWidth(2.0f);
	for (const std::vector<int> &face : faces)
	{
		glBegin(GL_POLYGON);
		for (int idx : face)
		{
			Vertex *v = vertices[idx];
			// blue
			glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
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
		drawColor();
	}
	else
	{
		drawWire();
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
	std::vector<Vector3> P = {
		{0.5f, -0.5f, -0.5f},  // 0
		{0.5f, -0.5f, 0.5f},   // 1
		{-0.5f, -0.5f, 0.5f},  // 2
		{-0.5f, -0.5f, -0.5f}, // 3

		{0.5f, 0.5f, -0.5f},  // 4
		{0.5f, 0.5f, 0.5f},	  // 5
		{-0.5f, 0.5f, 0.5f},  // 6
		{-0.5f, 0.5f, -0.5f}, // 7
	};

	std::vector<std::vector<int>> F = {
		{0, 1, 2, 3}, // bottom
		{7, 6, 5, 4}, // top
		{4, 5, 1, 0}, // +X
		{6, 7, 3, 2}, // -X
		{5, 6, 2, 1}, // +Z
		{7, 4, 0, 3}  // -Z
	};

	std::vector<Vertex *> vertices;
	std::vector<std::vector<int>> faces;

	auto computeNormal = [&](Vector3 a, Vector3 b, Vector3 c)
	{
		Vector3 u = b - a;
		Vector3 v = c - a;
		Vector3 n = u.cross(v);
		n.normalize();
		return n;
	};

	for (auto &face : F)
	{
		Vector3 N = computeNormal(P[face[0]], P[face[1]], P[face[2]]);
		// std::cout << "Normal: " << N.x << ", " << N.y << ", " << N.z << "\n";

		std::vector<int> newFace;

		for (int idx : face)
		{
			Vertex *v = new Vertex(P[idx], N);
			vertices.push_back(v);
			newFace.push_back(vertices.size() - 1);
		}

		faces.push_back(newFace);
	}

	Mesh *mesh = new Mesh();
	mesh->build(vertices, faces);

	return mesh;
}

Mesh *MeshFactory::buildShape4(float a, float b)
{
	std::vector<Vector3> P = {
		{0.5f, -0.5f, -0.5f},
		{0.5f, -0.5f, 0.5f},
		{-0.5f, -0.5f, 0.5f},
		{-0.5f, -0.5f, -0.5f},

		{a * 0.5f, b * -0.5f, a * -0.5f},
		{a * 0.5f, b * -0.5f, a * 0.5f},
		{-a * 0.5f, b * -0.5f, a * 0.5f},
		{-a * 0.5f, b * -0.5f, a * -0.5f},

		{0.5f, 0.5f, -0.5f},
		{0.5f, 0.5f, 0.5f},
		{-0.5f, 0.5f, 0.5f},
		{-0.5f, 0.5f, -0.5f},

		{a * 0.5f, b * 0.5f, a * -0.5f},
		{a * 0.5f, b * 0.5f, a * 0.5f},
		{-a * 0.5f, b * 0.5f, a * 0.5f},
		{-a * 0.5f, b * 0.5f, a * -0.5f},
	};

	std::vector<std::vector<int>> F = {
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

	std::vector<Vertex *> vertices;
	std::vector<std::vector<int>> faces;

	for (auto &f : F)
	{
		int start = vertices.size();
		int k = f.size();

		Vector3 p0 = P[f[0]];
		Vector3 p1 = P[f[1]];
		Vector3 p2 = P[f[2]];
		Vector3 n = Vector3::cross(p1 - p0, p2 - p0);
		n.normalize();

		for (int vid : f)
		{
			vertices.push_back(new Vertex(P[vid], n));
		}

		std::vector<int> newFace;
		for (int i = 0; i < k; i++)
			newFace.push_back(start + i);

		faces.push_back(newFace);
	}

	Mesh *mesh = new Mesh();
	mesh->build(vertices, faces);
	return mesh;
}

Mesh *MeshFactory::buildShape5(float a, float b)
{
	std::vector<Vector3> P = {
		{0.5f, -0.5f, -0.5f},  // 0
		{0.5f, -0.5f, 0.5f},   // 1
		{-0.5f, -0.5f, 0.5f},  // 2
		{-0.5f, -0.5f, -0.5f}, // 3

		{0.5f, 0.5f, -0.5f},	 // 4
		{0.5f, 0.5f, a - 0.5f},	 // 5
		{-0.5f, 0.5f, a - 0.5f}, // 6
		{-0.5f, 0.5f, -0.5f},	 // 7

		{0.5f, b - 0.5f, 0.5f},	 // 8
		{-0.5f, b - 0.5f, 0.5f}, // 9
	};

	std::vector<std::vector<int>> F = {
		{4, 5, 8, 1, 0},
		{6, 7, 3, 2, 9},
		{5, 6, 9, 8},
		{8, 9, 2, 1},
		{2, 3, 0, 1},
		{4, 0, 3, 7},
		{4, 7, 6, 5},
	};

	std::vector<Vertex *> vertices;
	std::vector<std::vector<int>> faces;

	auto computeNormal = [&](Vector3 a, Vector3 b, Vector3 c)
	{
		Vector3 u = Vector3(b - a);
		Vector3 v = c - a;
		Vector3 n = u.cross(v);
		n.normalize();
		return n;
	};

	for (auto &face : F)
	{
		Vector3 N = computeNormal(P[face[0]], P[face[1]], P[face[2]]);

		std::vector<int> newFace;

		for (int idx : face)
		{
			Vertex *v = new Vertex(P[idx], N);
			vertices.push_back(v);
			newFace.push_back(vertices.size() - 1);
		}

		faces.push_back(newFace);
	}

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

	std::vector<Vector3> bottomPositions;
	std::vector<Vector3> topPositions;

	for (int i = 0; i < n; ++i)
	{
		if (i >= idx1 && i <= idx2)
		{
			bottomPositions.push_back(Vector3(a * cos(alpha), -0.5f, a * sin(alpha)));
			topPositions.push_back(Vector3(a * cos(alpha), 0.5f, a * sin(alpha)));
		}
		else
		{
			bottomPositions.push_back(Vector3(cos(alpha), -0.5f, sin(alpha)));
			topPositions.push_back(Vector3(cos(alpha), 0.5f, sin(alpha)));
		}
		alpha += dt;
	}

	Vector3 bottomCenter(0.0f, -0.5f, 0.0f);
	Vector3 topCenter(0.0f, 0.5f, 0.0f);

	{
		int start = vertices.size();
		for (int i = 0; i < n; ++i)
		{
			vertices.push_back(new Vertex(bottomPositions[i], Vector3(0, -1, 0))); // normal down
		}
		vertices.push_back(new Vertex(bottomCenter, Vector3(0, -1, 0)));

		for (int i = 0; i < n; ++i)
		{
			std::vector<int> face = {start + i, start + (i + 1) % n, start + n}; // bottom center
			faces.push_back(face);
		}
	}

	{
		int start = vertices.size();
		for (int i = 0; i < n; ++i)
		{
			vertices.push_back(new Vertex(topPositions[i], Vector3(0, 1, 0))); // normal up
		}
		vertices.push_back(new Vertex(topCenter, Vector3(0, 1, 0)));

		for (int i = 0; i < n; ++i)
		{
			std::vector<int> face = {start + i, start + n, start + (i + 1) % n}; // top center
			faces.push_back(face);
		}
	}

	for (int i = 0; i < n; ++i)
	{
		int next = (i + 1) % n;
		Vector3 p0 = bottomPositions[i];
		Vector3 p1 = topPositions[i];
		Vector3 p2 = topPositions[next];

		Vector3 normal = Vector3::cross(p1 - p0, p2 - p0);
		normal.normalize();

		int start = vertices.size();
		vertices.push_back(new Vertex(bottomPositions[i], normal));
		vertices.push_back(new Vertex(bottomPositions[next], normal));
		vertices.push_back(new Vertex(topPositions[next], normal));
		vertices.push_back(new Vertex(topPositions[i], normal));

		faces.push_back({start, start + 1, start + 2, start + 3});
	}

	Mesh *mesh = new Mesh();
	mesh->build(vertices, faces);
	return mesh;
}

Mesh *MeshFactory::buildShape2(float a, int n, int idx1 /*=45*/, int idx2 /*=134*/, float size /*=0.2*/, int range /*=15*/)
{
	std::vector<Vertex *> vertices;
	std::vector<std::vector<int>> faces;

	float alpha = 0.0f;
	float delta = 2 * M_PI / n;
	int numberSlices = idx2 - idx1 + 1;
	vertices.resize((numberSlices + 1) * 8);

	for (int i = 0; i < n; ++i)
	{
		if (i >= idx1 && i <= (idx2 + 1))
		{
			// bottom
			vertices[i - idx1] = new Vertex(Vector3((1.0f - size) * std::cos(alpha), -0.5f, (1.0f - size) * std::sin(alpha)));
			vertices[i + numberSlices - idx1 + 1] = new Vertex(Vector3((1.0f) * std::cos(alpha), -0.5f, (1.0f) * std::sin(alpha)));
			vertices[i + 2 * numberSlices - idx1 + 2] = new Vertex(Vector3(a * std::cos(alpha), -0.5f, a * std::sin(alpha)));
			vertices[i + 3 * numberSlices - idx1 + 3] = new Vertex(Vector3((a + size) * std::cos(alpha), -0.5f, (a + size) * std::sin(alpha)));

			// up
			vertices[i + 4 * numberSlices - idx1 + 4] = new Vertex(Vector3((1.0f - size) * std::cos(alpha), 0.5f, (1.0f - size) * std::sin(alpha)));
			vertices[i + 5 * numberSlices - idx1 + 5] = new Vertex(Vector3((1.0f) * std::cos(alpha), 0.5f, (1.0f) * std::sin(alpha)));
			vertices[i + 6 * numberSlices - idx1 + 6] = new Vertex(Vector3(a * std::cos(alpha), 0.5f, a * std::sin(alpha)));
			vertices[i + 7 * numberSlices - idx1 + 7] = new Vertex(Vector3((a + size) * std::cos(alpha), 0.5f, (a + size) * std::sin(alpha)));
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
			std::vector<int> rf1 = {i + 1 * numberSlices + 1, i + 2 * numberSlices + 2, i + 2 * numberSlices + 2 + 1, i + 1 * numberSlices + 1 + 1};
			std::vector<int> rf2 = {i + 6 * numberSlices + 6, i + 5 * numberSlices + 5, i + 5 * numberSlices + 5 + 1, i + 6 * numberSlices + 6 + 1};

			faces.push_back(rf1);
			faces.push_back(rf2);
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

	{
		int i = idx1;
		std::vector<int> face0 = {i - idx1, i + 3 * numberSlices - idx1 + 3, i + 7 * numberSlices - idx1 + 7, i + 4 * numberSlices - idx1 + 4};
		i = idx2 + 1;
		std::vector<int> face1 = {i + 3 * numberSlices - idx1 + 3, i - idx1, i + 4 * numberSlices - idx1 + 4, i + 7 * numberSlices - idx1 + 7};
		i = range;
		std::vector<int> face2 = {i + 5 * numberSlices + 5, i + 6 * numberSlices + 6, i + 2 * numberSlices + 2, i + 1 * numberSlices + 1};
		i = numberSlices - range;
		std::vector<int> face3 = {i + 6 * numberSlices + 6, i + 5 * numberSlices + 5, i + 1 * numberSlices + 1, i + 2 * numberSlices + 2};

		faces.push_back(face0);
		faces.push_back(face1);
		faces.push_back(face2);
		faces.push_back(face3);
	}

	std::vector<Vertex *> flatVertices;
	std::vector<std::vector<int>> flatFaces;
	flatVertices.reserve(faces.size() * 4);
	flatFaces.reserve(faces.size());

	for (auto &face : faces)
	{
		if (face.size() < 3)
			continue;

		Vertex *v0 = vertices[face[0]];
		Vertex *v1 = vertices[face[1]];
		Vertex *v2 = vertices[face[2]];

		Vector3 e1 = v1->position - v0->position;
		Vector3 e2 = v2->position - v0->position;
		Vector3 N = e1.cross(e2);
		N.normalize();

		int base = (int)flatVertices.size();

		for (int idx : face)
		{
			Vertex *oldV = vertices[idx];
			Vertex *newV = new Vertex(oldV->position, N);
			flatVertices.push_back(newV);
		}

		int k = (int)face.size();
		std::vector<int> newFace;
		newFace.reserve(k);
		for (int j = 0; j < k; ++j)
			newFace.push_back(base + j);
		flatFaces.push_back(newFace);
	}

	for (Vertex *v : vertices)
	{
		if (v)
			delete v;
	}

	vertices = std::move(flatVertices);
	faces = std::move(flatFaces);

	Mesh *mesh = new Mesh();
	mesh->build(vertices, faces);
	return mesh;
}

Mesh *MeshFactory::buildShape3(float a)
{
	std::vector<Vertex *> vertices;

	vertices.push_back(new Vertex(Vector3(0.5f, -0.5f, -a / 2)));  // 0
	vertices.push_back(new Vertex(Vector3(0.5f, -0.5f, a / 2)));   // 1
	vertices.push_back(new Vertex(Vector3(a / 2, -0.5f, 0.5f)));   // 2
	vertices.push_back(new Vertex(Vector3(-a / 2, -0.5f, 0.5f)));  // 3
	vertices.push_back(new Vertex(Vector3(-0.5f, -0.5f, a / 2)));  // 4
	vertices.push_back(new Vertex(Vector3(-0.5f, -0.5f, -a / 2))); // 5
	vertices.push_back(new Vertex(Vector3(-a / 2, -0.5f, -0.5f))); // 6
	vertices.push_back(new Vertex(Vector3(a / 2, -0.5f, -0.5f)));  // 7

	vertices.push_back(new Vertex(Vector3(0.5f, 0.5f, -a / 2)));  // 8
	vertices.push_back(new Vertex(Vector3(0.5f, 0.5f, a / 2)));	  // 9
	vertices.push_back(new Vertex(Vector3(a / 2, 0.5f, 0.5f)));	  // 10
	vertices.push_back(new Vertex(Vector3(-a / 2, 0.5f, 0.5f)));  // 11
	vertices.push_back(new Vertex(Vector3(-0.5f, 0.5f, a / 2)));  // 12
	vertices.push_back(new Vertex(Vector3(-0.5f, 0.5f, -a / 2))); // 13
	vertices.push_back(new Vertex(Vector3(-a / 2, 0.5f, -0.5f))); // 14
	vertices.push_back(new Vertex(Vector3(a / 2, 0.5f, -0.5f)));  // 15

	std::vector<std::vector<int>> faces = {
		{0, 1, 2, 3, 4, 5, 6, 7},
		{15, 14, 13, 12, 11, 10, 9, 8},
		{8, 9, 1, 0},
		{9, 10, 2, 1},
		{10, 11, 3, 2},
		{11, 12, 4, 3},
		{12, 13, 5, 4},
		{13, 14, 6, 5},
		{14, 15, 7, 6},
		{15, 8, 0, 7}};

	std::vector<Vector3> normals(vertices.size(), Vector3(0, 0, 0));

	for (auto &f : faces)
	{
		Vector3 a = vertices[f[1]]->position - vertices[f[0]]->position;
		Vector3 b = vertices[f[2]]->position - vertices[f[0]]->position;
		Vector3 n = Vector3(a.cross(b).x, a.cross(b).y, a.cross(b).z);
		n.normalize();

		for (int idx : f)
		{
			normals[idx] = Vector3((normals[idx] + n).x, (normals[idx] + n).y, (normals[idx] + n).z);
		}
	}

	for (int i = 0; i < vertices.size(); i++)
	{
		normals[i].normalize();
		vertices[i]->normal = Vector3(normals[i].x, normals[i].y, normals[i].z);
	}

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

	Vector3 bottomCenterNormal(0.0f, -1.0f, 0.0f);
	Vector3 topCenterNormal(0.0f, 1.0f, 0.0f);

	for (int i = 0; i < n; ++i)
	{
		Vector3 sideNormal(std::cos(alpha), 0.0f, std::sin(alpha));

		Vector3 bottomVertexNormal = sideNormal + bottomCenterNormal;
		bottomVertexNormal.normalize();
		vertices[i] = new Vertex(Vector3(std::cos(alpha), -0.5f, std::sin(alpha)), bottomVertexNormal);

		Vector3 topVertexNormal = sideNormal + topCenterNormal;
		topVertexNormal.normalize();
		vertices[i + n] = new Vertex(Vector3(std::cos(alpha), 0.5f, std::sin(alpha)), topVertexNormal);

		alpha += delta;
	}

	vertices[2 * n] = new Vertex(Vector3(0.0f, -0.5f, 0.0f), bottomCenterNormal);
	vertices[2 * n + 1] = new Vertex(Vector3(0.0f, 0.5f, 0.0f), topCenterNormal);

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
	Material *material;
	Vector3 localPosition;
	Vector3 localRotation;
	Vector3 localScale;
	Vector4 color;

	MeshInstance(Mesh *m, Material *material = nullptr) : mesh(m), localPosition(), localRotation(), localScale(1.0f, 1.0f, 1.0f), material{material}, color{1.0f, 0.0f, 0.0f, 1.0f} {}

	void draw(bool isColour);
	void setMaterial(Material *material);
	void setDefaultMaterial();
	void setColor(float x, float y, float z, float a);
	~MeshInstance();
};

void MeshInstance::draw(bool isColour = false)
{
	glPushMatrix();

	// moving
	glTranslatef(localPosition.x, localPosition.y, localPosition.z);
	// rotating
	glRotatef(localRotation.z, 0, 0, 1);
	glRotatef(localRotation.y, 0, 1, 0);
	glRotatef(localRotation.x, 1, 0, 0);
	// scaling
	glScalef(localScale.x, localScale.y, localScale.z);

	if (isColour)
	{
		// ignored if lighting
		// else, apply that
		this->material->apply();
		glColor4f(this->material->diffuse.x, this->material->diffuse.y, this->material->diffuse.z, this->material->diffuse.w);
	}
	mesh->draw(isColour);

	glPopMatrix();
}

void MeshInstance::setMaterial(Material *material)
{
	delete this->material;
	this->material = material;
}

void MeshInstance::setDefaultMaterial()
{
	delete this->material;
	Material *m = new Material();
	this->setMaterial(m);
}

void MeshInstance::setColor(float x, float y, float z, float a)
{
	this->color.x = x;
	this->color.y = y;
	this->color.z = z;
	this->color.w = a;
}

MeshInstance::~MeshInstance()
{
	delete material;
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
	// w=1 -> positional, w=0 -> directional
	Vector4 position;
	Vector4 ambient;
	Vector4 diffuse;
	Vector4 specular;
	bool enabled;
	// GL_LIGHT0, GL_LIGHT1, ...
	GLenum glLightID;

	Light(GLenum lightID = GL_LIGHT0);

	void apply() const;
};

Light::Light(GLenum lightID)
{
	// Sunlight as directional light
	position.set(10.0f, 10.0f, 10.0f, 0.0f);
	ambient.set(0.2f, 0.2f, 0.2f, 1.0f);
	diffuse.set(1.0f, 1.0f, 1.0f, 1.0f);
	specular.set(1.0f, 1.0f, 1.0f, 1.0f);

	this->glLightID = lightID;
	this->enabled = true;
}

void Light::apply() const
{
	if (!enabled)
	{
		glDisable(glLightID);
		return;
	}

	glEnable(glLightID);
	glLightfv(glLightID, GL_POSITION, position.data());
	glLightfv(glLightID, GL_AMBIENT, ambient.data());
	glLightfv(glLightID, GL_DIFFUSE, diffuse.data());
	glLightfv(glLightID, GL_SPECULAR, specular.data());
}

// ###########################################################
// ###########################################################

// ###########################################################
// ###########################################################
class Scene
{
public:
	std::vector<Light *> lights;
	Camera *camera;
	std::vector<Object *> objects;
	std::map<std::string, Mesh *> meshes;
	float theta0;

	float point1;
	float point2;
	float point3;
	float point4;

	float vMove;

	bool isColour;

	std::vector<Vertex *> vertices;
	std::vector<std::vector<int>> quads;

	Scene();
	void add(Object *object);
	void renderBitmapString(float x, float y, float z, void *font, const char *string);
	void drawAxes();
	void drawAxisTicks(float min, float max, float size);
	void applyPerspectiveProjection();
	void applyOrthographicProjection();
	void draw();
	void draw2d();
	void drawSegment(bool v, float x, float y, float w, float h);
	void drawDigit(int num, float x, float y);
	void drawColon(float x, float y);
	void drawClock();
	void prepareBrick(float a, float b);
	void drawBrick();
	void drawFloor();
	void update();
	void init();
	void load();
	~Scene();
};

Scene::Scene() : isColour{false}, camera{new Camera()}
{
}

void Scene::add(Object *object)
{
	this->objects.push_back(object);
}

void Scene::renderBitmapString(float x, float y, float z, void *font, const char *string)
{
	glRasterPos3f(x, y, z);
	for (const char *c = string; *c != '\0'; c++)
		glutBitmapCharacter(font, *c);
}

void Scene::drawAxisTicks(float min = -10, float max = 10, float size = 0.05f)
{
	glColor3f(0.0f, 0.0f, 0.0f);
	for (int i = min; i <= max; i++)
	{
		glBegin(GL_LINES);
		glVertex3f(i, -size, 0);
		glVertex3f(i, size, 0);
		glEnd();

		// draw number
		if (i != 0)
		{
			char buf[8];
			sprintf(buf, "%d", i);
			renderBitmapString(i, 0.2f, 0, GLUT_BITMAP_HELVETICA_12, buf);
		}
	}

	for (int i = min; i <= max; i++)
	{
		glBegin(GL_LINES);
		glVertex3f(-size, i, 0);
		glVertex3f(size, i, 0);
		glEnd();

		// draw number
		if (i != 0)
		{
			char buf[8];
			sprintf(buf, "%d", i);
			renderBitmapString(0.2f, i, 0, GLUT_BITMAP_HELVETICA_12, buf);
		}
	}

	for (int i = min; i <= max; i++)
	{
		glBegin(GL_LINES);
		glVertex3f(0, -size, i);
		glVertex3f(0, size, i);
		glEnd();

		// draw number
		if (i != 0)
		{
			char buf[8];
			sprintf(buf, "%d", i);
			renderBitmapString(0.1f, 0.1f, i, GLUT_BITMAP_HELVETICA_12, buf);
		}
	}
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

	drawAxisTicks();

	glPopMatrix();
}

void Scene::applyPerspectiveProjection()
{
	gluPerspective(60.0f, float(WIDTH) / float(HEIGHT), 0.1f, 100.0f);
}

void Scene::applyOrthographicProjection()
{
	glOrtho(-5.0f, 5.0f, -5.0f, 5.0f, -5.f, 100.0f);
}

void Scene::draw()
{
	// std::cout << "3D " << this->isColour << "\n";
	// std::cout << "3D\n";
	// set the background

	glEnable(GL_DEPTH_TEST);

	// set the projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	applyPerspectiveProjection();

	// set the camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	this->camera->apply();

	// lighting for colour
	if (this->isColour)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_NORMALIZE);
		for (Light *light : lights)
		{
			light->apply();
		}
	}
	else
	{
		glDisable(GL_LIGHTING);
	}

	// start drawing/transform from data in memory
	if (this->isColour)
	{
		// drawBrick(0.0f, 0.15f);
		drawFloor();
	}

	// drawAxes();

	for (Object *object : objects)
	{
		object->draw(this->isColour);
	}
}

void Scene::draw2d()
{
	// std::cout << "2D " << this->isColour << "\n";
	// std::cout << "2D" << "\n";
	// parameters changed in the 3D mode doesn't affect it at all
	// set the projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-5.0f, 5.0f, -5.0f, 5.0f, 0.0f, 10.0f);

	// set the camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f, 2.5f, 7.0f, 0.0f, 2.5f, 0.0f, 0.0f, 1.0f, 0.0f);

	// disable lighting completely
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_COLOR_MATERIAL);

	// drawAxes();

	for (Object *o : this->objects)
	{
		// normal is ignored
		// colored is used
		// light is disable
		o->draw(this->isColour);
	}
}

void Scene::drawSegment(bool v, float x, float y, float w, float h)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

	float slant = (v ? w : h) * 0.3f;

	glBegin(GL_POLYGON);
	if (v)
	{
		glVertex2f(x + slant, y);		  // bottom-left (slanted)
		glVertex2f(x + w - slant, y);	  // bottom-right
		glVertex2f(x + w, y + h / 2);	  // right-middle
		glVertex2f(x + w - slant, y + h); // top-right
		glVertex2f(x + slant, y + h);	  // top-left
		glVertex2f(x, y + h / 2);		  // left-middle
	}
	else
	{
		glVertex2f(x, y + slant);		  // left-bottom
		glVertex2f(x + w / 2, y);		  // bottom-middle
		glVertex2f(x + w, y + slant);	  // right-bottom
		glVertex2f(x + w, y + h - slant); // right-top
		glVertex2f(x + w / 2, y + h);	  // top-middle
		glVertex2f(x, y + h - slant);	  // left-top
	}
	glEnd();
}

void Scene::drawDigit(int num, float x, float y)
{
	float W = 40;
	float H = 80;
	float T = 8;

	bool *S = segments[num];

	glPushMatrix();
	glTranslatef(x, y, 0);

	if (S[0])
		drawSegment(false, T, H - T, W - 2 * T, T); // A
	if (S[1])
		drawSegment(true, W - T, H / 2, T, H / 2 - T); // B
	if (S[2])
		drawSegment(true, W - T, 0, T, H / 2 - T); // C
	if (S[3])
		drawSegment(false, T, 0, W - 2 * T, T); // D
	if (S[4])
		drawSegment(true, 0, 0, T, H / 2 - T); // E
	if (S[5])
		drawSegment(true, 0, H / 2, T, H / 2 - T); // F
	if (S[6])
		drawSegment(false, T, H / 2 - T / 2, W - 2 * T, T); // G

	glPopMatrix();
}

void Scene::drawColon(float x, float y)
{
	glBegin(GL_QUADS);
	glVertex2f(x, y + 40);
	glVertex2f(x + 8, y + 40);
	glVertex2f(x + 8, y + 48);
	glVertex2f(x, y + 48);
	glVertex2f(x, y + 10);
	glVertex2f(x + 8, y + 10);
	glVertex2f(x + 8, y + 18);
	glVertex2f(x, y + 18);
	glEnd();
}

void Scene::drawClock()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_COLOR_MATERIAL);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-250, 350, -60, 60);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// get the current time
	time_t now = time(0);
	tm *t = localtime(&now);

	int h = t->tm_hour;
	int m = t->tm_min;
	int s = t->tm_sec;

	float y = -40;

	drawDigit(h / 10, -180, y);
	drawDigit(h % 10, -120, y);

	drawColon(-45, y);

	drawDigit(m / 10, 0, y);
	drawDigit(m % 10, 60, y);

	drawColon(135, y);

	drawDigit(s / 10, 180, y);
	drawDigit(s % 10, 240, y);
}

void Scene::prepareBrick(float a, float b)
{
	float start = M_PI_4 + M_PI_4 / 2;
	float theta = M_PI_4 / 2;
	std::vector<float> angles;
	Vector3 normal = {0.0f, 0.0f, 1.0f};

	for (int i = 0; i < 8; ++i)
	{
		angles.push_back(start);
		start -= M_PI_4;
	}

	int i = 0;

	for (float angle : angles)
	{
		float x = a * std::cos(angle);
		float y = a * std::sin(angle);

		Vertex *v0 = new Vertex(Vector3(x + 0.0f, y + 0.0f, 0.0f), normal);
		Vertex *v1 = new Vertex(Vector3(x + b * std::cos(angle - theta), y + b * std::sin(angle - theta), 0.0f), normal);
		Vertex *v2 = new Vertex(Vector3(x + 2 * b * std::cos(angle), y + 2 * b * std::sin(angle), 0.0f), normal);
		Vertex *v3 = new Vertex(Vector3(x + b * std::cos(angle + theta), y + b * std::sin(angle + theta), 0.0f), normal);

		this->vertices.push_back(v0);
		this->vertices.push_back(v1);
		this->vertices.push_back(v2);
		this->vertices.push_back(v3);

		std::vector<int> quad = {i, i + 1, i + 2, i + 3};
		this->quads.push_back(quad);

		i += 4;
	}
}

void Scene::drawBrick()
{
	// BG
	Vector4 color = {1.0f, 1.0f, 1.0f, 1.0f};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, color.data());
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.5f, -0.5f, -0.01f);

	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.5f, 0.5f, -0.01f);

	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.5f, 0.5f, -0.01f);

	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.5f, -0.5f, -0.01f);
	glEnd();

	// 8 diamonds
	Vector4 color2 = {90.0f / 255.0f, 156.0f / 255.0f, 250.0f / 255.0f, 1.0f};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, color2.data());
	glNormal3f(0.0f, 0.0f, 1.0f);

	for (std::vector<int> quad : this->quads)
	{
		glBegin(GL_QUADS);
		for (int idx : quad)
		{
			glVertex3f(this->vertices[idx]->position.x, this->vertices[idx]->position.y, this->vertices[idx]->position.z);
		}

		glEnd();
	}

	// 4 三角
	glNormal3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	glVertex3f(-0.5f, 0.5f, 0.0f);
	glVertex3f(-0.5f, 0.2f, 0.0f);
	glVertex3f(-0.2f, 0.5f, 0.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3f(0.5f, 0.5f, 0.0f);
	glVertex3f(0.2f, 0.5f, 0.0f);
	glVertex3f(0.5f, 0.2f, 0.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3f(0.5f, -0.5f, 0.0f);
	glVertex3f(0.5f, -0.2f, 0.0f);
	glVertex3f(0.2f, -0.5f, 0.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3f(-0.5f, -0.5f, 0.0f);
	glVertex3f(-0.2f, -0.5f, 0.0f);
	glVertex3f(-0.5f, -0.2f, 0.0f);
	glEnd();
}

void Scene::drawFloor()
{
	for (float i = -4.5f; i <= 4.5f; i += 1.0f)
	{
		for (float j = -4.5f; j <= 4.5f; j += 1.0f)
		{
			glPushMatrix();
			glTranslatef(i, 0.0f, j);
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

			this->drawBrick();
			glPopMatrix();
		}
	}
}

void Scene::init()
{
	// load and build memory for meshes
	this->load();

	// set up the system
	Object *obj = new Object();

	// 1. slider
	// mesh
	Mesh *m1 = this->meshes["cube"];
	// instance
	MeshInstance *mi1 = new MeshInstance(m1);
	mi1->localScale.set(0.3f, 3.0f, 0.3f);
	mi1->localPosition.set(0.0f, 1.5f + 1.5f, 0.0f);
	// material
	Material *material1 = new Material();
	material1->ambient = Vector4(0.0f, 0.2f, 0.0f, 1.0f);
	material1->diffuse = Vector4(0.1f, 0.6f, 0.1f, 1.0f);
	material1->specular = Vector4(0.6f, 0.6f, 0.6f, 1.0f);
	material1->emission = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	material1->shininess = 60.0f;
	mi1->setMaterial(material1);

	// 弓のように
	// mesh
	Mesh *m2 = this->meshes["2"];
	// instance
	MeshInstance *mi2 = new MeshInstance(m2);
	mi2->localScale.set(1.0f, 0.3f, 1.0f);
	mi2->localRotation.set(-90.0f, 0.0f, 0.0f);
	// material
	Material *material7 = new Material();
	material7->ambient = Vector4(0.3f, 0.0f, 0.1f, 1.0f);
	material7->diffuse = Vector4(0.1f, 0.6f, 0.1f, 1.0f);
	material7->specular = Vector4(0.6f, 0.6f, 0.6f, 1.0f);
	material7->emission = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	material7->shininess = 60.0f;
	mi2->setMaterial(material7);

	// ２つの如意きんこぼう
	// mesh
	Mesh *m3 = this->meshes["cylinder"];
	// instance
	MeshInstance *mi3 = new MeshInstance(m3);
	mi3->localScale.set(0.15f, 1.0f, 0.15f);
	mi3->localRotation.set(90.0f, 0.0f, 0.0f);
	mi3->localPosition.set((1.0f + 0.15f) * std::cos(M_PI / 3), (1.0f + 0.15f) * sin(M_PI / 3), 0.0f);
	MeshInstance *mi4 = new MeshInstance(m3);
	mi4->localScale.set(0.15f, 1.0f, 0.15f);
	mi4->localRotation.set(90.0f, 0.0f, 0.0f);
	mi4->localPosition.set((1.0f + 0.15f) * std::cos(2 * M_PI / 3), (1.0f + 0.15f) * sin(2 * M_PI / 3), 0.0f);
	// material
	Material *material2 = new Material();
	material2->ambient = Vector4(0.2f, 0.2f, 0.0f, 1.0f);
	material2->diffuse = Vector4(0.8f, 0.8f, 0.1f, 1.0f);
	material2->specular = Vector4(0.6f, 0.6f, 0.6f, 1.0f);
	material2->emission = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	material2->shininess = 50.0f;
	mi3->setMaterial(material2);
	mi4->setMaterial(material2);

	// 2. パンみたい
	// mesh
	Mesh *m4 = this->meshes["4"];
	// instance
	MeshInstance *mi5 = new MeshInstance(m4);
	mi5->localScale.set(3.0f / 8.0f, 1.0f, 3.0f / 8.0f);
	mi5->localPosition.set(0.0f, 3.0f, 0.0f);
	// material
	Material *material10 = new Material();
	material10->ambient = Vector4(0.1f, 0.1f, 0.3f, 1.0f);
	material10->diffuse = Vector4(0.0f, 0.1f, 0.9f, 1.0f);
	material10->specular = Vector4(0.6f, 0.6f, 0.6f, 1.0f);
	material10->emission = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	material10->shininess = 70.0f;
	mi5->setMaterial(material10);

	// 3. パンの隣
	// instance
	MeshInstance *mi6 = new MeshInstance(m1);
	mi6->localScale.set(3.0f / 8.0f, 1.0f, 0.5f);
	mi6->localPosition.set(0.0f, 3.0f, 3.0f / 16.0f + 0.25f);
	// material
	Material *material6 = new Material();
	material6->ambient = Vector4(0.1f, 0.1f, 0.3f, 1.0f);
	material6->diffuse = Vector4(0.0f, 0.1f, 0.9f, 1.0f);
	material6->specular = Vector4(0.6f, 0.6f, 0.6f, 1.0f);
	material6->emission = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	material6->shininess = 70.0f;
	mi6->setMaterial(material6);

	// 孫の手みたい
	// mesh
	Mesh *m5 = this->meshes["5"];
	// instance
	MeshInstance *mi7 = new MeshInstance(m5);
	mi7->localScale.set(3.0f / 8.0f, 3.3f, 0.3f);
	mi7->localPosition.set(0.0f, 3.5f - (3.3f / 2), 3.0f / 16.0f + 0.5f + 0.15f);
	// material
	Material *material9 = new Material();
	material9->ambient = Vector4(0.1f, 0.1f, 0.3f, 1.0f);
	material9->diffuse = Vector4(0.0f, 0.1f, 0.9f, 1.0f);
	material9->specular = Vector4(0.6f, 0.6f, 0.6f, 1.0f);
	material9->emission = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	material9->shininess = 70.0f;
	mi7->setMaterial(material9);

	// ピザのようなもの
	// mesh
	Mesh *m6 = this->meshes["1"];
	// instance
	MeshInstance *mi8 = new MeshInstance(m6);
	mi8->localScale.set(1.0f, 0.2f, 1.0f);
	mi8->localRotation.set(90.0f, 0.0f, 0.0f);
	mi8->localPosition.set(0.0f, 0.0f, 3.0f / 16.0f + 0.5f + 0.15f - 0.4f / 2 - 0.15f / 2.0f - 0.1f - 0.15f / 2);
	// material
	Material *material3 = new Material();
	material3->ambient = Vector4(0.3f, 0.0f, 0.1f, 1.0f);
	material3->diffuse = Vector4(0.9f, 0.0f, 0.0f, 1.0f);
	material3->specular = Vector4(0.6f, 0.6f, 0.6f, 1.0f);
	material3->emission = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	material3->shininess = 10.0f;
	mi8->setMaterial(material3);

	// また２つの如意きんこぼう
	// instance
	MeshInstance *mi9 = new MeshInstance(m3);
	mi9->localScale.set(0.3f, 0.15f, 0.3f);
	mi9->localRotation.set(90.0f, 0.0f, 0.0f);
	mi9->localPosition.set(0.0f, 0.0f, 3.0f / 16.0f + 0.5f + 0.15f - 0.4f / 2 - 0.15f / 2);
	// material
	Material *material5 = new Material();
	material5->ambient = Vector4(0.3f, 0.0f, 0.1f, 1.0f);
	material5->diffuse = Vector4(0.9f, 0.0f, 0.0f, 1.0f);
	material5->specular = Vector4(0.6f, 0.6f, 0.6f, 1.0f);
	material5->emission = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	material5->shininess = 60.0f;
	mi9->setMaterial(material5);
	// instance
	MeshInstance *mi10 = new MeshInstance(m3);
	mi10->localScale.set(0.15f, 0.5f, 0.15f);
	mi10->localRotation.set(90.0f, 0.0f, 0.0f);
	mi10->localPosition.set(0.0f, 0.0f, 3.0f / 16.0f + 0.5f + 0.15f + 0.5f / 2 + 0.4f / 2);
	// material
	Material *material4 = new Material();
	material4->ambient = Vector4(0.3f, 0.0f, 0.1f, 1.0f);
	material4->diffuse = Vector4(0.9f, 0.0f, 0.0f, 1.0f);
	material4->specular = Vector4(0.6f, 0.6f, 0.6f, 1.0f);
	material4->emission = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	material4->shininess = 60.0f;
	mi10->setMaterial(material4);

	// 武器みたい
	// mesh
	Mesh *m7 = this->meshes["3"];
	// instance
	MeshInstance *mi11 = new MeshInstance(m7);
	mi11->localScale.set(0.4f, 0.4f, 0.4f);
	mi11->localRotation.set(90.0f, 0.0f, 0.0f);
	mi11->localPosition.set(0.0f, 0.0f, 3.0f / 16.0f + 0.5f + 0.15f);
	// material
	Material *material8 = new Material();
	material8->ambient = Vector4(0.4f, 0.2f, 0.0f, 1.0f);
	material8->diffuse = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	material8->specular = Vector4(0.7f, 0.7f, 0.6f, 1.0f);
	material8->emission = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	material8->shininess = 50.0f;
	mi11->setMaterial(material8);

	// add instance to object
	obj->add(mi1);	// 0 👆
	obj->add(mi2);	// 1 👆
	obj->add(mi3);	// 2 👆
	obj->add(mi4);	// 3 👆
	obj->add(mi5);	// 4
	obj->add(mi6);	// 5
	obj->add(mi7);	// 6
	obj->add(mi8);	// 7
	obj->add(mi9);	// 8
	obj->add(mi10); // 9
	obj->add(mi11); // 10

	// change the obj
	obj->position.set(0.0f, 2.0f, 0.0f);

	// Add the object to the scene
	this->objects.push_back(obj);

	// set up the brick
	this->prepareBrick(0.03, 0.18f);

	// set up inial theta
	this->theta0 = 0.0f;

	float alpha = M_PI / 3.0f - std::asin(0.15f / 1.15f);
	float beta = M_PI_2 - std::asin(1.15f * std::cos(M_PI / 3.0f) / 1.65f) - std::asin(0.15f / 1.65f);
	this->point1 = M_PI_4 + alpha;
	this->point2 = M_PI_4 + beta;
	this->point3 = 5.0f * M_PI / 4.0f + M_PI_2 - beta;
	this->point4 = 5.0f * M_PI / 4.0f + M_PI_2 - alpha;

	float x = 1.15f * std::cos(M_PI / 3);
	float deltaS = std::sqrt(std::pow(1.65f, 2) - std::pow(x, 2)) - std::sqrt(std::pow(1.15f, 2) - std::pow(x, 2));
	float deltaA = beta - alpha;
	this->vMove = this->camera->omega / deltaA * deltaS;

	// set up initial camera
	float angle = M_PI / 2;
	float height = 2.5f;
	float radius = 7.0f;

	this->camera->angle = angle;
	this->camera->height = height;
	this->camera->radius = radius;

	this->camera->position.set(radius * std::cos(angle), height, radius * std::sin(angle));
	this->camera->target.set(0.0f, 2.5f, 0.0f);
	this->camera->up.set(0.0f, 1.0f, 0.0f);

	// ☀️☀️☀️ [0] - Sunlight
	Light *sun = new Light(GL_LIGHT0);

	// 💡💡💡 [5] - Above bulb
	Light *bulbAbove = new Light(GL_LIGHT5);
	bulbAbove->position.set(0.0f, 5.0f, 0.0f, 1.0f);
	bulbAbove->ambient.set(0.05f, 0.05f, 0.05f, 1.0f);
	bulbAbove->diffuse.set(1.0f, 0.9f, 0.8f, 1.0f);
	bulbAbove->specular.set(1.0f, 1.0f, 1.0f, 1.0f);

	// 💡💡💡 [1] - Right-side bulb
	Light *bulbRight = new Light(GL_LIGHT1);
	bulbRight->position.set(5.0f, 0.0f, 0.0f, 1.0f);
	bulbRight->ambient.set(0.05f, 0.05f, 0.05f, 1.0f);
	bulbRight->diffuse.set(1.0f, 0.9f, 0.8f, 1.0f);
	bulbRight->specular.set(1.0f, 1.0f, 1.0f, 1.0f);

	// 💡💡💡 [2] - Left-side bulb
	Light *bulbLeft = new Light(GL_LIGHT2);
	bulbLeft->position.set(-5.0f, .0f, 0.0f, 1.0f);
	bulbLeft->ambient.set(0.05f, 0.05f, 0.05f, 1.0f);
	bulbLeft->diffuse.set(1.0f, 0.9f, 0.8f, 1.0f);
	bulbLeft->specular.set(1.0f, 1.0f, 1.0f, 1.0f);

	// 💡💡💡 [3] - behind bulb
	Light *bulbBehind = new Light(GL_LIGHT3);
	bulbBehind->position.set(0.0f, 0.0f, -10.0f, 1.0f);
	bulbBehind->ambient.set(0.05f, 0.05f, 0.05f, 1.0f);
	bulbBehind->diffuse.set(1.0f, 0.9f, 0.8f, 1.0f);
	bulbBehind->specular.set(1.0f, 1.0f, 1.0f, 1.0f);

	// 💡💡💡 [4] - front bulb
	Light *bulbFront = new Light(GL_LIGHT4);
	bulbFront->position.set(0.0f, 0.0f, 5.0f, 1.0f);
	bulbFront->ambient.set(0.05f, 0.05f, 0.05f, 1.0f);
	bulbFront->diffuse.set(1.0f, 0.9f, 0.8f, 1.0f);
	bulbFront->specular.set(1.0f, 1.0f, 1.0f, 1.0f);

	// 💡💡💡 [6] - floor bulb
	Light *bulbFloor = new Light(GL_LIGHT6);
	bulbFloor->position.set(0.0f, 0.0f, 0.0f, 1.0f);
	bulbFloor->ambient.set(0.01f, 0.01f, 0.01f, 1.0f);
	bulbFloor->diffuse.set(1.0f, 1.0f, 1.0f, 1.0f);
	bulbFloor->specular.set(1.0f, 1.0f, 1.0f, 1.0f);

	sun->enabled = false;

	bulbRight->enabled = true;
	bulbLeft->enabled = true;
	bulbBehind->enabled = true;
	bulbFront->enabled = true;
	bulbAbove->enabled = true;
	bulbFloor->enabled = true;

	this->lights.push_back(sun);
	this->lights.push_back(bulbRight);
	this->lights.push_back(bulbLeft);
	this->lights.push_back(bulbBehind);
	this->lights.push_back(bulbFront);
	this->lights.push_back(bulbAbove);
	this->lights.push_back(bulbFloor);
}

void Scene::load()
{
	// should generalize to name
	// update based on scene
	this->meshes["cube"] = MeshFactory::cube();
	this->meshes["cylinder"] = MeshFactory::cylinder(72);
	this->meshes["1"] = MeshFactory::buildShape1(1.5f, 72, 9 + 1 - 1, 26 + 1);
	this->meshes["2"] = MeshFactory::buildShape2(1.3f, 72, 9, 26, 0.2f, 1);
	this->meshes["3"] = MeshFactory::buildShape3(0.6);
	this->meshes["4"] = MeshFactory::buildShape4(0.8f, 0.8f);
	this->meshes["5"] = MeshFactory::buildShape5(0.7f, 5.0f / 7.0f);
}

Scene::~Scene()
{
	for (Object *object : this->objects)
	{
		delete object;
	}

	delete camera;

	for (Light *light : this->lights)
	{
		delete light;
	}

	for (auto p : this->meshes)
	{
		delete p.second;
	}

	for (auto v : this->vertices)
	{
		delete v;
	}
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
		ACTION_COLOUR,
		ACTION_MOVE_WHEEL,
		ACTION_MOVE_WHEEL_COUNTER,
		ACTION_2D
	};
	Scene *scene;
	double lastTime;
	bool *actions;

	Game();
	void init();
	void update();
	void render();
	void stupidMove(bool up, float delta);
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

void Game::stupidMove(bool up, float delta)
{

	MeshInstance *part0 = this->scene->objects[0]->parts[0];
	MeshInstance *part1 = this->scene->objects[0]->parts[1];
	MeshInstance *part2 = this->scene->objects[0]->parts[2];
	MeshInstance *part3 = this->scene->objects[0]->parts[3];

	if (up)
	{
		part0->localPosition.y += delta;
		part1->localPosition.y += delta;
		part2->localPosition.y += delta;
		part3->localPosition.y += delta;
	}
	else
	{
		part0->localPosition.y -= delta;
		part1->localPosition.y -= delta;
		part2->localPosition.y -= delta;
		part3->localPosition.y -= delta;
	}
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

	if (this->actions[ACTION_2D])
	{
	}

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

	// update the wheel
	float velocity = this->scene->vMove - 0.04f - 0.01f - 0.01f + 0.01f + 0.01f;

	if (this->scene->theta0 > 2 * M_PI)
	{
		this->scene->theta0 = 0.0f;
	}

	if (this->scene->theta0 < 0.0f)
	{
		this->scene->theta0 = 2 * M_PI;
	}

	if (this->actions[ACTION_MOVE_WHEEL])
	{
		float delta = this->scene->camera->omega * dt * 180 / M_PI;
		MeshInstance *wheel = this->scene->objects[0]->parts[7];

		// change the local rotation along z axis
		wheel->localRotation.z -= delta;
		this->scene->theta0 += this->scene->camera->omega * dt;

		if (this->scene->theta0 >= this->scene->point1 && this->scene->theta0 < this->scene->point2)
		{
			// system move up
			stupidMove(true, velocity * dt);
		}

		if (this->scene->theta0 > this->scene->point3 && this->scene->theta0 < this->scene->point4)
		{
			// system move down
			stupidMove(false, velocity * dt);
		}
	}

	if (this->actions[ACTION_MOVE_WHEEL_COUNTER])
	{
		float delta = this->scene->camera->omega * dt * 180 / M_PI;
		MeshInstance *wheel = this->scene->objects[0]->parts[7];

		// change the local rotation along z axis
		wheel->localRotation.z += delta;
		this->scene->theta0 -= this->scene->camera->omega * dt;

		if (this->scene->theta0 > this->scene->point1 && this->scene->theta0 < this->scene->point2)
		{
			// system stupid move down
			stupidMove(false, velocity * dt);
		}

		if (this->scene->theta0 > this->scene->point3 && this->scene->theta0 < this->scene->point4)
		{
			// system move up
			stupidMove(true, velocity * dt);
		}
	}

	// update lastTime
	this->lastTime = now;
}

void Game::render()
{
	float clockWidth = WIDTH * 0.25f;
	float clockHeight = HEIGHT * 0.07f;

	float mainWidth = WIDTH - clockWidth;
	float mainHeight = HEIGHT - clockHeight;
	mainWidth = std::max(mainWidth, mainHeight);
	mainHeight = mainWidth;

	glDisable(GL_SCISSOR_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(clockHeight, 0, mainWidth, mainHeight);
	if (actions[ACTION_2D])
		this->scene->draw2d();
	else
		this->scene->draw();

	glViewport(0, HEIGHT - clockHeight, clockWidth, clockHeight);
	glEnable(GL_SCISSOR_TEST);
	glScissor(0, HEIGHT - clockHeight, clockWidth, clockHeight);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->scene->drawClock();

	glDisable(GL_SCISSOR_TEST);

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
	case '-':
		gGame.actions[Game::ACTION_MOVE_CAM_FORWARD] = true;
		break;
	case '+':
		gGame.actions[Game::ACTION_MOVE_CAM_BACKWARD] = true;
		break;
	case 'w':
	case 'W':
		gGame.actions[Game::ACTION_COLOUR] = !gGame.actions[Game::ACTION_COLOUR];
		break;
	case '1':
		gGame.actions[Game::ACTION_MOVE_WHEEL_COUNTER] = true;
		break;
	case '2':
		gGame.actions[Game::ACTION_MOVE_WHEEL] = true;
		break;
	case 'v':
	case 'V':
		gGame.actions[Game::ACTION_2D] = !gGame.actions[Game::ACTION_2D];
		break;
	}
}

void onKeyUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '-':
		gGame.actions[Game::ACTION_MOVE_CAM_FORWARD] = false;
		break;
	case '+':
		gGame.actions[Game::ACTION_MOVE_CAM_BACKWARD] = false;
		break;
	case '1':
		gGame.actions[Game::ACTION_MOVE_WHEEL_COUNTER] = false;
		break;
	case '2':
		gGame.actions[Game::ACTION_MOVE_WHEEL] = false;
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
	glutInitWindowPosition(WINDOW_X, WINDOW_Y);
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