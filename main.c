#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>
#include<math.h>

#define TIMER_INTERVAL 50
#define TIMER_ID 0

static int **matrix;
int n, m;
static void readMatrix();

/*static int active_function; identifikator aktuelne funkcije */
static float anim_param1 = 0;
static float anim_param2 = 0;
static float rotation;

/* Fleg koji odredjuje stanje tajmera. */
static int timer_active;

/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_timer(int value);

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
    glClearColor(1, 1, 0.8, 0);
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
            anim_param1 -= 0.2;
            
            printf("1  %f\n", anim_param1);
            glutPostRedisplay();
            break;
        case 'd':
        case 'D':
            anim_param1 += 0.2;
            printf("1  %f\n", anim_param1);
            glutPostRedisplay();
            break;
        case 's':
        case 'S':
            anim_param2 += 0.2;
            
            printf("2  %f\n", anim_param2);
            glutPostRedisplay();
            break;
        case 'w':
        case 'W':
            anim_param2 -= 0.2;
            
            printf("2  %f\n", anim_param2);
            glutPostRedisplay();
            break;
        case 'g':
        case 'G':
            /* Pokrece se simulacija. */
            if (!timer_active) {
                glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
                timer_active = 1;
            }
            break;
        case 27:
            /* Zavrsava se program. */
            exit(0);
            break;
        
    }
}

static void on_timer(int value){
    /* Proverava se da li callback dolazi od odgovarajuceg tajmera. */
    if (value != TIMER_ID)
        return;

    rotation += 10;
    
    /* Forsira se ponovno iscrtavanje prozora. */
    glutPostRedisplay();
    /* Po potrebi se ponovo postavlja tajmer. */
    if (timer_active)
        glutTimerFunc(50, on_timer, 0);
}

static void on_reshape(int width, int height){
    /* Podesava se viewport. */
    glViewport(0, 0, width, height);

    /* Podesava se projekcija. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, (float) width / height, 1, 100);
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

static void readMatrix(){
    FILE* f = fopen("matrica1.txt", "r"); 
    if(f == NULL){
        fprintf(stderr, "Greska pri otvaranju datoteke\n");
        exit(EXIT_FAILURE);
    }
    
    fscanf(f, "%d%d", &n, &m);
    /*printf("Dimenzije, NxM: %d %d\n\n", n , m);*/
    
    matrix = malloc(n*sizeof(int*));
    if(matrix == NULL){
        fprintf(stderr, "Greska pri alociranju matrice 1\n");
        exit(EXIT_FAILURE);
    }
    
    for(int i=0; i<n; i++){
        matrix[i] = malloc(m*sizeof(int));
        if(matrix[i] == NULL){
            fprintf(stderr, "Greska pri alociranju matrice 2\n");
            exit(EXIT_FAILURE);
        }
    }
    
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++)
            fscanf(f, "%d", &matrix[i][j]);
    }
    
    /*
     Ispis matrice
     for(int i=0; i<n; i++){
        for(int j=0; j<m; j++)
            printf("%d ", matrix[i][j]);
        printf("\n");
    }*/
    
    
    fclose(f);
}

static void on_display(void){
    readMatrix();
    
    /*Pozicija svetla (u pitanju je direkcionalno svetlo) */
    GLfloat light_position[] = { 0.6, 1, 0.8, 0 };
    /*Ambijentalna boja svetla */
    GLfloat light_ambient[] = { 0.1, 0.2, 0.1, 1 };
    /*Difuzna boja svetla */
    GLfloat light_diffuse[] = { 0.9, 0.6, 0.6, 1 };
    /*Spekularna boja svetla */
    GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 };

    GLfloat ambient_coeffs[] = { 0.9, 0.9, 0.9, 1 };
    GLfloat diffuse_coeffs[] = { 0.5, 0.5, 0.5, 1 };
    GLfloat specular_coeffs[] = { 1, 0.1, 0.1, 1 };
    GLfloat shininess = 20;

    /*Brise se prethodni sadrzaj prozora */ 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*Podesava se vidna tacka */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(-1.2+anim_param1/10, 2.3, 2.5+anim_param2/10, 0, 0, 0, 0, 1, 0);

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

    /* iscrtaj pomocnu ravan
     plot_function();*/
    
    /*glColor3f(0, 0.5, 0.5);*/
    glTranslatef(-1, 0.1, 0);
    
    
    glutWireCube(0.2);
    
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            
            if(matrix[i][j] == 1){
                glPushMatrix();
                    diffuse_coeffs[0] = 0.8;
                    diffuse_coeffs[1] = 0.5;
                    diffuse_coeffs[2] = 0.5;
                    
                    ambient_coeffs[0] = 0.5;
                    ambient_coeffs[1] = 0.5;
                    ambient_coeffs[2] = 0.5;
      
                    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
     
                    /*printf("%d %d\n", i, j);*/
                    glTranslatef(((float)j/10.0)*2.0, 0, ((float)i/10.0)*2.0);
                    glutSolidCube(0.2);
                glPopMatrix();
                
            }
            else if(matrix[i][j] == 2){
                 glPushMatrix();
                    diffuse_coeffs[0] = 0.1;
                    diffuse_coeffs[1] = 1;
                    diffuse_coeffs[2] = 1;
                    
                    ambient_coeffs[0] = .5;
                    ambient_coeffs[1] = 0.5;
                    ambient_coeffs[2] = 0.5;
      
                    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
     
                    /*printf("%d %d\n", i, j);*/
                    glTranslatef(((float)j/10.0)*2.0, 0, ((float)i/10.0)*2.0);
                    glutSolidCube(0.2);
                glPopMatrix();
            }
            else if(matrix[i][j] == 3){
                glPushMatrix();
                    diffuse_coeffs[0] = 0.3;
                    diffuse_coeffs[1] = 0.3;
                    diffuse_coeffs[2] = 0.9;
                    
                    ambient_coeffs[0] = 0.5;
                    ambient_coeffs[1] = 0.5;
                    ambient_coeffs[2] = 0.5;
      
                    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
     
                    /*printf("%d %d\n", i, j);*/
                    glTranslatef(((float)j/10.0)*2.0, 0, ((float)i/10.0)*2.0);
                    glRotatef(rotation, 0, 1, 0);
                    glScalef(0.125, 0.125, 0.125);
                    glutSolidIcosahedron();
                glPopMatrix();
                
            }
            else if(matrix[i][j] == 5){
                glPushMatrix();
                    diffuse_coeffs[0] = 0.6;
                    diffuse_coeffs[1] = 0.0;
      
                    ambient_coeffs[0] = 0.6;
                    ambient_coeffs[1] = 0.0;
                    ambient_coeffs[2] = 0.6;
      
                    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
     
                    /*printf("%d %d\n", i, j);*/
                    glTranslatef(((float)j/10.0)*2.0 + anim_param1, 0, ((float)i/10.0)*2.0 + anim_param2);
                    glutSolidCube(0.2);
                glPopMatrix();
                
            }
            else 
                continue;
            
        }
    }
    
    glTranslatef(1, -0.1, 0);
    

    /* Nova slika se salje na ekran. */
    glutSwapBuffers();
}

