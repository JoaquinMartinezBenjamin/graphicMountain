//
//  main.cpp
//  Montana
//
//  Created by Edgar alfred Rodriguez on 27/11/16.
//  Copyright © 2016 alfred. All rights reserved.
//

#include <iostream>
#include <openGL/gl.h>
#include <GLUT/glut.h>
#include <iostream>
#include <math.h>
#define VALOR_INVALIDO -1024
//#include <float.h>

void display(void);
void normalizacion(float[3]);
void productoCruzado(float[3], float[3], float[3]);
void init(void);
void formaNormal(GLfloat[3], GLfloat[3], GLfloat[3], GLfloat[3]);
void buscar_y_superior(void);
void hacerMontania();
void TeclasEscucha(unsigned char key, int x, int y);
void ArrowKey(int key,int x,int y);
void reshape(int w, int h);
GLfloat yAleatorio(GLfloat, GLfloat, GLfloat);

static GLdouble modelTrans[] = { -0.5, 0.0, 10.0 };
static GLfloat angulo[] = { 30.0, 0.0, 0.0 };
static float incThetha = 15.0;
static GLfloat *yVals;
static float rugosidad = 0.25;
static int division_malla = 9;
static int tam_arre = 0;
static int arreTotal = 0;
static GLfloat highestY = 0;
static double al = 3.5;
static double bl = 3.5;
static double cl = 3.5;
GLfloat luzX = 0.0;
GLfloat luzY = 5.0;
GLfloat luzZ = 5.0;
GLfloat luz = 10;
GLfloat mov = 0.1;


int main(int argc, char* argv[]) {
    hacerMontania();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1200, 900);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Fractal Montaña");
    glEnable(GL_DEPTH_TEST);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(TeclasEscucha);
    glutSpecialFunc(ArrowKey);
    glutMainLoop();
    return 0;
}

void init(void){
    //glClearColor(0.1,0.1,0.3,1.0);
    glClearColor(0.80,0.80,0.80,1.0);
}

void MontaniaRecursiva(GLfloat x1, GLfloat x2, GLfloat z1, GLfloat z2, int itLvl) {
    int fila1 = floor(x1*tam_arre + 0.5)*(tam_arre + 1);
    int fila2 = floor(x2*tam_arre + 0.5)*(tam_arre + 1);
    int col1 = floor(z1*tam_arre + 0.5);
    int col2 = floor(z2*tam_arre + 0.5);
    if (itLvl > division_malla) return;
    int ultimacol = pow(2, division_malla - itLvl);
    int ultimafila = (tam_arre + 1)*ultimacol;
    GLfloat mid1, mid2, mid3, mid4;
    GLfloat midx = (x1 + x2) / 2.0;
    GLfloat midz = (z1 + z2) / 2.0;
    GLfloat rug = rugosidad / pow(2, itLvl - 1);
    if (yVals[fila1 + col1 + ultimacol] == VALOR_INVALIDO){
        yVals[fila1 + col1 + ultimacol] = mid1 = yAleatorio(yVals[fila1 + col1], yVals[fila1 + col2], rug);}
    else {mid1 = yVals[fila1 + col1 + ultimacol];}
    
    if (yVals[fila1 + ultimafila + col1] == VALOR_INVALIDO){
        yVals[fila1 + ultimafila + col1] = mid2 = yAleatorio(yVals[fila1 + col1], yVals[fila2 + col1], rug);}
    else {mid2 = yVals[fila1 + ultimafila + col1];}
    
    if (yVals[fila2 + col1 + ultimacol] == VALOR_INVALIDO){
        yVals[fila2 + col1 + ultimacol] = mid3 = yAleatorio(yVals[fila2 + col1], yVals[fila2 + col2], rug);}
    else {mid3 = yVals[fila2 + col1 + ultimacol];}
    
    if (yVals[fila1 + ultimafila + col2] == VALOR_INVALIDO){
        yVals[fila1 + ultimafila + col2] = mid4 = yAleatorio(yVals[fila1 + col2], yVals[fila2 + col2], rug);}
    else {mid4 = yVals[fila1 + ultimafila + col2];}
    
    mid1 = (mid1 + mid2 + mid3 + mid4) / 4.0;
    yVals[fila1 + ultimafila + col1 + ultimacol] = yAleatorio(mid1, mid1, rug);
    itLvl++;
    MontaniaRecursiva(x1, midx, z1, midz, itLvl);
    MontaniaRecursiva(midx, x2, z1, midz, itLvl);
    MontaniaRecursiva(x1, midx, midz, z2, itLvl);
    MontaniaRecursiva(midx, x2, midz, z2, itLvl);
}

