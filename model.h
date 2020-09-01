#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GL/glut.h>
#include <iostream>
using namespace std;

class Obj
{
float min_x,min_y,min_z,max_x,max_y,max_z;

private:
    class Model
    {
    public:
        // Vertex and texture vertex positions
        float v[12000][3], vt[12000][2];
        int n_v = 0, n_vt = 0;

        // Normals for each vertex
        float vn[12000][3];
        int n_vn = 0;

        // Vertex indices for each face
        int v_i[20000][3], vt_i[20000][3], vn_i[20000][3];
        int n_f = 0;

        Model(char *path)
        {
            parse(path);
        }

        void parse(char *path)
        {
            FILE *file = fopen(path, "r");
            int line = 0;
            char lineHeader[1000];
            while (fscanf(file, "%s", lineHeader) != EOF)
            {
                if (strcmp(lineHeader, "v") == 0)
                {
                    fscanf(file, "%f %f %f\n", &v[n_v][0], &v[n_v][1], &v[n_v][2]);
                    n_v++;
                }
                else if (strcmp(lineHeader, "vt") == 0)
                {
                    fscanf(file, "%f %f\n", &vt[n_vt][0], &vt[n_vt][1]);
                    n_vt++;
                }
                else if (strcmp(lineHeader, "vn") == 0)
                {
                    fscanf(file, "%f %f %f\n", &vn[n_vn][0], &vn[n_vn][1], &vn[n_vn][2]);
                    n_vn++;
                }
                else if (strcmp(lineHeader, "f") == 0)
                {
                    int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &v_i[n_f][0], &vt_i[n_f][0], &vn_i[n_f][0], &v_i[n_f][1], &vt_i[n_f][1], &vn_i[n_f][1], &v_i[n_f][2], &vt_i[n_f][2], &vn_i[n_f][2]);
                    if (matches != 9)
                    {
                        cout << "Model file " << path << " can't be read by this parser!" << endl;
                        cout << "Error at line: " << line << endl;
                        fclose(file);
                        exit(1);
                    }
                    n_f++;
                }
                line++;
            }
            fclose(file);
        }
    };

    

    void load_texture(char *path)
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        int width, height, nrChannels;
        unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            // glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            cout << "Failed to load texture: " << path << endl;
        }
        stbi_image_free(data);
    }

public:
    

    Model *model;
    
    unsigned int texture;

    Obj(char *name)
    {
        char model_path[50] = "assets/";
       
        char texture_path[50] = "assets/";

        strcat(model_path, name);
        
        strcat(texture_path, name);

        strcat(model_path, ".obj");
        
        strcat(texture_path, ".jpg");

        model = new Model(model_path);
        
        load_texture(texture_path);

        min_max_find();
    }

     Obj(char *model_path, char *texture_path)
    {
        
        model = new Model(model_path);
        load_texture(texture_path);
        min_max_find();
    }

    ~Obj()
    {
        delete model;
    }

    void min_max_find(){
       
        min_z = model->v[model->v_i[0][0] - 1][2];
        max_z = model->v[model->v_i[0][0] - 1][2];

        min_x = model->v[model->v_i[0][0] - 1][0];
        max_x = model->v[model->v_i[0][0] - 1][0];

        min_y = model->v[model->v_i[0][0] - 1][1];
        max_y = model->v[model->v_i[0][0] - 1][1];

        for (int i = 0; i < model->n_f; i++)
        {
            for (int j = 0; j < 3; j++)
            {   
                if(min_z > model->v[model->v_i[i][j] - 1][2])
                    min_z = model->v[model->v_i[i][j] - 1][2];

                if(max_z < model->v[model->v_i[i][j] - 1][2])
                    max_z = model->v[model->v_i[i][j] - 1][2];

                if(min_y > model->v[model->v_i[i][j] - 1][1])
                    min_y = model->v[model->v_i[i][j] - 1][1];

                if(max_y < model->v[model->v_i[i][j] - 1][1])
                    max_y = model->v[model->v_i[i][j] - 1][1];

                if(min_x > model->v[model->v_i[i][j] - 1][0])
                    min_x = model->v[model->v_i[i][j] - 1][0];

                if(max_x < model->v[model->v_i[i][j] - 1][0])
                    max_x = model->v[model->v_i[i][j] - 1][0];
            }
        }

        
    }

    float get_min_x(){
        return min_x;
    }
    float get_min_y(){
        return min_y;
    }
     float get_min_z(){
        return min_z;
    }

    float get_max_z(){
        return max_z;
    }

    float get_max_x(){
        return max_x;
    }

    float get_max_y(){
        return max_y;
    }




   

    void display()
    {   

        //min_max_find();
        
        //printf(" min_x: %f max_x:%f min_y: %f max_y: %f\n", min_x,max_x,min_y,max_y);


        glBindTexture(GL_TEXTURE_2D, texture);
        glEnable(GL_TEXTURE_2D);
        glBegin(GL_TRIANGLES);
       




        for (int i = 0; i < model->n_f; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                
                glTexCoord2f(model->vt[model->vt_i[i][j] - 1][0], -model->vt[model->vt_i[i][j] - 1][1]);
                glNormal3f((model->vn[model->vn_i[i][j] - 1][0]), (model->vn[model->vn_i[i][j] - 1][1]), (model->vn[model->vn_i[i][j] - 1][2]));
                glVertex3f((model->v[model->v_i[i][j] - 1][0]), (model->v[model->v_i[i][j] - 1][1]), (model->v[model->v_i[i][j] - 1][2]));
            }
        }
        
        glEnd();

       //printf("no modelo min_x: %f max_x:%f min_y: %f max_y: %f\n", min_x,max_x,min_y,max_y);

        glDisable(GL_TEXTURE_2D);

        //glBegin(GL_QUADS);
        //glVertex2f(min_x,min_y);
        //glVertex2f(max_x,min_y);
        //glVertex2f(max_x,max_y);
        //glVertex2f(min_x,max_y);
        //glEnd();
    }
};