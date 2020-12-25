/*
****************************************************
20181024 ������
���� 3���� ������ ���̴� ��ǻ�� �׷��Ƚ�
*****************************************************
*/
#include <stdio.h>
#include <GL\glaux.h>
#include <GL\glut.h>
#include <GL\glu.h>
#include <fstream>
using namespace std;
#include <stdlib.h>
#include <math.h>
#include "tga.h" // tga ������ �ε��ϱ� ���� ������� �ʿ� 
//winddir �� ���� windspeed ��ӵ�
int screennum = 0, winddir = 45;
GLfloat windspeed = 0.0005f;
GLint b = 300;
float  counter = 7.5f;//�ڵ��� �̵� ����
int Width, Height;
//enviroment mapping����
GLuint     filter, texture_enviroment[6], object = 1;
GLUquadricObj* quadratic;
//�ؽ�ó ����
#define TEXTURE_NUM 9 
GLuint g_textureID = -1;
GLuint texture[TEXTURE_NUM];
char texture_name[TEXTURE_NUM][20] = { "road.bmp","road.bmp","street.bmp","road.bmp","sky.bmp","signal.bmp","red.bmp","car.bmp","speed.bmp" };

//����
float a = 1.0f;
float j = 0.0f;
float c = 5.0f;
double rad;

//�ִϸ��̼� ���� ����
static bool isanimation = false;
void LoadEnviromentTextures();