void dibujaMontania() {
    GLfloat x1, x2, z, norm[3], vertInc = 1.0 / tam_arre;
    GLfloat lastx1Vert[3], lastx2Vert[3], newx1Vert[3], newx2Vert[3];
    glTranslated(-0.5, 0, -0.5);
    glPushMatrix();
    GLfloat color1[] = {0.282,0.380,0.074};
    GLfloat color2[] = {0.423,0.568,0.113};
    GLfloat color3[] = {0.752,0.894,0.454};
    GLfloat color4[] = {0.835,0.929,0.635};
    GLfloat colorAgua[] = { 0.0, 0.5, 1.0, 1.0 };
    GLfloat colorNieve[] = { 1.0, 1.0, 1.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color1);
    glColor3f(1.0, 0.0, 0.0);
    buscar_y_superior();
    highestY = 0.6*highestY;
    for (int h = 0; h < arreTotal; h++){ if (yVals[h] < 0) yVals[h] = 0;}
    for (int i = 0; i < tam_arre; i++){
        x1 = i*vertInc;
        x2 = (i + 1)*vertInc;
        z = 0.0;
        
        glBegin(GL_QUAD_STRIP);
        //glBegin(GL_POLYGON);
        //glBegin(GL_POLYGON_BIT);
        //glBegin(GL_QUADS);
        //glBegin(GL_TRIANGLES);
        
        lastx1Vert[0] = x1; lastx1Vert[1] = yVals[i*(tam_arre + 1)]; lastx1Vert[2] = z;
        lastx2Vert[0] = x2; lastx2Vert[1] = yVals[(i + 1)*(tam_arre + 1)]; lastx2Vert[2] = z;
        for (int j = 0; j <= tam_arre; j++) {
            z += vertInc;
            newx1Vert[0] = x1; newx1Vert[1] = yVals[i*(tam_arre + 1) + j]; newx1Vert[2] = z;
            newx2Vert[0] = x2; newx2Vert[1] = yVals[(i + 1)*(tam_arre + 1) + j]; newx2Vert[2] = z;
            if (lastx1Vert[1] <= 0){
                glMaterialfv(GL_FRONT, GL_DIFFUSE, colorAgua);
                glColor3f(colorAgua[0], colorAgua[1], colorAgua[2]);
            }
            else if (lastx1Vert[1] >= highestY){
                glMaterialfv(GL_FRONT, GL_DIFFUSE, colorNieve);
                glColor3f(colorNieve[0], colorNieve[1], colorNieve[2]);
            }
            else if (lastx1Vert[1] >= highestY/1.2){
                glMaterialfv(GL_FRONT, GL_DIFFUSE, color4);
                glColor3f(color4[0], color4[1], color4[2]);
            }
            else if (lastx1Vert[1] >= highestY/3){
                glMaterialfv(GL_FRONT, GL_DIFFUSE, color3);
                glColor3f(color3[0], color3[1], color3[2]);
            }
            else if (lastx1Vert[1] >= highestY/5){
                glMaterialfv(GL_FRONT, GL_DIFFUSE, color2);
                glColor3f(color2[0], color2[1], color2[2]);
            }
            else {
                glMaterialfv(GL_FRONT, GL_DIFFUSE, color1);
                glColor3f(color1[0], color1[1], color1[2]);
            }
            formaNormal(lastx1Vert, newx1Vert, lastx2Vert, norm);
            glNormal3fv(norm);
            glVertex3fv(lastx1Vert);
            
            if (lastx2Vert[1] <= 0){
                glMaterialfv(GL_FRONT, GL_DIFFUSE, colorAgua);
                glColor3f(colorAgua[0], colorAgua[1], colorAgua[2]);
            }
            else if (lastx2Vert[1] >= highestY){
                glMaterialfv(GL_FRONT, GL_DIFFUSE, colorNieve);
                glColor3f(colorNieve[0], colorNieve[1], colorNieve[2]);
            }
            else if (lastx2Vert[1] >= highestY/1.2){
                glMaterialfv(GL_FRONT, GL_DIFFUSE, color4);
                glColor3f(color4[0], color4[1], color4[2]);
            }
            else if (lastx2Vert[1] >= highestY/3){
                glMaterialfv(GL_FRONT, GL_DIFFUSE, color3);
                glColor3f(color3[0], color3[1], color3[2]);
            }
            else if (lastx2Vert[1] >= highestY/5){
                glMaterialfv(GL_FRONT, GL_DIFFUSE, color2);
                glColor3f(color2[0], color2[1], color2[2]);
            }
            else {
                glMaterialfv(GL_FRONT, GL_DIFFUSE, color1);
                glColor3f(color1[0], color1[1], color1[2]);
            }
             
            formaNormal(lastx2Vert, lastx1Vert, newx2Vert, norm);
            glNormal3fv(norm);
            glVertex3fv(lastx2Vert);
            lastx1Vert[0] = newx1Vert[0]; lastx1Vert[1] = newx1Vert[1]; lastx1Vert[2] = newx1Vert[2];
            lastx2Vert[0] = newx2Vert[0]; lastx2Vert[1] = newx2Vert[1]; lastx2Vert[2] = newx2Vert[2];
        }
        glNormal3fv(norm);
        glVertex3fv(lastx1Vert);
        glNormal3fv(norm);
        glVertex3fv(lastx2Vert);
        glEnd();
    }
    glPopMatrix();
}

