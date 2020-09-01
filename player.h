#include "model.h"

class Horse
{
public:
    Obj *obj;
    float xx = 0 ,yy = 0, zz = 0;
     float min_x,min_y,max_x,max_y;
    int dir = 1;
    float rot = 1 , inc = 0;
    bool walking = false;
   

    Horse()
    {
        obj = new Obj("horse");
        min_x = obj->get_min_z();
        min_y = obj->get_min_y();
        max_x = obj->get_max_z();
        max_y = obj->get_max_y();
    }

    ~Horse()
    {
        delete obj;
    }


     float get_min_x(){
        return min_x;
    }
    float get_min_y(){
        return min_y;
    }

    float get_max_x(){
        return max_x;
    }

    float get_max_y(){
        return max_y;
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