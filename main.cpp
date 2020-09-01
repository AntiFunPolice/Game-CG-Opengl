
#include <iostream>
#include <GL/glew.h>
#include <GL/glext.h>
#include <GL/glut.h>
#include <cstdio>

#include "player.h"
using namespace std;

int min_width = 1000, min_height = 1000;
float zoom = 1, zoom_increment = 0.1;
Horse *horse;
Obj *objects[30];
int n_objects = 0;
int active_camera = 3;
float gforce = 0.00;


bool _w = false;                                                                            //  <<-global->> bool for whether the w key is down
bool _a = false;                                                                            //  <<-global->> bool for whether the a key is down
bool _s = false;                                                                            //  <<-global->> bool for whether the s key is down
bool _d = false;                                                                            //  <<-global->> bool for whether the d key is down
bool _q = false;                                                                            //  <<-global->> bool for whether the q key is down
bool _e = false;                                                                            //  <<-global->> bool for whether the e key is down
bool _space = false;                                                                        //  <<-global->> bool for whether the e key is down
bool airup = false;
bool airdown = false;
bool colliding = false;


void create_objects()
{
    horse = new Horse();
    //horse_obj = new Obj("horse");
    //objects[n_objects++] = new Obj("floor");
    //objects[n_objects++] = new Obj("walls");
    //objects[n_objects++] = new Obj("roof");
    //objects[n_objects++] = new Obj("door");
    objects[n_objects++] = new Obj("chao");
    
}

void init()
{

    glClearColor(0.2, 0.2, 0.2, 0.0);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, 1, 0.5, 50);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    create_objects();


}

void set_camera(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ, GLdouble centerX, GLdouble centerY, GLdouble centerZ, GLdouble upX, GLdouble upY, GLdouble upZ)
{
    eyeX = (eyeX - centerX) / zoom + centerX;
    eyeY = (eyeY - centerY) / zoom + centerY;
    eyeZ = (eyeZ - centerZ) / zoom + centerZ;
    gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
}

void camera()
{
    switch (active_camera)
    {
        case 1:
            set_camera(4, 4, 4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
            break;
        case 2:
            set_camera(-4, 3, -2, -6.0, 1.7, 0, 0.0, 1.0, 0.0);
            break;
        case 3:
            set_camera(5, 3, 20, 5, 0.0, 5.0, 0.0, 1.0, 0.0);
            break;
        case 4:
            set_camera(3, 2, -3, 0.0, 0.3, 0.0, 0.0, 1.0, 0.0);
            break;
        case 5:
            set_camera(1.1, 1.9, -0.6, -6.0, 1.7, 0, 0.0, 1.0, 0.0);
            break;
    }
}

void camerachaser(){
    set_camera(horse->xx , 3+ horse->yy, 20, horse->xx, 0.0, 5.0, 0.0, 1.0, 0.0);
}

void movement(){
    float speedfactor = 0;
    
    if(airup || airdown) { 
        if(airup)
            horse->inc = 10;
        else
            horse->inc = -5;

        speedfactor = 0.03;
        
    }else{
        speedfactor=0;
        horse->inc=0;
    }

    if(_w||_a||_s||_d){
       // if(_w) 
       //     horse->zz = horse->zz-0.07;
        if(_a){
            horse->xx = horse->xx-0.09-speedfactor;
            horse->rot =-1;
        }
       // if(_s)
       //     horse->zz = horse->zz+0.07;
        if(_d){
            horse->xx = horse->xx+0.09+speedfactor;
            horse->rot =1;
        }
    }


    //if(_w && _a){horse->zz = horse->zz+0.01 ; horse->xx = horse->xx+0.01;}
    //if(_w && _d){horse->zz = horse->zz+0.01; horse->xx = horse->xx-0.01;}
    //if(_s && _a){horse->zz = horse->zz-0.01; horse->xx = horse->xx+0.01;}
    //if(_s && _d){horse->zz = horse->zz-0.01; horse->xx = horse->xx-0.01;}

    if( gforce < 0.05 && airup){
        airdown=true;
        airup=false;
    }

    if( horse->yy <= 0.0){
        horse->yy = 0.0;
        airdown=false;
    }


    if( horse->yy <= 0.0 && !airup){
        gforce=0;
    }

    if(airup && gforce != 0 && !airdown){
        
        horse->yy = horse->yy+gforce;
        gforce = gforce- gforce/10;


    }

    if(airdown && horse->yy > 0.0 && !airup){
        horse->yy = horse->yy-gforce;
        if(gforce<0.5)
            gforce = gforce +(9*gforce/10)/8;
    }


    if(_space){
        if(airup==false && airdown== false){
            airup=true;
            gforce = 0.5;
        }
    }
    float x = horse->yy;
    float force = gforce;
   // printf("airdown: %d  airup: %d y: %f gforce: %f\n",airdown,airup, x,force );
}

void display()
{
// Limpa os "buffers"
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// Indica que as próximas operações de matrizes serão feitas sobre
// a matriz "ModelView"
    glMatrixMode(GL_MODELVIEW);
// Limpa a matriz "ModelView" (transforma-a na matriz identidade)
    glLoadIdentity();
// ---
// Os objetos serão desenhados e animados aqui
// ---
    //camera();
    camerachaser();
    movement();
    glColor3f(1.0f, 1.0f, 1.0f);

    
    horse->display();
    
    glPushMatrix();
     glTranslatef(0,-1,0);
    for (int i = 0; i < n_objects; i++)
        objects[i]->display();

    glPopMatrix();
     glColor3f(1.0,0.0,0.0); // red x
    glBegin(GL_LINES);
    // x aix
 
    glVertex3f(-4.0, 0.0f, 0.0f);
    glVertex3f(4.0, 0.0f, 0.0f);
 
    // arrow
    glVertex3f(4.0, 0.0f, 0.0f);
    glVertex3f(3.0, 1.0f, 0.0f);
 
    glVertex3f(4.0, 0.0f, 0.0f);
    glVertex3f(3.0, -1.0f, 0.0f);
    glEnd();
    glFlush();
 
 
 
    // y 
    glColor3f(0.0,1.0,0.0); // green y
    glBegin(GL_LINES);
    glVertex3f(0.0, -4.0f, 0.0f);
    glVertex3f(0.0, 4.0f, 0.0f);
 
    // arrow
    glVertex3f(0.0, 4.0f, 0.0f);
    glVertex3f(1.0, 3.0f, 0.0f);
 
    glVertex3f(0.0, 4.0f, 0.0f);
    glVertex3f(-1.0, 3.0f, 0.0f);
    glEnd();
    glFlush();
 
    // z 
    glColor3f(0.0,0.0,1.0); // blue z
    glBegin(GL_LINES);
    glVertex3f(0.0, 0.0f ,-4.0f );
    glVertex3f(0.0, 0.0f ,4.0f );
 
    // arrow
    glVertex3f(0.0, 0.0f ,4.0f );
    glVertex3f(0.0, 1.0f ,3.0f );
 
    glVertex3f(0.0, 0.0f ,4.0f );
    glVertex3f(0.0, -1.0f ,3.0f );
    glEnd();
    glFlush();

// Troca os dois "buffers" de "display", e imprime na janela o
// atualizado. Substitui o "glFlush", quando em modo "GLUT_DOUBLE".
    glutSwapBuffers();
}

void reshape(int w, int h)
{
// Define o "Viewport" como o tamanho da janela
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}




void call_display(int)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / 120, call_display, 0);
}

