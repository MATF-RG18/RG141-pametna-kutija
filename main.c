#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>
#include<math.h>

/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);

/*static int active_function;     identifikator aktuelne funkcije */

static float anim_param1 = 0;
static float anim_param2 = 0;

int main(int argc, char **argv){
    /* Inicijalizuje se GLUT. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Kreira se prozor. */
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(300, 50);
    glutCreateWindow(argv[0]);

    /* Registruju se callback funkcije. */
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    /* Obavlja se OpenGL inicijalizacija. */
    glClearColor(0.6, 0.8, 1, 0);
    glEnable(GL_DEPTH_TEST);

    /* Ukljucujemo normalizaciju vektora normala */
    glEnable(GL_NORMALIZE);

    /* Program ulazi u glavnu petlju. */
    glutMainLoop();

    return 0;
}

static void on_keyboard(unsigned char key, int x, int y){
    switch (key) {
        case 'a':
        case 'A':
            if(anim_param1 > -0.95)
                anim_param1 -= 0.05;
            else
                anim_param1 -= 0.0;
            printf("1  %f\n", anim_param1);
            glutPostRedisplay();
            break;
        case 'd':
        case 'D':
            if(anim_param1 < 0.95)
                anim_param1 += 0.05;
            else
                anim_param1 += 0.0;
            printf("1  %f\n", anim_param1);
            glutPostRedisplay();
            break;
        case 's':
        case 'S':
            if(anim_param2 < 0.95)
                anim_param2 += 0.05;
            else
                anim_param1 += 0.0;
            printf("2  %f\n", anim_param2);
            glutPostRedisplay();
            break;
        case 'w':
        case 'W':
            if(anim_param2 > -0.95)
                anim_param2 -= 0.05;
            else
                anim_param2 -= 0.0;
            printf("2  %f\n", anim_param2);
            glutPostRedisplay();
            break;
        case 27:
            /* Zavrsava se program. */
            exit(0);
            break;
        
    }
}

static void on_reshape(int width, int height){
    /* Podesava se viewport. */
    glViewport(0, 0, width, height);

    /* Podesava se projekcija. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) width / height, 1, 50);
}

/* Crtamo ravan */
void plot_function(){
    glPushMatrix();
    glBegin(GL_POLYGON);
        glVertex3f(-1, 0, -1);
        glVertex3f(1, 0, -1);
        glVertex3f(1, 0, 1);
        glVertex3f(-1, 0, 1);
    glEnd();
    glPopMatrix();
}

static void on_display(void){
    /*Pozicija svetla (u pitanju je direkcionalno svetlo) */
    GLfloat light_position[] = { 1, 1, 1, 0 };
    /*Ambijentalna boja svetla */
    GLfloat light_ambient[] = { 0.1, 0.2, 0.1, 1 };
    /*Difuzna boja svetla */
    GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };
    /*Spekularna boja svetla */
    GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 };

    GLfloat ambient_coeffs[] = { 0.5, 0.9, 0.5, 1 };
    GLfloat diffuse_coeffs[] = { 0.5, 0.5, 0.5, 1 };
    GLfloat specular_coeffs[] = { 1, 0.1, 0.1, 1 };
    GLfloat shininess = 20;

    /*Brise se prethodni sadrzaj prozora */ 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*Podesava se vidna tacka */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(-1.5, 1, 1.8, 0, 0, 0, 0, 1, 0);

    /*Ukljucuje se osvjetljenje i podesavaju parametri svetla */ 
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    /*Podesavaju se parametri materijala */ 
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    glShadeModel(GL_SMOOTH);
    
    /*X*/
    
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(100, 0, 0);
    glEnd();
    /*Y*/ 
    
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 100, 0);
    glEnd();
        
    /*Z*/
    
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 100);
    glEnd();

    plot_function();
    
    /*glColor3f(0, 0.5, 0.5);*/
    glTranslatef(0, 0.05, 0);
    
    glPushMatrix();
      diffuse_coeffs[0] = 0.6;
      diffuse_coeffs[1] = 0.0;
      
      ambient_coeffs[0] = 0.6;
      ambient_coeffs[1] = 0.0;
      ambient_coeffs[2] = 0.6;
      
      glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
     
      glTranslatef(0+anim_param1, 0, 0+anim_param2);
      glutSolidCube(0.1);
    glPopMatrix();
    
    glTranslatef(0, -0.05, 0);
    

    /* Nova slika se salje na ekran. */
    glutSwapBuffers();
}

