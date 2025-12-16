#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>

class Point3
{
public:
    float x, y, z;

    void set(float dx, float dy, float dz)
    {
        x = dx;
        y = dy;
        z = dz;
    }

    void set(Point3 &p)
    {
        x = p.x;
        y = p.y;
        z = p.z;
    }

    Point3()
    {
        x = y = z = 0;
    }

    Point3(float dx, float dy, float dz)
    {
        x = dx;
        y = dy;
        z = dz;
    }
};

class VertexID
{
public:
    int vertIndex;
    int colorIndex;
};

class Face
{
public:
    int nVerts;
    VertexID *vert;

    Face()
    {
        nVerts = 0;
        vert = NULL;
    }

    ~Face()
    {
        if (vert != NULL)
        {
            delete[] vert;
            vert = NULL;
        }
        nVerts = 0;
    }
};

class Mesh
{
public:
    int numVerts;
    Point3 *pt;
    int numFaces;
    Face *face;

    Mesh()
    {
        numVerts = 0;
        pt = NULL;
        numFaces = 0;
        face = NULL;
    }

    ~Mesh()
    {
        if (pt != NULL)
        {
            delete[] pt;
        }
        if (face != NULL)
        {
            delete[] face;
        }
        numVerts = 0;
        numFaces = 0;
    }

    void DrawWireframe()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        for (int f = 0; f < numFaces; f++)
        {
            glBegin(GL_POLYGON);
            for (int v = 0; v < face[f].nVerts; v++)
            {
                int iv = face[f].vert[v].vertIndex;

                glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
            }
            glEnd();
        }
    }

    void DrawColor()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        for (int f = 0; f < numFaces; f++)
        {
            glBegin(GL_POLYGON);
            for (int v = 0; v < face[f].nVerts; v++)
            {
                int iv = face[f].vert[v].vertIndex;
                int ic = face[f].vert[v].colorIndex;

                glColor3f(1.0f, 0.5f, 0.2f);
                glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
            }
            glEnd();
        }
    }
};

void testDrawCone()
{
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-5.0f, 5.0f, -5.0f, 5.0f, 1.0f, 10.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(5.0f, 5.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    int numSlices = 20;
    float height = 3.0f;
    float radius = 1.0f;

    // circle 0 -> numSlices - 1
    // lower point -> numSlices
    // higher point -> numSlices + 1
    Mesh *m = new Mesh();
    m->numVerts = numSlices + 1 + 1;
    m->pt = new Point3[m->numVerts];

    float alpha = 0.0f;
    float delta = 2 * M_PI / numSlices;

    // prepare point
    for (int i = 0; i < numSlices; ++i)
    {
        // m->pt[i] = Point3(radius * std::cos(alpha), 0.0f, std::sin(alpha));
        m->pt[i].x = radius * std::cos(alpha);
        m->pt[i].y = 0.0f;
        m->pt[i].z = radius * std::sin(alpha);

        alpha += delta;
    }

    m->pt[numSlices] = Point3(0.0f, 0.0f, 0.0f);
    m->pt[numSlices + 1] = Point3(0.0f, height, 0.0f);

    // prepare faces
    m->numFaces = numSlices + numSlices;
    m->face = new Face[m->numFaces];

    for (int i = 0; i < numSlices; ++i)
    {
        int numVerts = 3;

        // num vertices
        m->face[i].nVerts = numVerts;
        // init vertices ID
        m->face[i].vert = new VertexID[numVerts];

        // i -> numSlices -> i + 1
        m->face[i].vert[0].vertIndex = i;
        m->face[i].vert[1].vertIndex = numSlices;
        m->face[i].vert[2].vertIndex = (i + 1) % numSlices;

        m->face[i + numSlices].nVerts = numVerts;
        m->face[i + numSlices].vert = new VertexID[numVerts];

        // numSlices + 1, i, i + 1
        m->face[i + numSlices].vert[0].vertIndex = numSlices + 1;
        m->face[i + numSlices].vert[1].vertIndex = i;
        m->face[i + numSlices].vert[2].vertIndex = (i + 1) % numSlices;
    }

    m->DrawWireframe();
    glutSwapBuffers();
    delete m;
}