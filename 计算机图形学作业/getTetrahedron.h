#include "setLookAt.h"

GLuint getTetrahedron(point3D pA, point3D pB, point3D pC, point3D pD) {
    GLuint tetrahedronID = glGenLists(1);
    glNewList(tetrahedronID, GL_COMPILE);
    glBegin(GL_TRIANGLES);

    glColor4f(0.0f, 0.6f, 1.0f, 0.5f);
    glVertex3f(pA.x, pA.y, pA.z);
    glVertex3f(pB.x, pB.y, pB.z);
    glVertex3f(pD.x, pD.y, pD.z);

    glColor4f(0.6f, 0.0f, 1.0f, 0.5f);
    glVertex3f(pA.x, pA.y, pA.z);
    glVertex3f(pB.x, pB.y, pB.z);
    glVertex3f(pC.x, pC.y, pC.z);

    glColor4f(1.0f, 0.6f, 0.0f, 0.5f);
    glVertex3f(2.0f, 0.0f, 0.0);
    glVertex3f(pC.x, pC.y, pC.z);
    glVertex3f(pD.x, pD.y, pD.z);

    glColor4f(0.6f, 1.0f, 0.6f, 0.5f);
    glVertex3f(pB.x, pB.y, pB.z);
    glVertex3f(pC.x, pC.y, pC.z);
    glVertex3f(pD.x, pD.y, pD.z);
    glEnd();
    glEndList();
    return tetrahedronID;
}