struct Vertex {
    float tu, tv;
    float x, y, z;
};
Vertex g_cubeVertices[] = {
    { 0.0f,0.0f, -2.0f,-0.5f, 0.5f },
    { 1.0f,0.0f,  -1.5f,-0.5f, 0.5f },
    { 1.0f,1.0f,  -1.5f, 0.0f, 0.5f },
    { 0.0f,1.0f, -2.0f, 0.0f, 0.5f },

    { 1.0f,0.0f, -2.0f,-0.5f,-0.5f },
    { 1.0f,1.0f, -2.0f, 0.0f,-0.5f },
    { 0.0f,1.0f,  -1.5f, 0.0f,-0.5f },
    { 0.0f,0.0f,  -1.5f,-0.5f,-0.5f },

    { 0.0f,1.0f, -2.0f, 0.0f,-0.5f },
    { 0.0f,0.0f, -2.0f, 0.0f, 0.5f },
    { 1.0f,0.0f,  -1.5f, 0.0f, 0.5f },
    { 1.0f,1.0f,  -1.5f, 0.0f,-0.5f },

    { 1.0f,1.0f, -2.0f,-0.5f,-0.5f },
    { 0.0f,1.0f,  -1.5f,-0.5f,-0.5f },
    { 0.0f,0.0f,  -1.5f,-0.5f, 0.5f },
    { 1.0f,0.0f, -2.0f,-0.5f, 0.5f },

    { 1.0f,0.0f,  -1.5f,-0.5f,-0.5f },
    { 1.0f,1.0f,  -1.5f, 0.0f,-0.5f },
    { 0.0f,1.0f,  -1.5f, 0.0f, 0.5f },
    { 0.0f,0.0f,  -1.5f,-0.5f, 0.5f },

    { 0.0f,0.0f, -2.0f,-0.5f,-0.5f },
    { 1.0f,0.0f, -2.0f,-0.5f, 0.5f },
    { 1.0f,1.0f, -2.0f, 0.0f, 0.5f },
    { 0.0f,1.0f, -2.0f, 0.0f,-0.5f }
};
//��ƼŬ ����ü ���� 
struct particle {
    GLfloat x, y, z;
    GLfloat r, g, b;
    GLfloat xd, yd, zd;
    GLfloat cs;
} p[1000];
void MyReshape(int w, int h) {
    Width = w;   Height = h;
}
void Init() {
    //GLfloat fogColor[] = { 0.6f, 0.6f, 0.6f, 1.0f }; 
    LoadEnviromentTextures();
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    glOrtho(-5.0, 5.0, -5.0, 5.0, 0.5, 5.0);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    //���� ����
    GLfloat Light0_Ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat Light0_Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat Light0_Position[] = { 0.0f, 0.0f, 2.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, Light0_Ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Light0_Diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, Light0_Position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    quadratic = gluNewQuadric();
    gluQuadricNormals(quadratic, GLU_SMOOTH);
    gluQuadricTexture(quadratic, GL_TRUE);

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

}


void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glTranslatef(-1.0f, 0.0f, -8.0f);
    glViewport(0, 0, Width, Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (GLfloat)Width / (GLfloat)Height, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    //���� ����
    gluLookAt(a, j, c, a, j, 0.0, 0.0, 1.0, 0.0);

    //glPushMatrix();
    //glColor4ub(255, 255, 255, 255);
    //glNormal3f(0.0f, 1.0f, 0.0f);



    //glLoadIdentity();
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glBindTexture(GL_TEXTURE_2D, texture_enviroment[filter + (filter + 1)]);
    glPushMatrix();
    glRotatef(30, 1.0f, 0.0f, 0.0f);
    glRotatef(30, 0.0f, 1.0f, 0.0f);
    glTranslatef(3.2f, 1.0f, 2.0f);
    gluSphere(quadratic, 0.4, 32, 32);
    glPopMatrix();


    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);

    glBindTexture(GL_TEXTURE_2D, texture_enviroment[filter * 2]);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -24.0f);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(7.5f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, -1.0f, -1.0f);	// �޸�
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-3.0f, 2.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(7.5f, 2.0f, -1.0f);
    glEnd();
    //glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //glDisable(GL_BLEND);

    
    glEnable(GL_TEXTURE_2D);
    //��� �����
    //glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_enviroment[filter * 2]);

    glBegin(GL_QUADS);//


    glTexCoord2f(1.0f, 0.0f); glVertex3f(7.5f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, -1.0f, -1.0f);	// �޸�
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-3.0f, 2.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(7.5f, 2.0f, -1.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[1]);

    glBegin(GL_QUADS);//
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-3.0f, -1.0f, -1.0f);	// ������
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, -1.0f, 2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-3.0f, 2.0f, 2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-3.0f, 2.0f, -1.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glBegin(GL_QUADS);//�߰�*/
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, -1.0f, -1.0f);		// �Ʒ���
    glTexCoord2f(0.0f, 1.0f); glVertex3f(7.5f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(7.5f, -1.0f, 2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-3.0f, -1.0f, 2.0f);

    glEnd();
    //�� �����

    //�� ����
    glBindTexture(GL_TEXTURE_2D, texture[7]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.1f, 0.351f); glVertex3f(counter, -0.8f, 0.5f);
    glTexCoord2f(0.1f, 0.546f); glVertex3f(counter, -0.2f, 0.5f);
    glTexCoord2f(0.9f, 0.546f); glVertex3f(counter - 1.7f, -0.2f, 0.5f);
    glTexCoord2f(0.9f, 0.351f); glVertex3f(counter - 1.7f, -0.8f, 0.5f);

    glEnd();
    //
    glBindTexture(GL_TEXTURE_2D, texture[7]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.69f, 0.546f); glVertex3f(counter - 1.5f, -0.2f, 0.5f);
    glTexCoord2f(0.59f, 0.691f); glVertex3f(counter - 1.2f, 0.3f, 0.5f);
    glTexCoord2f(0.29f, 0.691f); glVertex3f(counter - 0.3f, 0.3f, 0.5f);
    glTexCoord2f(0.19f, 0.546f); glVertex3f(counter, -0.2f, 0.5f);

    glEnd();
    //���� �����
    rad = 0.22;

    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++)
    {
        double angle = i * 3.141592 / 180;
        double x = rad * cos(angle) - 1.7;
        double y = rad * sin(angle) + 1.7;
        glVertex3f(x + counter + 1.3f, y - 2.5f, 0.5f);
    }
    glEnd();
    rad = 0.22;

    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++)
    {
        double angle = i * 3.141592 / 180;
        double x = rad * cos(angle) - 1.7;
        double y = rad * sin(angle) + 1.7;
        glVertex3f(x + counter + 0.4f, y - 2.5f, 0.5f);
    }
    glEnd();

    //�ӵ����� ǥ����
    glBegin(GL_POLYGON);
    glVertex3f(0.8f, -1.0f, -0.9f);
    glVertex3f(1.0f, -1.0f, -0.9f);
    glVertex3f(1.0f, 1.0f, -0.9f);
    glVertex3f(0.8f, 1.0f, -0.9f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, 1.0f, -0.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.3f, 1.0f, -0.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.3f, 1.5f, -0.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, 1.5f, -0.9f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex3f(3.8f, -1.0f, -0.9f);
    glVertex3f(4.0f, -1.0f, -0.9f);
    glVertex3f(4.0f, 1.0f, -0.9f);
    glVertex3f(3.8f, 1.0f, -0.9f);
    glEnd();
    //��ȣ��
    glBegin(GL_POLYGON);
    glVertex3f(2.0f, -1.0f, -0.9f);
    glVertex3f(2.2f, -1.0f, -0.9f);
    glVertex3f(2.2f, 1.0f, -0.9f);
    glVertex3f(2.0f, 1.0f, -0.9f);
    glEnd();

    //pole

    if (counter < 3.0f && counter>2.9f) {//��ȣ�� ������ȣ ����
        glBindTexture(GL_TEXTURE_2D, texture[6]);
    }
    else {//�ƴϸ� �������� ��ȣ�� ��ȣ ����
        glBindTexture(GL_TEXTURE_2D, texture[5]);
    }
    glBegin(GL_QUADS);//�߰�*/
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.7f, 1.0f, -0.9f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2.5f, 1.0f, -0.9f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(2.5f, 1.5f, -0.9f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.7f, 1.5f, -0.9f);
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
    //�� : ��ƼŬ******************
    glPushMatrix();
    if (counter < 2.5f && counter>0.5f) {


        for (int i = 0; i < 1000; i++) {
            p[i].x += cos(winddir * .0174532925f) * windspeed;
            p[i].y += p[i].yd;
            p[i].z += sin(winddir * .0174532925f) * windspeed;
            p[i].yd -= rand() / 32767.0f / 100000.0f;

            if (p[i].y <= -1.0f) {
                p[i].xd = -(rand() / 32767.0f - 0.5f) / 200.0f;
                p[i].zd = -(rand() / 32767.0f - 0.5f) / 200.0f;
                p[i].yd = -rand() / 32767.0f / 100.0f;
                p[i].x = (rand() / 32767.0f - 2.0f);
                p[i].y = 1.0f;
                p[i].z = (rand() / 32767.0f - 1.0f);
                p[i].b = rand() / 32767.0f;
                p[i].g = p[i].b;
                p[i].r = p[i].b;
            }
        }
        for (int i = 0; i < 1000; i++) {
            p[i].x += cos(winddir * .0174532925f) * windspeed;
            p[i].y += p[i].yd;
            p[i].z += sin(winddir * .0174532925f) * windspeed;
            p[i].yd -= rand() / 32767.0f / 100000.0f;

            if (p[i].y <= -1.0f) {
                p[i].xd = -(rand() / 32767.0f - 0.5f) / 200.0f;
                p[i].zd = -(rand() / 32767.0f - 0.5f) / 200.0f;
                p[i].yd = -rand() / 32767.0f / 100.0f;
                p[i].x = (rand() / 32767.0f - 1.0f);
                p[i].y = 1.0f;
                p[i].z = (rand() / 32767.0f - 1.0f);
                p[i].b = rand() / 32767.0f;
                p[i].g = p[i].b;
                p[i].r = p[i].b;
            }
        }

        for (int i = 0; i < 1000; i++) {
            float difx = 0.0, dify = 0.0;
            if (i % 3 == 0) {
                difx = 0.005;
                dify = 0.01;
            }
            else if (i % 3 == 1) {
                difx = 0.005;
                dify = 0.03;
            }
            else {
                difx = 0.005;
                dify = 0.05;
            }

            glDisable(GL_TEXTURE_2D);
            //glColor3f(0.5, 0.5, 0.5);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(p[i].x - difx, p[i].y - dify, p[i].z);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(p[i].x, p[i].y - dify, p[i].z);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(p[i].x, p[i].y, p[i].z);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(p[i].x - difx, p[i].y, p[i].z);
            glEnd();
        }
        glDisable(GL_TEXTURE_2D);

    }
    glPopMatrix();
    // glPopMatrix();
     //���� ���� : tga�� �̿��� transperancy
    glPushMatrix();
    if (counter < 0.5f) {
        glDisable(GL_DEPTH_TEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);

        glBindTexture(GL_TEXTURE_2D, g_textureID);
        //6�� �� �׸��� ����

        glEnable(GL_CULL_FACE);

        glCullFace(GL_FRONT);
        //�޸� �׸���
        glInterleavedArrays(GL_T2F_V3F, 0, g_cubeVertices);
        glDrawArrays(GL_QUADS, 0, 24);


        glCullFace(GL_BACK);

        glInterleavedArrays(GL_T2F_V3F, 0, g_cubeVertices);
        glDrawArrays(GL_QUADS, 0, 24);

        glDisable(GL_CULL_FACE);
        glDisable(GL_BLEND);

    }
    glPopMatrix();
   
    glMatrixMode(GL_PROJECTION);



    glutPostRedisplay();
    glutSwapBuffers();

}