void hacerMontania() {
    tam_arre = pow(2, division_malla);
    arreTotal = (tam_arre + 1)*(tam_arre + 1);
    yVals = (GLfloat *)malloc(arreTotal*sizeof(GLfloat));
    for (int i = 0; i < arreTotal; i++){ yVals[i] = VALOR_INVALIDO;}
    yVals[0] = yAleatorio(0, 0, rugosidad);
    yVals[tam_arre] = yAleatorio(0, 0, rugosidad);
    yVals[(tam_arre + 1)*tam_arre] = yAleatorio(0, 0, rugosidad);
    yVals[(tam_arre + 1)*tam_arre + tam_arre] = yAleatorio(0, 0, rugosidad);
    MontaniaRecursiva(0, 1.0, 0, 1.0, 1);
}

void estableceVista(){
    glLoadIdentity();
    //gluLookAt(0.0, 1.5, 5.0, -0.5, 0.0, -0.5, 0.0, 1.0, 0.0);
    gluLookAt(0.0,0.0,5.0, -1.25, 1.0, 0.0, 0.0, 1.0, 0.0);
    glTranslatef(-1.25f, 1.0f, -12.0f);
}

void estableceLuz() {
    GLfloat light_ambient[]={0.0, 0.0, 0.0, 1.0};
    GLfloat light_diffuse[]={1.0, 1.0, 1.0, 1.0};
    GLfloat light_specular[]={1.0, 1.0, 1.0, 1.0};
    GLfloat light_position[]={luzX,luzY,luzZ, 0.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    return;
}

void estableceModelo() {
    glTranslatef(modelTrans[0], modelTrans[1], modelTrans[2]);
    glScalef(al, bl, cl);
    glRotatef(angulo[0], 1.0, 0.0, 0.0);
    glRotatef(angulo[1], 0.0, 1.0, 0.0);
    glRotatef(angulo[2], 0.0, 0.0, 1.0);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40, 1, 0.1, 100);
    glMatrixMode(GL_MODELVIEW);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    estableceVista();
    estableceLuz();
    estableceModelo();
    dibujaMontania();
    glutSwapBuffers();
}

void buscar_y_superior() {
    highestY = 0;
    for (int i = 0; i < arreTotal; i++) {
        if (yVals[i] > highestY) highestY = yVals[i];
    }
}

GLfloat yAleatorio(GLfloat y1, GLfloat y2, GLfloat r) {
    return ((y1 + y2) / 2.0) + ((((float)(rand()) / RAND_MAX * 2) - 1.0)*r);
}


