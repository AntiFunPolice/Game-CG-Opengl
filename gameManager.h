#include "model.h"

class GameManager
{
public:
    Obj *skybox;

    Obj *obj;
    int lvl = 1;
    float translations[3][20];   // 3 levels  up to 20 translactions for the box
    bool walking = false;
 

    GameManager(){
        Obj = new Obj("chao");
        //Obj = new Obj("skybox");
    }

    ~GameManager(){
        
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
   

    void display(){

        switch (lvl)
        {
            case 1:
                
                break;
            case 2:
                
                break;
            case 3:
                
                break;
           
        }   
    }
};