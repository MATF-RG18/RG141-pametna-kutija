/* Kod nalik na onom sa casa */

#include"lights.h"

void init_lights(){
    /* Koeficijenti svetla */
    GLfloat light_position[] = {0.6, 1.4, 0.9, 0 };
    GLfloat light_ambient[] = {0.2, 0.2, 0.2, 1 };
    GLfloat light_diffuse[] = {0.7, 0.7, 0.7, 1 };
    GLfloat light_specular[] = {0.9, 0.9, 0.9, 1 };

    /* Ukljucuje se osvjetljenje i podesavaju parametri svetla. */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}