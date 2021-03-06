/* Student:  Lea Petkovic 163/2016
   Naziv projekta: Pametna kutija
   Predmet: Racunarska grafika */
#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>
#include<math.h>
#include"../header/SOIL.h"
#include"../header/lights.h"

/* Indikator koji govori da li je predjen poslednji nivo */
static int id = 0;

/* Funkcija za obradu gresaka i njen makro */
static void error(char* str, int lineNum){
    fprintf(stderr, "Greska: %s u liniji %d\n", str, lineNum);
    exit(EXIT_FAILURE);
}
#define ERROR(str) error(str, __LINE__);

/* Matrica koja cuva poligon i neophodne fje i informacije za matricu */
static int **matrix;
static int n, m, tren_i, tren_j;
static int indikator = 0;
static void readMatrix(void);
static void allocMatrix(void);
static void freeMatrix(void);

/* Globalne promenljive, u svrhu animacije i kretanja kamere */
static float anim_param = 0;   /* Kretanje plocice */
static float anim_param1 = 0;  /* Pomeranje kamere */
static float anim_param2 = 0;  /* Pomeranje kamere */
static float rotation;         /* Ugao rotacije */

/* Makroi za tajmer i fleg koji odredjuje stanje tajmera */
#define TIMER_INTERVAL 50
#define TIMER_ID 0
static int timer_active = 1;
/* Teksture, inicijalizacija, promenljive koje cuvaju  teksture */
GLuint flower, end, sky;
void init_tex();

/* Deklaracije callback funkcija. */
static void on_keyboard_special(int key, int x, int y);
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_timer(int value);

int main(int argc, char **argv){
    /* Inicijalizuje se GLUT */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

    /* Kreira se prozor */
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(300, 50);
    glutCreateWindow(argv[0]);

    /* Registruju se callback funkcije */
    glutSpecialFunc(on_keyboard_special);
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
    
    /* Obavlja se OpenGL inicijalizacija */
    glClearColor(0.5, 0.9, 1, 0);
    glEnable(GL_DEPTH_TEST);
    /* Ukljucujemo normalizaciju vektora normala */
    glEnable(GL_NORMALIZE);
    
    /* Fja generise poligon, prvi nivo */
    readMatrix();
    /* Inicijalizujemo teksture */
    init_tex();
    /* Program ulazi u glavnu petlju */
    glutMainLoop();

    return 0;
}

