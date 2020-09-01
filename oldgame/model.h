#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GL/glut.h>
#include <iostream>
using namespace std;

class Obj
{
   
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
    }

    Obj(char *model_path, char *texture_path)
    {
        model = new Model(model_path);
        load_texture(texture_path);
    }

    ~Obj()
    {
        delete model;
    }

    void display()
    {
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
        glDisable(GL_TEXTURE_2D);
    }
};