//�ִϸ��̼� ����
void MyIdle() {

    if (isanimation) {
        if (counter < 3.0f && counter>2.9f) {
            Sleep(100);
            counter = counter - 0.0015f;
        }
        else if (counter < 2.0f && counter >= -1.0f) {
            counter = counter - 0.0007f;
        }
        else if (counter < -1.0f) {
            counter = 7.5f;
        }
        else {
            counter = counter - 0.0015f;
        }


    }


    glutPostRedisplay();
}
//bmp ���� �ҷ�����
AUX_RGBImageRec* LoadBMP(char* Filename) {
    FILE* File = NULL;
    if (!Filename) return NULL;
    File = fopen(Filename, "r");
    if (File) {
        fclose(File);
        return auxDIBImageLoad(Filename);
    }
    return NULL;
}

void loadtag(void)
{
    //tga�̹��� �о�帲
    tgaImageFile tgaImage;//�ܺ����� �б����� ����
    tgaImage.load((char*)"question_box.tga"); // tga ���� �ʿ� // (char *)�� ĳ��Ʈ 
    //���� �������ʹ� bmp�� ������
    glGenTextures(1, &g_textureID);

    glBindTexture(GL_TEXTURE_2D, g_textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, tgaImage.m_texFormat,
        tgaImage.m_nImageWidth, tgaImage.m_nImageHeight,
        0, tgaImage.m_texFormat, GL_UNSIGNED_BYTE,
        tgaImage.m_nImageData);
}

