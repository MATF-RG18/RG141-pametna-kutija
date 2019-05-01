#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h>
#include<math.h>

#define TIMER_INTERVAL 50
#define TIMER_ID 0

static void error(char* str, int lineNum){
    fprintf(stderr, "Greska: %s u liniji %d\n", str, lineNum);
    exit(EXIT_FAILURE);
}

#define ERROR(str) error(str, __LINE__);

/* Matrica koja cuva poligon */
static int **matrix;
static int n, m, tren_i, tren_j;
static int indikator = 0;
static void readMatrix(void);
static void allocMatrix(void);
static void freeMatrix(void);

/* Globalne promenljive, u svrhe animacija i kretanja kamere */
static float anim_param = 0;   /* Kretanje plocice */
static float anim_param1 = 0;  /* Pomeranje kamere */
static float anim_param2 = 0;  /* Pomeranje kamere */
static float rotation;         /* Ugao rotacije */

/* Fleg koji odredjuje stanje tajmera. */
static int timer_active;

/* Deklaracije callback funkcija. */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_timer(int value);

int main(int argc, char **argv){
    /* Inicijalizuje se GLUT */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Kreira se prozor */
    glutInitWindowSize(700, 550);
    glutInitWindowPosition(300, 50);
    glutCreateWindow(argv[0]);

    /* Registruju se callback funkcije */
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    /* Obavlja se OpenGL inicijalizacija */
    glClearColor(1, 1, 0.8, 0);
    glEnable(GL_DEPTH_TEST);

    /* Poligon prvog nivoa */
    readMatrix();
    
    /* Ukljucujemo normalizaciju vektora normala */
    glEnable(GL_NORMALIZE);

    /* Program ulazi u glavnu petlju */
    glutMainLoop();

    return 0;
}

static void on_keyboard(unsigned char key, int x, int y){
    switch (key) {
        /* Levo */
        case 'a':
        case 'A':
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
        case 'd':
        case 'D':
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
        case 's':
        case 'S':
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
        case 'w':
        case 'W':
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
        /* Pokrece se simulacija */
        case 'g':
        case 'G':
            if (!timer_active) {
                glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
                timer_active = 1;
            }
            break;
        /* restart */
        case 'r':
            anim_param = 0;
            anim_param1 = 0;
            anim_param2 = 0;
            readMatrix();
            
            glutPostRedisplay();
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
    gluPerspective(90, (float) width / height, 1, 100);
}

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
        f = fopen("matrica1.txt", "r"); 
        if(f == NULL)
            ERROR("Citanje matrice");
    }
    else if(indikator == 1){
        f = fopen("matrica2.txt", "r"); 
        if(f == NULL)
            ERROR("Citanje matrice");
    }
    else if(indikator == 2){
        f = fopen("matrica3.txt", "r"); 
        if(f == NULL)
            ERROR("Citanje matrice");
    }
    else if(indikator == 3){
        f = fopen("matrica4.txt", "r"); 
        if(f == NULL)
            ERROR("Citanje matrice");
    }
    else{
        printf("\n*** THE END ***\n\n");
        exit(EXIT_SUCCESS);
    }
    fscanf(f, "%d%d", &n, &m);
    
    allocMatrix();
    
    for(int k=0; k<n; k++){
        for(int l=0; l<m; l++)
            fscanf(f, "%d", &matrix[k][l]);
    }
    
    fclose(f);
}

static void freeMatrix(void){
    for(int i=0; i<n; i++){
        free(matrix[i]);
    }
    free(matrix);
}

