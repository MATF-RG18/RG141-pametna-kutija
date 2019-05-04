#ifndef LIGHT_H
#define LIGHT_H
#include <GL/glut.h>

/* Koeficijenti svetla */
GLfloat light_position[] = {0.6, 1.4, 0.9, 0 };
GLfloat light_ambient[] = {0.2, 0.2, 0.2, 1 };
GLfloat light_diffuse[] = {0.7, 0.7, 0.7, 1 };
GLfloat light_specular[] = {0.9, 0.9, 0.9, 1 };

/* Koeficijenti materijala */
GLfloat ambient_coeffs[] = {0, 0, 0, 1};
GLfloat diffuse_coeffs[] = {0, 0, 0, 1};
GLfloat specular_coeffs[] = {0, 0, 0, 1};
GLfloat shininess = 20;

#endif