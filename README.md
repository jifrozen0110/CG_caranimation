# CG_caranimation
컴퓨터 그래픽스
![image](https://user-images.githubusercontent.com/62784314/103120416-fef0fe00-46ba-11eb-82a4-8c8ace72e6bd.png)
![image](https://user-images.githubusercontent.com/62784314/103120422-03b5b200-46bb-11eb-92b0-dca18c6b50f9.png)
![image](https://user-images.githubusercontent.com/62784314/103120427-06180c00-46bb-11eb-82f7-1febe6bcb52c.png)
![image](https://user-images.githubusercontent.com/62784314/103120430-087a6600-46bb-11eb-80d1-cc5e33587daa.png)
1) 주제 설정
신호등 빨간불에서 멈춰야 하고 속도위반 표지판에 맞춰 속도를 늦춰야한다.
자동차는 직진을 해 도착상자를 먹으면 제자리로 돌아간다.

2) window 해상도

glutInitWindowSize(1024, 768);


3),4) 공간 배치 , 물체 배치
-> 영상확인

5) Texture mapping
– 이미지를 이용하여 바닥과 벽을 사실적으로 표현
– 모든 물체는 텍스처 매핑이 되어있어야 함.
1. .tga 파일을 이용하여 투명한 텍스처 매핑
코드 일부분

void loadtag(void)
{
    tgaImageFile tgaImage;//외부파일 읽기위해 선언
    tgaImage.load((char*)"question_box.tga"); // tga 파일 필요 // (char *)로 캐스트 
    //읽은 다음부터는 bmp와 동일함
    glGenTextures(1, &g_textureID);
    glBindTexture(GL_TEXTURE_2D, g_textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, tgaImage.m_texFormat,
        tgaImage.m_nImageWidth, tgaImage.m_nImageHeight,
        0, tgaImage.m_texFormat, GL_UNSIGNED_BYTE,
        tgaImage.m_nImageData);
}

//도착 상자 : tga를 이용해 transperancy
    glPushMatrix();
    if (counter < 0.5f) {
        glDisable(GL_DEPTH_TEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);

        glBindTexture(GL_TEXTURE_2D, g_textureID);
        //6개 면 그리는 순서

        glEnable(GL_CULL_FACE);

        glCullFace(GL_FRONT);
        //뒷면 그리고
        glInterleavedArrays(GL_T2F_V3F, 0, g_cubeVertices);
        glDrawArrays(GL_QUADS, 0, 24);


        glCullFace(GL_BACK);

        glInterleavedArrays(GL_T2F_V3F, 0, g_cubeVertices);
        glDrawArrays(GL_QUADS, 0, 24);

        glDisable(GL_CULL_FACE);
        glDisable(GL_BLEND);

    }
    glPopMatrix();

2. Environment mapping 
부족한 점 : 매핑 후 이벤트 실행하면 비 컬러가 매핑된 후 화면에서 사라짐

//Enviroment mapping
void LoadEnviromentTextures() {
    AUX_RGBImageRec* TextureImage1[2];
    memset(TextureImage1, 0, sizeof(void*) * 2);

    if ((TextureImage1[0] = LoadBMP((char*)"Background.bmp")) && // 형 변환
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
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, -1.0f, -1.0f);	// 뒷면
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-3.0f, 2.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(7.5f, 2.0f, -1.0f);
    glEnd();
    //glDisable(GL_TEXTURE_2D);
    glPopMatrix();


6) Particle systems
비

//winddir 비 각도 windspeed 비속도
int screennum = 0, winddir = 45;
GLfloat windspeed = 0.0005f;

//파티클 구조체 선언 
struct particle {
    GLfloat x, y, z;
    GLfloat r, g, b;
    GLfloat xd, yd, zd;
    GLfloat cs;
} p[1000];
//비 : 파티클******************
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


7) 조명을 추가해 더욱 사실적인 공간을 나타냄

GLfloat Light0_Ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat Light0_Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat Light0_Position[] = { 0.0f, 0.0f, 2.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, Light0_Ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Light0_Diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, Light0_Position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);


8) 카메라 이동 : 다양한 거리, 각도에서 코트를 바라볼 수 있음
– Keyboard의 화살표(↑← ↓ → ) 키들을 사용하여 Camera가 상, 하, 좌,
우 이동이 가능하여야 하며, <PgUp> 및 <PgDn> 키를 사용하여 Zoom
In/Out이 가능하여야 함
– Zoom In/Out에서, Zoom In의 경우 모든 객체(Object)들이 Window 영
역을 벗어나지 않게 제약조건을 부여하고, Zoom Out의 경우에는 모든
객체(Object)들을 제대로 볼 수 있는 최소한의 위치에서 더 이상 Zoom
Out이 되지 않도록 제약조건을 부여함 -> 영상확인

   //시점 설정
    gluLookAt(a, j, c, a, j, 0.0, 0.0, 1.0, 0.0);

//줌인 줌아웃 이벤트 + 시점
void MySpecial(int key, int x, int y) {
    //화살표에 따라 움직이는
    switch (key) {
    case GLUT_KEY_DOWN://아래방향 화살표
        j = j - 0.1f; 
        printf("GLUT_KEY_DOWN");
        printf("\n"); break;

    case GLUT_KEY_UP://위
        j = j + 0.1f; 
        printf("GLUT_KEY_UP");
        printf("\n"); break;


    case GLUT_KEY_LEFT://왼쪽
        a = a - 0.1f; 
        printf("GLUT_KEY_LEFT");
        printf("\n"); break;

    case GLUT_KEY_RIGHT://오른쪽
        a = a + 0.1f; 
        printf("GLUT_KEY_RIGHT");
        printf("\n"); break;

    case GLUT_KEY_PAGE_UP://줌인
        c = c + 0.1f; 
        printf("GLUT_KEY_PAGE_UP");
        printf("\n"); break;

    case GLUT_KEY_PAGE_DOWN://줌아웃
        if (c > 3.0f) {
            c = c - 0.1f;
            printf("GLUT_KEY_PAGE_DOWN");
            printf("\n");
        } break;



    }
    //그림을 다시 그려줘야한다는거
    glutPostRedisplay();//MyDisplay() 호출
}

9) Animation
– 타이머를 이용하여 물체의 기하변환을 애니메이션 함
– 단. 애니메이션은 주제에 부합하여야 함
– Mouse의 Left Button을 Click하면 전체적인 Animation이 Play
– Mouse의 Right Button을 Click할 경우 Animation이 Pause

//애니메이션 조절
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

– 물론 현재 Pause된 상태에서 Mouse의 Left Button을 다시 Click할 경우
다시 Play
– Animation은 무한 반복되어져야 하며, 자연스럽게 연결되어져야 함.
-> 영상 확인