/* Kretanje pomocu strelica na tastaturi */
static void on_keyboard_special(int key, int x, int y){
    switch (key) {
        /* Levo */
        case GLUT_KEY_LEFT:
            if(tren_i<n && tren_i>=0 && tren_j>=1 && tren_j<m  /* Prazno polje */
               && matrix[tren_i][tren_j-1] == 0){
                
                matrix[tren_i][tren_j] = 0;
                matrix[tren_i][tren_j-1] = 5;
                
                anim_param1 -= 0.1;
            }
            else if(tren_i<n && tren_i>=0 && tren_j>=1 && tren_j<m  /* Rupa */
               && matrix[tren_i][tren_j-1] == 4){
                
                matrix[tren_i][tren_j] = 0;
                printf("GAME OVER\n");
                exit(0);
            }
            else if(tren_i<n && tren_i>=0 && tren_j>=2 && tren_j<m 
               && matrix[tren_i][tren_j-1] == 2 && matrix[tren_i][tren_j-2] == 8){
                
                matrix[tren_i][tren_j] = 0;
                matrix[tren_i][tren_j-1] = 5;
                matrix[tren_i][tren_j-2] = 2;
                
                for(int i=0; i<n; i++){
                    for(int j=0; j<m; j++){
                        if(matrix[i][j] == 7){
                            matrix[i][j] = 0;
                            break;
                        }
                    }
                }
                
                anim_param1 -= 0.1;
            }
            else if(tren_i<n && tren_i>=0 && tren_j>=2 && tren_j<m 
               && matrix[tren_i][tren_j-1] == 2 && matrix[tren_i][tren_j-2] == 0){
                
                matrix[tren_i][tren_j] = 0;
                matrix[tren_i][tren_j-1] = 5;
                matrix[tren_i][tren_j-2] = 2;
                
                anim_param1 -= 0.1;
            }
            else if(tren_i<n && tren_i>=0 && tren_j>=2 && tren_j<m 
               && matrix[tren_i][tren_j-1] == 2 && matrix[tren_i][tren_j-2] == 4){
                
                matrix[tren_i][tren_j] = 0;
                matrix[tren_i][tren_j-1] = 5;
                matrix[tren_i][tren_j-2] = 0;
                
                anim_param1 -= 0.1;
            }
            else if(tren_i<n && tren_i>=0 && tren_j>=1 && tren_j<m 
               && matrix[tren_i][tren_j-1] == 3){       /* Naisli smo na cilj */
                matrix[tren_i][tren_j] = 0;
                matrix[tren_i][tren_j-1] = 5;
                
                anim_param1 -= 0.1;
                
                indikator++;
                
                /* Oslobadjanje matrice, kako ne bi doslo do curenja memorije */
                freeMatrix();
                /* Naredni nivo */
                readMatrix();
            }
            glutPostRedisplay();
            break;
        /* Desno */
        case GLUT_KEY_RIGHT:
            if(tren_i>=0 && tren_i<n && tren_j>=0 && tren_j<m-1 
               && matrix[tren_i][tren_j+1] == 0){
                
                matrix[tren_i][tren_j] = 0;
                matrix[tren_i][tren_j+1] = 5;
                
                anim_param1 += 0.1;
            }
            else if(tren_i>=0 && tren_i<n && tren_j>=0 && tren_j<m-1 
               && matrix[tren_i][tren_j+1] == 4){
                
                matrix[tren_i][tren_j] = 0;
                printf("GAME OVER\n");
                exit(0);
            }
            else if(tren_i<n && tren_i>=0 && tren_j>=1 && tren_j<m-2 
               && matrix[tren_i][tren_j+1] == 2 && matrix[tren_i][tren_j+2] == 0){
                
                matrix[tren_i][tren_j] = 0;
                matrix[tren_i][tren_j+1] = 5;
                matrix[tren_i][tren_j+2] = 2;
                
                anim_param1 += 0.1;
            }
            else if(tren_i<n && tren_i>=0 && tren_j>=0 && tren_j<m-2 
               && matrix[tren_i][tren_j+1] == 2 && matrix[tren_i][tren_j+2] == 8){
                
                matrix[tren_i][tren_j] = 0;
                matrix[tren_i][tren_j+1] = 5;
                matrix[tren_i][tren_j+2] = 2;
                
                for(int i=0; i<n; i++){
                    for(int j=0; j<m; j++){
                        if(matrix[i][j] == 7){
                            matrix[i][j] = 0;
                            break;
                        }
                    }
                }
                anim_param1 += 0.1;
            }
            else if(tren_i<n && tren_i>=0 && tren_j>=1 && tren_j<m-2 
               && matrix[tren_i][tren_j+1] == 2 && matrix[tren_i][tren_j+2] == 4){
                
                matrix[tren_i][tren_j] = 0;
                matrix[tren_i][tren_j+1] = 5;
                matrix[tren_i][tren_j+2] = 0;
                
                anim_param1 += 0.1;
            }
            else if(tren_i>=0 && tren_i<n && tren_j>=0 && tren_j<m-1 
                    && matrix[tren_i][tren_j+1] == 3){       /* Naisli smo na cilj */
                
                matrix[tren_i][tren_j] = 0;
                matrix[tren_i][tren_j+1] = 5;
                
                anim_param1 += 0.1;
                indikator++;
            
                /* Oslobadjanje matrice, kako ne bi doslo do curenja memorije */
                freeMatrix();
                /* Naredni nivo */
                readMatrix();
            }
            
            glutPostRedisplay();
            break;
        /* Dole */
        case GLUT_KEY_DOWN:
            if(tren_i>=0 && tren_i<n-1 && tren_j>=0 && tren_j<m 
               && matrix[tren_i+1][tren_j] == 0){
                
                matrix[tren_i][tren_j] = 0;
                matrix[tren_i+1][tren_j] = 5;
                
                anim_param2 += 0.1;
            }
            else if(tren_i>=0 && tren_i<n-1 && tren_j>=0 && tren_j<m 
               && matrix[tren_i+1][tren_j] == 4){
                
                matrix[tren_i][tren_j] = 0;
                printf("GAME OVER\n");
                exit(0);
                
                anim_param2 += 0.1;
            }
            else if(tren_i<n-2 && tren_i>=0 && tren_j>=0 && tren_j<m 
               && matrix[tren_i+1][tren_j] == 2 && matrix[tren_i+2][tren_j] == 0){
                
                matrix[tren_i][tren_j] = 0;
                matrix[tren_i+1][tren_j] = 5;
                matrix[tren_i+2][tren_j] = 2;
                
                anim_param2 += 0.1;
            }
            else if(tren_i<n-2 && tren_i>=0 && tren_j>=0 && tren_j<m 
               && matrix[tren_i+1][tren_j] == 2 && matrix[tren_i+2][tren_j] == 8){
                
                matrix[tren_i][tren_j] = 0;
                matrix[tren_i+1][tren_j] = 5;
                matrix[tren_i+2][tren_j] = 2;
                
                for(int i=0; i<n; i++){
                    for(int j=0; j<m; j++){
                        if(matrix[i][j] == 7){
                            matrix[i][j] = 0;
                            break;
                        }
                    }
                }
                anim_param2 += 0.1;
            }
            else if(tren_i<n-2 && tren_i>=0 && tren_j>=0 && tren_j<m 
               && matrix[tren_i+1][tren_j] == 2 && matrix[tren_i+2][tren_j] == 4){
                
                matrix[tren_i][tren_j] = 0;
                matrix[tren_i+1][tren_j] = 5;
                matrix[tren_i+2][tren_j] = 0;
                
                anim_param2 += 0.1;
            }
            else if(tren_i>=0 && tren_i<n-1 && tren_j>=0 && tren_j<m 
                    && matrix[tren_i+1][tren_j] == 3){       /* Naisli smo na cilj */
                
                matrix[tren_i][tren_j] = 0;
                matrix[tren_i+1][tren_j] = 5;
                
                anim_param2 += 0.1;
                indikator++;
            
                /* Oslobadjanje matrice, kako ne bi doslo do curenja memorije */
                freeMatrix();
                /* Naredni nivo */
                readMatrix();
            }
            glutPostRedisplay();
            break;
        /* Gore */
        case GLUT_KEY_UP:
            if(tren_i>=1 && tren_i<n && tren_j>=0 && tren_j<m 
               && matrix[tren_i-1][tren_j] == 0){
                
                matrix[tren_i][tren_j] = 0;
                matrix[tren_i-1][tren_j] = 5;
                
                anim_param2 -= 0.1;
            }
            else if(tren_i>=1 && tren_i<n && tren_j>=0 && tren_j<m 
               && matrix[tren_i-1][tren_j] == 4){
                
                matrix[tren_i][tren_j] = 0;
                printf("GAME OVER\n");
                exit(0);
                anim_param2 -= 0.1;
            }
            else if(tren_i<n && tren_i>=2 && tren_j>=0 && tren_j<m 
               && matrix[tren_i-1][tren_j] == 2 && matrix[tren_i-2][tren_j] == 0){
                
                matrix[tren_i][tren_j] = 0;
                matrix[tren_i-1][tren_j] = 5;
                matrix[tren_i-2][tren_j] = 2;
                
                anim_param2 -= 0.1;
            }
            else if(tren_i<n && tren_i>=2 && tren_j>=0 && tren_j<m 
               && matrix[tren_i-1][tren_j] == 2 && matrix[tren_i-2][tren_j] == 8){
                
                matrix[tren_i][tren_j] = 0;
                matrix[tren_i-1][tren_j] = 5;
                matrix[tren_i-2][tren_j] = 2;
                
                for(int i=0; i<n; i++){
                    for(int j=0; j<m; j++){
                        if(matrix[i][j] == 7){
                            matrix[i][j] = 0;
                            break;
                        }
                    }
                }
                anim_param2 -= 0.1;
            }
            else if(tren_i<n && tren_i>=2 && tren_j>=0 && tren_j<m 
               && matrix[tren_i-1][tren_j] == 2 && matrix[tren_i-2][tren_j] == 4){
                
                matrix[tren_i][tren_j] = 0;
                matrix[tren_i-1][tren_j] = 5;
                matrix[tren_i-2][tren_j] = 0;
                
                anim_param2 -= 0.1;
            }
            else if(tren_i>=1 && tren_i<n && tren_j>=0 && tren_j<m 
                   && matrix[tren_i-1][tren_j] == 3){       /* Naisli smo na cilj */
                
                matrix[tren_i][tren_j] = 0;
                matrix[tren_i-1][tren_j] = 5;
                
                anim_param2 -= 0.1;
                indikator++;
            
                /* Oslobadjanje matrice, kako ne bi doslo do curenja memorije */
                freeMatrix();
                /* Naredni nivo */
                readMatrix();
            }
            glutPostRedisplay();
            break;
    }
}