void TeclasEscucha(unsigned char key, int x, int y){
    switch (key){
        case 'o'://mover en z
            modelTrans[2] -= mov;
            display();
            break;
        case 'p'://mover en z
            modelTrans[2] += mov;
            display();
            break;
        case 'r'://reset
            angulo[0] = 30.0;
            angulo[1] = 0.0;
            angulo[2] = 0.0;
            luzX = 0.0;
            luzY = 5.0;
            luzZ = 5.0;
            al = 3.5;
            bl = 3.5;
            cl = 3.5;
            display();
            break;
        case '+'://escala
            al += 0.1;
            bl += 0.1;
            cl += 0.1;
            //hacerMontania();
            display();
            break;
        case '-'://escala
            al -= 0.1;
            bl -= 0.1;
            cl -= 0.1;
            //hacerMontania();
            display();
            break;
        case 'X':
            if(luzX<luz){
                luzX+=1;
            }
            display();
            break;
        case 'x':
            if(luzX>-luz){
                luzX-=1;
            }
            display();
            break;
        case 'Y':
            if(luzY<luz){
                luzY+=1;
            }
            display();
            break;
        case 'y':
            if(luzY>-luz){
                luzY-=1;
            }
            display();
            break;
        case 'Z':
            if(luzZ<luz){
                luzZ+=1;
            }
            display();
            break;
        case 'z':
            if(luzZ>-luz){
                luzZ-=1;
            }
            display();
            break;
        case '1'://rotar en x-
            angulo[0] -= incThetha;
            //if (angulo[0] > 360.0) angulo[0] -= 360.0;
            display();
            break;
            
        case '2'://rotar en y-
            angulo[1] -= incThetha;
            //if (angulo[1] > 360.0) angulo[1] -= 360.0;
            display();
            break;
            
        case '3'://rotar en z-
            angulo[2] -= incThetha;
            //if (angulo[2] > 360.0) angulo[2] -= 360.0;
            display();
            break;
        case 27:
            exit(0);//salir del programa con la tecla esc
            break;
        default:
            display();
    }
}

void ArrowKey(int key,int x,int y){
    switch(key){
        case GLUT_KEY_RIGHT://mover a la derecha
            modelTrans[0] += mov;
            display();
            break;
        case GLUT_KEY_LEFT://mover a la izquierda
            modelTrans[0] -= mov;
            display();
            break;
        case GLUT_KEY_UP://mover arriba
            modelTrans[1] += mov;
            display();
            break;
            
        case GLUT_KEY_DOWN://mover abajo
            modelTrans[1] -= mov;
            display();
            break;
            
        case GLUT_KEY_F1://rotar en x
            angulo[0] += incThetha;
            display();
            break;
            
        case GLUT_KEY_F2://rotar en y
            angulo[1] += incThetha;
            display();
            break;
            
        case GLUT_KEY_F3://rotar en z
            angulo[2] += incThetha;
            display();
            break;
            
        case GLUT_KEY_F4://new montaña
            if (rugosidad < 0.25)
                rugosidad = 1.0;
            rugosidad = rugosidad * 0.5;
            hacerMontania();
            display();
            //printf("ss %f",rugosidad);
            break;
            
        case GLUT_KEY_F5://disminuir iteraciones (montaña menos rugosa)
            division_malla = division_malla - 1;
            hacerMontania();
            display();
            break;
        case GLUT_KEY_F6://aumentar iteraciones (montaña mas rugosa)
            division_malla = division_malla + 1;
            hacerMontania();
            display();
            break;
            /*case GLUT_KEY_F7://elevacion
             if (rugosidad < 0.25)
             rugosidad = 1.0;
             rugosidad = rugosidad * 2;
             hacerMontania();
             display();
             break;*/
        default:
            display();
            break;
            
    }
    
}

void normalizacion(float v[3]) {
    GLfloat d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (d == 0.0) {
        printf("vector 0 length");
        return;
    }
    v[0] = v[0] / d; v[1] = v[1] / d; v[2] = v[2] / d;
}

void productoNormalCruz(float v1[3], float v2[3], float out[3]) {
    out[0] = v1[1] * v2[2] - v1[2] * v2[1];
    out[1] = v1[2] * v2[0] - v1[0] * v2[2];
    out[2] = v1[0] * v2[1] - v1[1] * v2[0];
    normalizacion(out);
}

void formaNormal(GLfloat v[3], GLfloat v1[3], GLfloat v2[3], GLfloat out[3]) {
    if (v[1] == 0) {
        out[0] = 0; out[1] = 1.0; out[2] = 0;
        return;
    }
    GLfloat vect1[3] = { v1[0] - v[0], v1[1] - v[1], v1[2] - v[2] };
    GLfloat vect2[3] = { v2[0] - v[0], v2[1] - v[1], v2[2] - v[2] };
    productoNormalCruz(vect1, vect2, out);
}