//<-------------------------------------------------------------------------------------------------------------------------------------------------------------->
//<-------------------------------------------------------keyboard function: called each key press--------------------------------------------------------------->
//<-------------------------------------------------------------------------------------------------------------------------------------------------------------->

void keyboard(unsigned char key, int x, int y){
    if(key == 0x1B)(*(int*)0) = 69; //if esc is pressed crash program
    if(key == 0x1B){
        glutDestroyWindow ( glutGetWindow() );
    }
    switch(key){
        case 'w': _w = true; break;
        case 'a': _a = true; break;
        case 's': _s = true; break;
        case 'd': _d = true; break;
        case 'q': _q = true; break;
        case 'e': _e = true; break;
        case ' ': _space = true; break;

    }
}


//<-------------------------------------------------------------------------------------------------------------------------------------------------------------->
//<------------------------------------------------------keyboard function: called each key release-------------------------------------------------------------->
//<-------------------------------------------------------------------------------------------------------------------------------------------------------------->

void keyboardup(unsigned char key, int x, int y){
    switch(key){
        case 'w': _w = false; break;
        case 'a': _a = false; break;
        case 's': _s = false; break;
        case 'd': _d = false; break;
        case 'q': _q = false; break;
        case 'e': _e = false; break;
        case ' ': _space = false; break;
    }
}


void keyboard_special(int key, int, int)
{
    switch (key)
    {
    case GLUT_KEY_F1:
        active_camera = 1;
        break;
    case GLUT_KEY_F2:
        active_camera = 2;
        break;
    case GLUT_KEY_F3:
        active_camera = 3;
        break;
    case GLUT_KEY_F4:
        active_camera = 4;
        break;
    case GLUT_KEY_F5:
        active_camera = 5;
        break;
    case GLUT_KEY_F6:
        active_camera = 5;
        break;
    
    }
}

int main(int argc, char **argv)
{
// Inicializa o GLUT
    glutInit(&argc, argv);
// Indica que o "display" utilizará duplo "buffer", o esquema de
// cores "RBG" ("Red, Green, Blue") e um "buffer" de profundidade
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
// Indica a posição da janela
    glutInitWindowPosition(0, 0);
// Indica o tamanho da janela
    glutInitWindowSize(min_width, min_height);
// Cria a janela, com o título dado
    glutCreateWindow("The Project");
// Define mais algumas opções
    init();
// Define a função de "display", que é chamada no início e sempre
// que a função "glutPostRedisplay é chamada
    glutDisplayFunc(display);
// Define a função de "reshape", que é chamada sempre que o tamanho
// da janela muda
    glutReshapeFunc(reshape);
    call_display(0);

    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardup);
    glutSpecialFunc(keyboard_special);
// Inicia o ciclo de vida do GLUT
    glutMainLoop();
    return 1;
}