/* Resetovanje i izlaz iz programa putem tastature */
static void on_keyboard(unsigned char key, int x, int y){
    switch (key) {
        /* restart */
        case 'r':
            if(id != 1){
                anim_param = 0;
                anim_param1 = 0;
                anim_param2 = 0;
                readMatrix();
                
                glutPostRedisplay();
                
            }
            break;
        /* Zavrsava se program */
        case 27:
            timer_active = 0;
            exit(0);
            break;
    }
}

static void on_timer(int value){
    /* Proverava se da li callback dolazi od odgovarajuceg tajmera */
    if (value != TIMER_ID)
        return;

    rotation += 10;
    anim_param += 0.1;
    
    /* Forsira se ponovno iscrtavanje prozora */
    glutPostRedisplay();
    /* Po potrebi se ponovo postavlja tajmer */
    if (timer_active)
        glutTimerFunc(50, on_timer, 0);
}

static void on_reshape(int width, int height){
    /* Podesava se viewport */
    glViewport(0, 0, width, height);

    /* Podesava se projekcija */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, (float) width/height, 1, 250);
}

static void on_display(void){
    /* Full screen mode */
    glutFullScreen();
    /*Brise se prethodni sadrzaj prozora */ 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    /*Podesava se vidna tacka */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(-1.4+anim_param1/10, 2.7, 2.7+anim_param2/10, 0, 0, 0, 0, 1, 0);

    /* Osvetljenje i materijal se podesavaju */
    init_lights();
    glShadeModel(GL_SMOOTH);

    /* Ako jos nisu predjeni svi nivoi, iscrtava se tekuci */
    if(id == 0){
        glTranslatef(0, 0, 0.15);
        /* Omogucavamo teksture */
        glEnable(GL_TEXTURE_2D);
        /* Cvece za podijum */
        glBindTexture(GL_TEXTURE_2D, flower);
        glBegin(GL_QUADS);
            glNormal3f(0, 1, 0);

            glTexCoord2f(0, 0);
            glVertex3f(-2.0*m/10.0, 0, 2.0*n/10.0);

            glTexCoord2f(5, 0);
            glVertex3f(2.0*m/10.0, 0, 2.0*n/10.0);

            glTexCoord2f(5, 5);
            glVertex3f(2.0*m/10.0, 0, -2.0*n/10.0);

            glTexCoord2f(0, 5);
            glVertex3f(-2.0*m/10.0, 0, -2.0*n/10.0);
        glEnd();
        glTranslatef(0, 0, -0.15);
        
        /* POzadina, nebo */
        glTranslatef(10, -9, -10);
        glRotatef(-30, 0,1,0);
        glRotatef(50, 1,0,0);
        glBindTexture(GL_TEXTURE_2D, sky);
        glBegin(GL_QUADS);
            glNormal3f(1, -1, 1);

            glTexCoord2f(0, 0);
            glVertex3f(-50.0, 0, 50.0);

            glTexCoord2f(3, 0);
            glVertex3f(50.0, 0, 50.0);

            glTexCoord2f(3, 3);
            glVertex3f(50.0, 0, -50.0);

            glTexCoord2f(0, 3);
            glVertex3f(-50.0, 0, -50.0);
        glEnd();
        glRotatef(-50, 1,0,0);
        glRotatef(30, 0,1,0);
        glTranslatef(-10, 9, 10);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_COLOR_MATERIAL);
        
        /* Transliramo i skaliramo poligon, kako bi bio na sredini ekrana, vidne tacke */
        glScalef(1.6, 1.6, 1.6);
        glTranslatef(-1.1, 0.1, -1.05);
        
        /* Iscrtavamo mapu/teren pomocu matrice */
        for(int i=0; i<n; i++){
            for(int j=0; j<m; j++){
                
                /* Zidovi */
                if(matrix[i][j] == 1){
                    glPushMatrix();
                        glTranslatef(((float)j/10.0)*2.0, 0, ((float)i/10.0)*2.0);
                        glColor3f(1, 0.5, 0.5);
                        glutSolidCube(0.2);
                    glPopMatrix();
                    
                }
                /* Kutije koje igrac moze da pomera */
                else if(matrix[i][j] == 2){
                    glPushMatrix();
                        glColor3f(0.75, 0.05, 0.6);
                        glTranslatef(((float)j/10.0)*2.0, 0, ((float)i/10.0)*2.0);
                        glutSolidCube(0.2);
                    glPopMatrix();
                }
                /* Cilj */
                else if(matrix[i][j] == 3){
                    glPushMatrix();
                        glColor3f(1, 1, 0);
                        glTranslatef(((float)j/10.0)*2.0, 0, ((float)i/10.0)*2.0);
                        glRotatef(rotation, 0, 1, 0);
                        glScalef(0.125, 0.125, 0.125);
                        glutSolidIcosahedron();
                    glPopMatrix();
                    
                }
                /* Plocice koje oznacavaju rupe */
                else if(matrix[i][j] == 4){
                    glPushMatrix();
                        glColor3f(0,0,0);
                    
                        glTranslatef(((float)j/10.0)*2.0, 0+sin(anim_param)/8, ((float)i/10.0)*2.0);
                        glBegin(GL_POLYGON);
                        glVertex3f(-0.1, 0.2, -0.1);
                        glVertex3f(0.1, 0.2, -0.1);
                        glVertex3f(0.1, 0.2, 0.1);
                        glVertex3f(-0.1, 0.2, 0.1);
                        glEnd();
                    glPopMatrix();
                }
                /* Igrac */
                else if(matrix[i][j] == 5){
                    tren_i = i;
                    tren_j = j;
                    glPushMatrix();
                        glColor3f(0.01, 0.7, 0.85);
                        glTranslatef(((float)j/10.0)*2.0, 0, ((float)i/10.0)*2.0);
                        glutSolidCube(0.2);
                        
                    glPopMatrix();
                    
                }
                /* Drvece */
                else if(matrix[i][j] == 6){
                    /* Stablo */
                    glPushMatrix();
                        glColor3f(0.8, 0.2, 0.0);
                        glTranslatef(((float)j/10.0)*2.0, 0, ((float)i/10.0)*2.0);
                        glScalef(0.5, 1, 0.5);
                        glutSolidCube(0.2);
                    glPopMatrix();
                    /* Krosnja */
                    glPushMatrix();
                    
                        glColor3f(0, 0.8, 0.2);
                        glTranslatef(((float)j/10.0)*2.0, 0.2, ((float)i/10.0)*2.0);
                        glScalef(1.1, 1.5, 1.1);
                        
                        double clip_plane1[] = {0, -0.1, 0, 0};
                        glClipPlane(GL_CLIP_PLANE0, clip_plane1);
                        glEnable(GL_CLIP_PLANE0);
                        
                        glutSolidCube(0.2);
                        
                        glDisable(GL_CLIP_PLANE0);
                    glPopMatrix();
                    glPushMatrix();
                        glColor3f(0, 0.6, 0.2);
                        glTranslatef(((float)j/10.0)*2.0, 0.2, ((float)i/10.0)*2.0);
                        glScalef(1.1, 1.5, 1.1);
                        
                        double clip_plane2[] = {0, 0.1, 0, 0};

                        glClipPlane(GL_CLIP_PLANE0, clip_plane2);
                        glEnable(GL_CLIP_PLANE0);
                        glutSolidCube(0.2);
                        glDisable(GL_CLIP_PLANE0);
                    glPopMatrix();
                }
                /* Vrata */
                else if(matrix[i][j] == 7){
                    glPushMatrix();
                        glColor3f(1, 0.1, 0);
                        glTranslatef(((float)j/10.0)*2.0, 0, ((float)i/10.0)*2.0);
                        glScalef(1.5, 2.2, 0.25);
                        glutSolidCube(0.1);
                        
                    glPopMatrix();
                }
                /* Kljuc koji otvara vrata */
                else if(matrix[i][j] == 8){
                    glPushMatrix();
                        glColor3f(1, 0.1, 0);
                        glTranslatef(((float)j/10.0)*2.0, 0.1, ((float)i/10.0)*2.0);
                        glRotatef(rotation, 1, 1, 0);
                        glutSolidCube(0.1);
                        
                    glPopMatrix();
                }
                else if(matrix[i][j] == 9){
                    /* Stablo */
                    glPushMatrix();
                        glColor3f(0.8, 0.2, 0.0);
                        glTranslatef(((float)j/10.0)*2, 0, ((float)i/10.0)*2);
                        glScalef(0.5, 1, 0.5);
                        glutSolidCube(0.15);
                    glPopMatrix();
                    /* Krosnja */
                    glPushMatrix();
                        glColor3f(0, 0.3, 0.1);
                        glTranslatef(((float)j/10.0)*2.0, 0.1, ((float)i/10.0)*2.0);
                        glRotatef(90,-1,0,0);
                        
                        glutSolidCone(0.2, 0.6, 10,10);
                        
                        glDisable(GL_CLIP_PLANE0);
                    glPopMatrix();
                }
                else 
                    continue;
            }
        }
        glTranslatef(1.1, -0.1, 1.05);
    }
    /* The end - postavlja se zavrsna tekstura */
    else{
        /* Podesavase vidna tacka */
        glLoadIdentity();
        gluLookAt(-1, 0, 0, 
                   0, 0, 0, 
                   0, 1, 0);
        /* Transliramo i skaliramo teksturu kako bi bila na sredini i preko celog ekrana */
        glTranslatef(0,-50,0);
        glScalef(0.8,1,1.5);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, end);
        glBegin(GL_QUADS);
            glNormal3f(1, -1, 1);

            glTexCoord2f(0, 0);
            glVertex3f(50, 0, -50);
 
            glTexCoord2f(1, 0);
            glVertex3f(50, 0, 50);
   
            glTexCoord2f(1, 1);
            glVertex3f(50, 100, 50);

            glTexCoord2f(0, 1);
            glVertex3f(50, 100, -50);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_COLOR_MATERIAL);
    }
    /* Nova slika se salje na ekran. */
    glutSwapBuffers();
}