int LoadGLTextures() {
    int Status = TRUE, i;
    AUX_RGBImageRec* TextureImage;

    for (i = 0; i < TEXTURE_NUM; i++) {
        TextureImage = NULL;
        if (TextureImage = LoadBMP(texture_name[i])) {//load�Ǹ� ������ //�̹��� �д°Ŷ� �����ϴ°� �и� 
            glGenTextures(1, &texture[i]);//�̸���
            glBindTexture(GL_TEXTURE_2D, texture[i]);//2d ���ε�
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage->sizeX, TextureImage->sizeY,
                1, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
        }


        else {
            MessageBox(NULL, "Image file has a error !", texture_name[i], MB_ICONERROR);
            Status = false;
        }

        if (TextureImage) {
            if (TextureImage->data) free(TextureImage->data);
            free(TextureImage);
        }
    }
    return Status;
}
//Enviroment mapping
void LoadEnviromentTextures() {
    AUX_RGBImageRec* TextureImage1[2];
    memset(TextureImage1, 0, sizeof(void*) * 2);

    if ((TextureImage1[0] = LoadBMP((char*)"Background.bmp")) && // �� ��ȯ
        (TextureImage1[1] = LoadBMP((char*)"SphereMap.bmp"))) {
        glGenTextures(6, &texture_enviroment[0]);

        for (int i = 0; i < 2; i++) {
            // Create Nearest Filtered Texture
            glBindTexture(GL_TEXTURE_2D, texture_enviroment[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage1[i]->sizeX,
                TextureImage1[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE,
                TextureImage1[i]->data);

            // Create Linear Filtered Texture
            glBindTexture(GL_TEXTURE_2D, texture_enviroment[i + 2]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage1[i]->sizeX,
                TextureImage1[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE,
                TextureImage1[i]->data);

            // Create MipMapped Texture
            glBindTexture(GL_TEXTURE_2D, texture_enviroment[i + 4]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                GL_LINEAR_MIPMAP_NEAREST);
            gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage1[i]->sizeX,
                TextureImage1[i]->sizeY, GL_RGB, GL_UNSIGNED_BYTE,
                TextureImage1[i]->data);
        }

        for (int i = 0; i < 2; i++) {
            if (TextureImage1[i]) {
                if (TextureImage1[i]->data) {
                    free(TextureImage1[i]->data);
                }
                free(TextureImage1[i]);
            }
        }
    }
}
//���� �ܾƿ� �̺�Ʈ + ����
void MySpecial(int key, int x, int y) {
    //ȭ��ǥ�� ���� �����̴�
    switch (key) {
    case GLUT_KEY_DOWN://�Ʒ����� ȭ��ǥ
        j = j - 0.1f; 
        printf("GLUT_KEY_DOWN");
        printf("\n"); break;

    case GLUT_KEY_UP://��
        j = j + 0.1f; 
        printf("GLUT_KEY_UP");
        printf("\n"); break;


    case GLUT_KEY_LEFT://����
        a = a - 0.1f; 
        printf("GLUT_KEY_LEFT");
        printf("\n"); break;

    case GLUT_KEY_RIGHT://������
        a = a + 0.1f; 
        printf("GLUT_KEY_RIGHT");
        printf("\n"); break;

    case GLUT_KEY_PAGE_UP://����
        c = c + 0.1f; 
        printf("GLUT_KEY_PAGE_UP");
        printf("\n"); break;

    case GLUT_KEY_PAGE_DOWN://�ܾƿ�
        if (c > 3.0f) {
            c = c - 0.1f;
            printf("GLUT_KEY_PAGE_DOWN");
            printf("\n");
        } break;



    }
    //�׸��� �ٽ� �׷�����Ѵٴ°�
    glutPostRedisplay();//MyDisplay() ȣ��
}
//�ִϸ��̼� ����
void MyMouseClick(GLint Button, GLint State, GLint X, GLint Y) {
    switch (Button)
    {
    case GLUT_LEFT_BUTTON:
        printf("GLUT_LEFT_BUTTON");
        printf("\n");isanimation = true;
        
        break;
    case GLUT_RIGHT_BUTTON:
        printf("GLUT_RIGHT_BUTTON");
        printf("\n");isanimation = false;
        
        break;
    }
}

void main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("final_project");
    printf("*************************************************\n20181024 ������ ��ǻ�ͱ׷��Ƚ� �⸻����\n*************************************************\n");
   
    Init();
    glutDisplayFunc(MyDisplay);

    glutReshapeFunc(MyReshape);
    //glutKeyboardFunc(MyKeyboard);
    glutSpecialFunc(MySpecial);
    glutMouseFunc(MyMouseClick);
    //glutMotionFunc(MyMotion);//ȭ����ȯ
    LoadGLTextures();
    LoadEnviromentTextures();
    loadtag();

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glutIdleFunc(MyIdle);
    glutMainLoop();
}