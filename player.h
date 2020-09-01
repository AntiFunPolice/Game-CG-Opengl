#include "model.h"

class Horse
{
public:
    Obj *obj;
    float xx = 0 ,yy = 0, zz = 0;
    int dir = 1;
    float rot = 1 , inc = 0;
    bool walking = false;
    float min_x,min_y,max_x,max_y;

    Horse()
    {
        obj = new Obj("horse");
    }

    ~Horse()
    {
        delete obj;
    }

   

    void display()
    {
        
        glPushMatrix();
        glTranslatef(xx,yy,zz);
        glRotatef(90*rot,0,-1,0);
        glRotatef(inc,1,0,0);
        obj->display();
        glPopMatrix();
    }
};