/* Alokacija matrice */
static void allocMatrix(void){
    matrix = malloc(n*sizeof(int*));
    if(matrix == NULL)
        ERROR("Alokacija matrice");
    
    for(int i=0; i<n; i++){
        matrix[i] = malloc(m*sizeof(int));
        if(matrix[i] == NULL)
            ERROR("Alokacija matrice");
    }
}

/* Citamo matricu iz datoteke */
static void readMatrix(void){
    FILE* f;
    if(indikator == 0){
        f = fopen("./Matrix/matrica1.txt", "r"); 
        if(f == NULL)
            ERROR("Citanje matrice");
    }
    else if(indikator == 1){
        f = fopen("./Matrix/matrica2.txt", "r"); 
        if(f == NULL)
            ERROR("Citanje matrice");
    }
    else if(indikator == 2){
        f = fopen("./Matrix/matrica3.txt", "r"); 
        if(f == NULL)
            ERROR("Citanje matrice");
    }
    else if(indikator == 3){
        f = fopen("./Matrix/matrica4.txt", "r"); 
        if(f == NULL)
            ERROR("Citanje matrice");
    }
    else if(indikator == 4){
        f = fopen("./Matrix/matrica5.txt", "r"); 
        if(f == NULL)
            ERROR("Citanje matrice");
    }
    else{
        printf("\n*** THE END ***\n\n");
        id++;
        glutPostRedisplay();
        return;
    }
    fscanf(f, "%d%d", &n, &m);
    
    allocMatrix();
    
    for(int k=0; k<n; k++){
        for(int l=0; l<m; l++)
            fscanf(f, "%d", &matrix[k][l]);
    }
    
    fclose(f);
}

