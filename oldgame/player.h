#include "model.h"

class Horse
{
public:
    Obj *obj;
    float xx = 0 ,yy = 0, zz = 0;
    int dir = 1;
    float rot = 0, rot_increment = 5;
    float inc = 0, inc_increment = 1;
    bool walking = false;

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
        obj->display();
        glPopMatrix();
    }
};