static void on_display(void){
    /*Brise se prethodni sadrzaj prozora */ 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /*Podesava se vidna tacka */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(-1.4+anim_param1/10, 2.7, 2.7+anim_param2/10, 0, 0, 0, 0, 1, 0);
    
    /*Pozicija svetla (u pitanju je direkcionalno svetlo) */
    GLfloat light_position[] = { 0.6, 1.4, 0.9, 0 };
    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1 };
    GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1 };
    GLfloat light_specular[] = { 0.9, 0.9, 0.9, 1 };

    GLfloat ambient_coeffs[] = { 0, 0, 0, 1 };
    GLfloat diffuse_coeffs[] = { 0, 0, 0, 1 };
    GLfloat specular_coeffs[] = { 0, 0, 0, 1 };
    GLfloat shininess = 20;
    
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

    /* Transliramo mapu */
    glScalef(1.6, 1.6, 1.6);
    glTranslatef(-1.1, 0.1, -1.05);
    
    /* Iscrtavamo mapu/teren pomocu matrice */
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            
            /* Zidovi */
            if(matrix[i][j] == 1){
                glPushMatrix();
                    diffuse_coeffs[0] = 0.8;
                    diffuse_coeffs[1] = 0.1;
                    diffuse_coeffs[2] = 0.1;
                    
                    ambient_coeffs[0] = 0.7;
                    ambient_coeffs[1] = 0.7;
                    ambient_coeffs[2] = 0.7;
      
                    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
     
                    glTranslatef(((float)j/10.0)*2.0, 0, ((float)i/10.0)*2.0);
                    glutSolidCube(0.2);
                glPopMatrix();
                
            }
            /* Kutije koje igrac moze da pomera */
            else if(matrix[i][j] == 2){
                 glPushMatrix();
                    diffuse_coeffs[0] = 0.9;
                    diffuse_coeffs[1] = 0.1;
                    diffuse_coeffs[2] = 0.6;
                    
                    ambient_coeffs[0] = 0.7;
                    ambient_coeffs[1] = 0.5;
                    ambient_coeffs[2] = 0.6;
      
                    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
     
                    glTranslatef(((float)j/10.0)*2.0, 0, ((float)i/10.0)*2.0);
                    glutSolidCube(0.2);
                glPopMatrix();
            }
            /* Cilj */
            else if(matrix[i][j] == 3){
                glPushMatrix();
                    diffuse_coeffs[0] = 0.85;
                    diffuse_coeffs[1] = 0.85;
                    diffuse_coeffs[2] = 0.0;
                    
                    ambient_coeffs[0] = 0.9;
                    ambient_coeffs[1] = 0.9;
                    ambient_coeffs[2] = 1;
      
                    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
     
                    /* TODO */
                    glTranslatef(((float)j/10.0)*2.0, 0, ((float)i/10.0)*2.0);
                    glRotatef(rotation, 0, 1, 0);
                    glScalef(0.125, 0.125, 0.125);
                    glutSolidIcosahedron();
                glPopMatrix();
                
            }
            /* Plocice koje oznacavaju rupe */
            else if(matrix[i][j] == 4){
                glPushMatrix();
                    diffuse_coeffs[0] = 0.8;
                    diffuse_coeffs[1] = 0.05;
                    diffuse_coeffs[2] = 0.8;
                    
                    ambient_coeffs[0] = 0.6;
                    ambient_coeffs[1] = 0.6;
                    ambient_coeffs[2] = 0.6;
      
                    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
     
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
                    diffuse_coeffs[0] = 0;
                    diffuse_coeffs[1] = 0.8;
                    diffuse_coeffs[2] = 1;
                    
                    ambient_coeffs[0] = 0.5;
                    ambient_coeffs[1] = 0.5;
                    ambient_coeffs[2] = 0.5;
      
                    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
     
                    glTranslatef(((float)j/10.0)*2.0, 0, ((float)i/10.0)*2.0);
                    glutSolidCube(0.2);
                    
                glPopMatrix();
                
            }
            /* Drvece */
            else if(matrix[i][j] == 6){
                /* Stablo */
                glPushMatrix();
                    diffuse_coeffs[0] = 0.8;
                    diffuse_coeffs[1] = 0.2;
                    diffuse_coeffs[2] = 0.1;
                    
                    ambient_coeffs[0] = 0.3;
                    ambient_coeffs[1] = 0.3;
                    ambient_coeffs[2] = 0.3;
      
                    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
                
                    glTranslatef(((float)j/10.0)*2.0, 0, ((float)i/10.0)*2.0);
                    glScalef(0.5, 1, 0.5);
                    glutSolidCube(0.2);
                glPopMatrix();
                /* Krosnja */
                glPushMatrix();
                    diffuse_coeffs[0] = 0.2;
                    diffuse_coeffs[1] = 0.8;
                    diffuse_coeffs[2] = 0.1;
                    
                    ambient_coeffs[0] = 0.6;
                    ambient_coeffs[1] = 0.7;
                    ambient_coeffs[2] = 0.6;
      
                    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
                    
                    glTranslatef(((float)j/10.0)*2.0, 0.2, ((float)i/10.0)*2.0);
                    glScalef(1.1, 1.5, 1.1);
                    
                    double clip_plane1[] = {0, -0.1, 0, 0};
                    glClipPlane(GL_CLIP_PLANE0, clip_plane1);
                    glEnable(GL_CLIP_PLANE0);
                    
                    glutSolidCube(0.2);
                    
                    glDisable(GL_CLIP_PLANE0);
                glPopMatrix();
                glPushMatrix();
                    diffuse_coeffs[0] = 0.2;
                    diffuse_coeffs[1] = 0.7;
                    diffuse_coeffs[2] = 0.1;
                    
                    ambient_coeffs[0] = 0.4;
                    ambient_coeffs[1] = 0.4;
                    ambient_coeffs[2] = 0.4;
      
                    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
                    
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
                    diffuse_coeffs[0] = 0.4;
                    diffuse_coeffs[1] = 1;
                    diffuse_coeffs[2] = 0.5;
                    
                    ambient_coeffs[0] = 0.9;
                    ambient_coeffs[1] = 1;
                    ambient_coeffs[2] = 0.9;
      
                    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
     
                    glTranslatef(((float)j/10.0)*2.0, 0, ((float)i/10.0)*2.0);
                    glScalef(1.5, 2.2, 0.25);
                    glutSolidCube(0.1);
                    
                glPopMatrix();
            }
            /* Kljuc koji otvara vrata */
            else if(matrix[i][j] == 8){
                glPushMatrix();
                    diffuse_coeffs[0] = 0.4;
                    diffuse_coeffs[1] = 1;
                    diffuse_coeffs[2] = 0.5;
                    
                    ambient_coeffs[0] = 0.9;
                    ambient_coeffs[1] = 1;
                    ambient_coeffs[2] = 0.9;
      
                    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
     
                    glTranslatef(((float)j/10.0)*2.0, 0.1, ((float)i/10.0)*2.0);
                    glRotatef(rotation, 1, 1, 0);
                    glutSolidCube(0.1);
                    
                glPopMatrix();
            }
            else 
                continue;
            
        }
    }
    glTranslatef(1.1, -0.1, 1.05);
    /* Nova slika se salje na ekran. */
    glutSwapBuffers();
}