/* Oslobadjanje memorije */
static void freeMatrix(void){
    for(int i=0; i<n; i++){
        free(matrix[i]);
    }
    free(matrix);
}

/* Inicijalizacija tekstura */
void init_tex(){
    /* Inicijalizuje se tekstura sa cvecem */
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    flower = SOIL_load_OGL_texture("./Texture/cvece.png", 
                                SOIL_LOAD_AUTO, 
                                SOIL_CREATE_NEW_ID, 
                                SOIL_FLAG_INVERT_Y);
    /* U slucaju da ucitavanje strukture nije uspelo, ispisati vrstu greske i prekinuti program */
    if(flower == 0){
        printf("%s\n", SOIL_last_result());
        ERROR("Nije ucitana tekstura");
    }

    glBindTexture(GL_TEXTURE_2D, flower);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);
    
    /* Nebo - inicijalizacija teksture */
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    sky = SOIL_load_OGL_texture("./Texture/sky.png", 
                                SOIL_LOAD_AUTO, 
                                SOIL_CREATE_NEW_ID, 
                                SOIL_FLAG_INVERT_Y);
    /* U slucaju da ucitavanje strukture nije uspelo, ispisati vrstu greske i prekinuti program */
    if(sky == 0){
        printf("%s\n", SOIL_last_result());
        ERROR("Nije ucitana tekstura - sky");
    }

    glBindTexture(GL_TEXTURE_2D, sky);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);
    
    
    /* Kraj igrice - inicijalizacija teksture */
    glTexEnvf(GL_TEXTURE_ENV, 
              GL_TEXTURE_ENV_MODE, 
              GL_REPLACE);

    end = SOIL_load_OGL_texture("./Texture/theend.png", 
                                SOIL_LOAD_AUTO, 
                                SOIL_CREATE_NEW_ID, 
                                SOIL_FLAG_INVERT_Y);
    if(end == 0){
        printf("%s\n", SOIL_last_result());
        ERROR("Nije ucitana tekstura");
    }

    glBindTexture(GL_TEXTURE_2D, end);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);
}
