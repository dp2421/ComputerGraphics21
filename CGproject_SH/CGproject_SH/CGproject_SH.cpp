#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <random>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp> //수현
//#include <glm/glm/glm.hpp>
//#include <glm/glm/ext.hpp>
//#include <glm/glm/gtc/matrix_transform.hpp> //예나


////////너랑 나랑 gl경로가 달라서 서로 상대방 거 주석처리하고 사용하는 걸로 하자!/////////

////////////////////미래의 상대방에게 전하는 메시지////////////////////

//쉐이더 수정하면서 컬러값을 좌표가 아니라 밑에 드로우에서 glUniform3f(Color_1, 0.690196, 0.768627, 0.870588); 이렇게 받아오는걸로 수정
//조명이 저모양으로나오는는거 수정해야함...왜저러지 흑흑
//충돌함수같은것들 이름 전부 cube->player 수정, player 사이즈, 방향 수정, 스테이지 1, 2 색상 다르게 수정
//충돌하면 해당 스테이지 시작점으로 이동, 스테이지 2 카메라 조금만 더 손보면 좋을 듯?
//그리고 스테이지 1에서 끝까지 가면 플레이어가 뒤돌고 스테이지 2 바로 시작!

using namespace std;
unsigned int texture[1];

GLvoid drawScene(GLvoid);
GLvoid Reshape(int, int);
GLvoid CubeInitBuffer();
GLvoid GroundInitBuffer();
GLvoid PlayerInitBuffer();
GLvoid CarInitBuffer();
GLvoid InitTexture();

void make_vertexShaders();
void make_fragmentShaders();
void make_shaderProgram();
unsigned int Color_1;
GLchar* vertexsource, * fragmentsource;
GLuint vertexShader, fragmentShader;
GLuint shaderID;
GLuint s_program;

GLvoid Keyboard(void (*func)(unsigned char key, int x, int y));
GLvoid TimeFunction(int value);

GLint result;
GLchar errorLog[512];

GLchar* filetobuf(const char* file);

GLuint vao[4], vbo[3], ebo;

//육면체 위치 변수
GLfloat CubePosX = 0.0f;
GLfloat CubePosZ = 30.0f;

//카메라 위치 변수
GLfloat CamPosX = 2.0f;
GLfloat CamPosY = 5.0f;
GLfloat CamPosZ = 50.0f;

//카메라가 보는 방향 변수
GLfloat CamDirX = CubePosX;
GLfloat CamDirZ = CubePosZ - 7.0f;

//자동차 움직임
glm::mat4 Carmoving = glm::mat4(1.f);
glm::mat4 Carmoving2 = glm::mat4(1.f);
glm::mat4 Carmoving3 = glm::mat4(1.f);

//wasd check
bool checkW = false;
bool checkA = false;
bool checkS = false;
bool checkD = false;

//충돌 bool값
bool checkCrash1 = false;
bool checkCrash2 = false;

//스테이지 종료 bool값
bool checkStage1 = true;
bool checkStage2 = false;

//맵(1이 장애물이 있는 칸, 0은 없는 칸)
int map1[30] = { 0,0,0,0,1,1,1,0,0,1,1,0,0,0,1,0,0,0,1,1,0,0,0,0,1,1,1,0,0,0 };
int map2[30] = { 0,0,0,1,1,1,0,0,1,1,1,0,0,0,1,1,0,1,0,1,1,1,0,0,1,1,1,0,0,0 };


//obj 관련 변수 
int loadObj(const char* filename);
int loadObj_normalize_center(const char* filename);	//불러오는 함수들 (밑에있음)
int loadObj_normalize_center1(const char* filename);

int num_Triangle;
const int num_vertices = 3;
const int num_triangles = 1;
std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
std::vector< glm::vec3 > temp_vertices;
std::vector< glm::vec2 > temp_uvs;
std::vector< glm::vec3 > temp_normals;
std::vector< glm::vec3 > outvertex, outnormal;
std::vector< glm::vec2 > outuv;
float sumX = 0.0, sumY = 0.0, sumZ = 0.0;
float aveX, aveY, aveZ;
float scaleX, scaleY, scaleZ;
float minX = 0.0, minY = 0.0, minZ = 0.0;
float maxX = 0.0, maxY = 0.0, maxZ = 0.0;
float scaleAll;


int num_Triangle1;
const int num_vertices1 = 3;
const int num_triangles1 = 1;
std::vector< unsigned int > vertexIndices1, uvIndices1, normalIndices1;
std::vector< glm::vec3 > temp_vertices1;
std::vector< glm::vec2 > temp_uvs1;
std::vector< glm::vec3 > temp_normals1;
std::vector< glm::vec3 > outvertex1, outnormal1;
std::vector< glm::vec2 > outuv1;
float sumX1 = 0.0, sumY1 = 0.0, sumZ1 = 0.0;
float aveX1, aveY1, aveZ1;
float scaleX1, scaleY1, scaleZ1;
float minX1 = 0.0, minY1 = 0.0, minZ1 = 0.0;
float maxX1 = 0.0, maxY1 = 0.0, maxZ1 = 0.0;
float scaleAll1;
struct BB {
	float minx;
	float minz;
	float maxx;
	float maxz;

	BB() {}

	BB(float minX, float minZ, float maxX, float maxZ)
	{
		this->minx = minX;
		this->minz = minZ;
		this->maxx = maxX;
		this->maxz = maxZ;
	}
};

BB getbb_player(float centerx, float centerz)
{
	return BB(centerx - 0.5f, centerz, centerx + 0.5f, centerz);
}

BB getbb_car(float centerx, float centerz)
{
	return BB(centerx, centerz - 0.5f, centerx, centerz + 0.5f);
}

bool Collide(BB a, BB b)
{
	if (a.maxx <= b.minx)
		return false;
	if (a.minx >= b.maxx)
		return false;
	if (a.maxz <= b.minz)
		return false;
	if (a.minz >= b.maxz)
		return false;
	return true;
}

GLfloat ground[][3] = {
	-1, 0.0, -1,
	1, 0.0, -1,
	-1, 0.0, 1,
	1, 0.0, 1,
};


GLuint ground_element[] = { 2, 0, 1, 2, 1, 3, };

GLfloat cube[][3] = {
	-0.5, -0.5, -0.5,
	0.5, -0.5, -0.5,
	-0.5, 0.5, -0.5,
	0.5, 0.5, -0.5,
	-0.5, -0.5, 0.5,
	0.5, -0.5, 0.5,
	-0.5, 0.5, 0.5,
	0.5, 0.5, 0.5,
};

GLfloat ground_Texture[][6] = {
	{0.0, 1.0,
	0.0, 0.0,
	1.0, 1.0},
	{1.0, 1.0,
	0.0, 0.0,
	1.0, 0.0},
};

GLuint cubelement[36] = {
	2, 0, 1, 2, 1, 3,
	0, 4, 5, 0, 5, 1,
	3, 1, 5, 3, 5, 7,
	7, 5, 4, 7, 4, 6,
	6, 4, 0, 6, 0, 2,
	6, 2, 3, 6, 3, 7
};

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		CubePosZ -= 2.f;
		CamDirZ -= 2.f;
		CamPosZ -= 2.f;
		cout << "cubeZ:" << CubePosZ << endl;
		checkW = true;
		checkA = false;
		checkS = false;
		checkD = false;
		break;
	case 'a':
		CubePosX -= 2.f;
		checkW = false;
		checkA = true;
		checkS = false;
		checkD = false;
		break;
	case 's':
		CubePosZ += 2.f;
		CamDirZ += 2.f;
		CamPosZ += 2.f;
		cout << "cubeZ:" << CubePosZ << endl;
		checkW = false;
		checkA = false;
		checkS = true;
		checkD = false;
		break;
	case 'd':
		CubePosX += 2.f;
		checkW = false;
		checkA = false;
		checkS = false;
		checkD = true;
		break;
		/////////////////////////////카메라가 이동할 수 있는게 짜기 편할 것 같아서 x,y,z로 이동할 수 있게 해놨어!!!///////////////////////
		///////////카메라 이동시 CameraDirection도 같이 변경해주어야 해서 수정///////////
	case 'x':
		CamPosX += 1.0f;
		CamDirX += 1.f;	//요게 추가한거
		break;
	case 'y':
		CamPosY += 1.0f;
		//CamDirY += 1.f;
		break;
	case 'z':
		CamPosZ += 1.0f;
		CamDirZ += 1.f;
		break;
	case 'X':
		CamPosX -= 1.0f;
		CamDirX -= 1.f;
		break;
	case 'Y':
		CamPosY -= 1.0f;
		break;
	case 'Z':
		CamPosZ -= 1.0f;
		CamDirZ -= 1.f;
		break;
	case 'r':
		checkCrash1 = false;
		CubePosX = 0.0f;
		CubePosZ = 30.0f;
		CamPosX = 2.0f;
		CamPosY = 5.0f;
		CamPosZ = 50.0f;
		break;
	case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

float cnt[3] = { -10.f };	//도로의 끝인지, 체크하는 변수
GLfloat CarPosZ;
GLvoid TimeFunction(int value)
{
	if (cnt[0] >= 10.f) {
		cnt[0] = -10.f;
		Carmoving = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0));
	}
	else if (cnt[1] >= 10.f) {
		cnt[1] = -10.f;
		Carmoving2 = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0));
	}
	else if (cnt[2] >= 10.f) {
		cnt[2] = -10.f;
		Carmoving3 = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0));
	}
	else {
		Carmoving = glm::translate(Carmoving, glm::vec3(0.05, 0, 0));
		Carmoving2 = glm::translate(Carmoving2, glm::vec3(0.08, 0, 0));
		Carmoving3 = glm::translate(Carmoving3, glm::vec3(0.1, 0, 0));
		cnt[0] += 0.05;
		cnt[1] += 0.08;
		cnt[2] += 0.1;
		for (int i = 0; i < 30; ++i)
		{
			CarPosZ = 30 - 2 * i;
		}
	}

	glutPostRedisplay();
	glutTimerFunc(10, TimeFunction, 1);
}

GLvoid Camera()
{
	//카메라============================================================================================================================
	glm::vec3 camerapos = glm::vec3(CamPosX, CamPosY, CamPosZ);
	glm::vec3 cameradirection = glm::vec3(CamDirX, 0.0f, CamDirZ);
	glm::vec3 cameraup = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(camerapos, cameradirection, cameraup);

	GLuint viewlocation = glGetUniformLocation(s_program, "viewTransform");
	glUniformMatrix4fv(viewlocation, 1, GL_FALSE, value_ptr(view));
}

GLvoid Projection()
{
	//투영===============================================================================================================================
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(60.0f), (float)800.0 / (float)800.0, 0.1f, 100.0f);
	GLuint Projectionlocation = glGetUniformLocation(s_program, "projectionTransform");
	glUniformMatrix4fv(Projectionlocation, 1, GL_FALSE, value_ptr(projection));
}

GLvoid Ground()
{
	glUniform3f(Color_1, 0.690196, 0.768627, 0.870588);
	//바닥 그리기=========================================================================================================================
	glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 2.0f, 30.0f));

	unsigned int TransformLocation = glGetUniformLocation(s_program, "modelTransForm"); //--- 버텍스 세이더에서 모델링 변환 위치 가져오기
	glUniformMatrix4fv(TransformLocation, 1, GL_FALSE, glm::value_ptr(Scale)); //--- modelTransform 변수에 변환 값 적용하기
	glBindVertexArray(vao[0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

GLvoid Player()
{
	glUniform3f(Color_1, 1, 0.713725, 0.756863);
	glm::mat4 Rot;
	if (checkStage2 == true)
		Rot = glm::rotate(glm::mat4(1.f), glm::radians(0.f), glm::vec3(0, 1, 0));
	else
		Rot = glm::rotate(glm::mat4(1.f), glm::radians(180.f), glm::vec3(0, 1, 0));
	glm::mat4 Scale = glm::scale(glm::mat4(1.f), glm::vec3(0.7, 0.7, 0.7));
	glm::mat4 Trans = glm::translate(glm::mat4(1.0f), glm::vec3(CubePosX, 0.7f, CubePosZ));
	glm::mat4 Mat_Cube = Trans * Rot * Scale;

	GLuint TransformLocation = glGetUniformLocation(s_program, "modelTransForm"); //--- 버텍스 세이더에서 모델링 변환 위치 가져오기
	glUniformMatrix4fv(TransformLocation, 1, GL_FALSE, glm::value_ptr(Mat_Cube)); //--- modelTransform 변수에 변환 값 적용하기
	glBindVertexArray(vao[2]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, num_Triangle);
}

GLvoid Car1()
{
	glUniform3f(Color_1, 1, 0.713725, 0.756863);
	int mapcnt = 0;
	glm::mat4 Trans = glm::mat4(1.f);
	for (int i = 0; i < 30; i++)
	{
		//자동차 위치 변수
		CarPosZ = 30 - 2 * i;
		if (map1[i] == 1)
		{
			glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
			glm::mat4 BottomScale = glm::scale(glm::mat4(1.f), glm::vec3(2.f, 1.f, 1.f));
			glm::mat4 UpperTrans = glm::translate(glm::mat4(1.f), glm::vec3(0, 0.7, 0));
			glm::mat4 Mat_Car = glm::mat4(1.f);
			glm::mat4 Trans_Car = glm::mat4(1.f);
			Trans = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, 0.6f, CarPosZ));
			switch (mapcnt)
			{
			case 0:
			case 3:
			case 5:
			case 8:
				Mat_Car = Carmoving3 * Trans;
				break;
			case 1:
			case 4:
			case 6:
			case 9:
				Mat_Car = Carmoving2 * Trans;
				break;
			default:
				Mat_Car = Carmoving * Trans;
			}
			//Trans_Car = Mat_Car * UpperTrans * Scale;
			Trans_Car = Mat_Car;
			GLuint TransformLocation = glGetUniformLocation(s_program, "modelTransForm"); //--- 버텍스 세이더에서 모델링 변환 위치 가져오기
			glUniformMatrix4fv(TransformLocation, 1, GL_FALSE, glm::value_ptr(Trans_Car)); //--- modelTransform 변수에 변환 값 적용하기
			glBindVertexArray(vao[3]);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, num_Triangle1);

			if (checkCrash1 == false)
			{
				//cout << cnt[0] << endl;

				if (Collide(getbb_player(CubePosX, CubePosZ), getbb_car(cnt[0], CarPosZ)) == true)
				{
					CubePosX = 0.0f;
					CubePosZ = 30.0f;
					CamPosX = 2.0f;
					CamPosY = 5.0f;
					CamPosZ = 50.0f;
				}
			}

			if (CubePosZ < -30.f)
			{
				checkStage2 = true; //스테이지2 시작
				checkStage1 = false; //충돌 시 스테이지1 자동차 사라짐
				CamPosX = 2.0f;
				CamPosY = 5.0f;
				CamPosZ = -5.f;
				CamDirZ = CubePosZ;	//카메라 조정(반대편)
			}
			mapcnt++;
		}
	}
}

GLvoid Car2()
{
	glUniform3f(Color_1, 0.713725, 1, 0.756863);
	int mapcnt = 0;
	glm::mat4 Trans = glm::mat4(1.f);
	for (int i = 0; i < 30; i++)
	{
		//자동차 위치 변수
		CarPosZ = 30 - 2 * i;
		if (map2[i] == 1)
		{
			glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.f, 0.7f, 1.f));
			glm::mat4 BottomScale = glm::scale(glm::mat4(1.f), glm::vec3(2.f, 1.f, 1.f));
			glm::mat4 UpperTrans = glm::translate(glm::mat4(1.f), glm::vec3(0, 0.7, 0));
			glm::mat4 Mat_Car = glm::mat4(1.f);
			glm::mat4 Trans_Car = glm::mat4(1.f);
			Trans = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, 0.6f, CarPosZ));
			switch (mapcnt)
			{
			case 0:
			case 3:
			case 5:
			case 8:
				Mat_Car = Carmoving3 * 2.0f * Trans; //속도 좀 더 빠르게
				break;
			case 1:
			case 4:
			case 6:
			case 9:
				Mat_Car = Carmoving2 * Trans;
				break;
			default:
				Mat_Car = Carmoving * 1.5f * Trans;
			}
			Trans_Car = Mat_Car;
			GLuint TransformLocation = glGetUniformLocation(s_program, "modelTransForm"); //--- 버텍스 세이더에서 모델링 변환 위치 가져오기
			glUniformMatrix4fv(TransformLocation, 1, GL_FALSE, glm::value_ptr(Trans_Car)); //--- modelTransform 변수에 변환 값 적용하기
			glBindVertexArray(vao[3]);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, num_Triangle1);

			//Trans_Car = Mat_Car * BottomScale;
			//glUniformMatrix4fv(TransformLocation, 1, GL_FALSE, glm::value_ptr(Trans_Car)); //--- modelTransform 변수에 변환 값 적용하기
			//glBindVertexArray(vao[1]);
			//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			if (checkCrash2 == false)
			{
				if (Collide(getbb_player(CubePosX, CubePosZ), getbb_car(cnt[0], CarPosZ)) == true) //차에 큐브가 갖다 들이박기, 지나가는 차에 치이기 둘 다 충돌 처리
				{
					CubePosX = 0.0f;
					CubePosZ = -30.0f;
					CamPosX = 2.0f;
					CamPosY = 5.0f;
					CamPosZ = -5.f;
					CamDirZ = CubePosZ;	//카메라 조정(반대편)
				}
			}
			mapcnt++;
		}
	}
}
GLvoid Light()
{
	int lightColorLocation = glGetUniformLocation(s_program, "lightColor");
	int lightPosLocation = glGetUniformLocation(s_program, "lightPos");
	glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);
	glUniform3f(lightPosLocation, CubePosX, 20, CubePosZ);
}

GLvoid InitTexture()
{
	BITMAP* bmp;

	int width, height, nrChannels;

	glGenTextures(1, &texture[0]); //--- 텍스처 생성

	glBindTexture(GL_TEXTURE_2D, texture[0]); //--- 텍스처 바인딩
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load("stage1.png", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //---텍스처 이미지 정의

	glUseProgram(shaderID);
	int tLocation = glGetUniformLocation(s_program, "outTexture"); //--- outTexture 유니폼 샘플러의 위치를 가져옴
	glUniform1i(tLocation, 0); //--- 샘플러를 0번 유닛으로 설정
	stbi_image_free(data);
}

GLvoid GroundInitBuffer()
{
	glGenVertexArrays(1, &vao[0]);
	glBindVertexArray(vao[0]);

	glGenBuffers(1, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);	//첫 번째 인자: 인덱스
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &vbo[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground_Texture), ground_Texture, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); // GL_ELEMENT_ARRAY_BUFFER 버퍼 유형으로 바인딩
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ground_element), ground_element, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	//glEnableVertexAttribArray(2);	//vao에 vbo를 묶어줌
}

GLvoid GroundTexture()
{
	Ground();
	glBindVertexArray(vao[0]); //--- 첫 번째 폴리곤
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]); //--- texture[0]을 사용하여 폴리곤을 그린다.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawScene() //--- glutDisplayFunc()함수로 등록한 그리기 콜백 함수
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(s_program);
	Light();
	Camera(); //카메라
	Projection(); //투영
	Ground(); //바닥 그리기
	if (checkStage1)
	{
		if (checkCrash1 == false)
		{
			Player(); //객체 그리기
		}
		Car1();
	}
	if (checkStage2)
	{
		if (checkCrash2 == false)
		{
			Player();
		}
		Car2();
	}
	//glFrontFace(GL_CCW);
	glutPostRedisplay();
	glutSwapBuffers();
}

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 800);
	glutCreateWindow("길건너 친구들");

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	make_shaderProgram();
	CubeInitBuffer();
	GroundInitBuffer();
	PlayerInitBuffer();
	CarInitBuffer();
	Color_1 = glGetUniformLocation(s_program, "in_Color");
	glEnable(GL_DEPTH_TEST);

	glGenBuffers(3, vbo);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(10, TimeFunction, 1);
	glutMainLoop();
}

GLvoid PlayerInitBuffer()
{
	num_Triangle = loadObj_normalize_center("child.obj");
	//// 5.1. VAO 객체 생성 및 바인딩
	glGenVertexArrays(2, &vao[2]);
	glGenBuffers(2, &vbo[0]);
	glGenBuffers(2, &vbo[1]);

	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, outvertex.size() * sizeof(glm::vec3), &outvertex[0], GL_STATIC_DRAW);
	GLint pAttribute = glGetAttribLocation(s_program, "vPos");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, outnormal.size() * sizeof(glm::vec3), &outnormal[0], GL_STATIC_DRAW);
	GLint nAttribute = glGetAttribLocation(s_program, "vNormal");
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);
}
GLvoid CubeInitBuffer()
{
	glGenVertexArrays(1, &vao[1]);
	glBindVertexArray(vao[1]);

	glGenBuffers(1, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);	//첫 번째 인자: 인덱스
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); // GL_ELEMENT_ARRAY_BUFFER 버퍼 유형으로 바인딩
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubelement), cubelement, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(2);	//vao에 vbo를 묶어줌
}
GLvoid CarInitBuffer()
{
	num_Triangle1 = loadObj_normalize_center1("car.obj");
	//// 5.1. VAO 객체 생성 및 바인딩
	glGenVertexArrays(2, &vao[3]);
	glGenBuffers(2, &vbo[0]);
	glGenBuffers(2, &vbo[1]);

	glBindVertexArray(vao[3]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, outvertex1.size() * sizeof(glm::vec3), &outvertex1[0], GL_STATIC_DRAW);
	GLint pAttribute = glGetAttribLocation(s_program, "vPos");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, outnormal1.size() * sizeof(glm::vec3), &outnormal1[0], GL_STATIC_DRAW);
	GLint nAttribute = glGetAttribLocation(s_program, "vNormal");
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

void make_vertexShaders()
{
	vertexsource = filetobuf("lightvertex.glsl");

	//--- 버텍스 세이더 객체 만들기
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexsource, 0);

	//--- 버텍스 세이더 컴파일하기
	glCompileShader(vertexShader);

	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << endl;
		return exit(-1);
	}
}

void make_fragmentShaders()
{
	fragmentsource = filetobuf("mirrorfragment.glsl");
	//--- 프래그먼트 세이더 객체 만들기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentsource, 0);
	//--- 프래그먼트 세이더 컴파일
	glCompileShader(fragmentShader);
	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << endl;
		return exit(-1);
	}
}

void make_shaderProgram()
{
	make_vertexShaders();
	make_fragmentShaders();

	s_program = glCreateProgram();

	glAttachShader(s_program, vertexShader);
	glAttachShader(s_program, fragmentShader);
	glLinkProgram(s_program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	if (!result)
	{
		glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
		cerr << "Error : shader program 연결 실패 \n" << errorLog << endl;
		exit(-1);
	}

	glUseProgram(s_program);
}

GLchar* filetobuf(const char* file) {
	long length;
	char* buf;

	FILE* fptr = fopen(file, "rb");
	if (fptr == NULL) {
		cout << "파일 오픈 실패" << endl;
		return NULL;
	}
	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);
	buf = (char*)malloc(length + 1);
	fseek(fptr, 0, SEEK_SET);
	fread(buf, length, 1, fptr);
	fclose(fptr);
	buf[length] = 0;

	return buf;
}
int loadObj(const char* filename)
{
	FILE* objFile;

	fopen_s(&objFile, filename, "rb");

	if (objFile == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(objFile, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(objFile, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

			if (vertex.x < minX) minX = vertex.x;
			if (vertex.y < minY) minY = vertex.y;
			if (vertex.z < minZ) minZ = vertex.z;
			if (vertex.x > maxX) maxX = vertex.x;
			if (vertex.y > maxY) maxY = vertex.y;
			if (vertex.z > maxZ) maxZ = vertex.z;
			sumX += vertex.x;
			sumY += vertex.y;
			sumZ += vertex.z;

			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(objFile, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(objFile, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(objFile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	std::cout << "minX: " << minX << " minY: " << minY << " minZ: " << minZ << std::endl;
	std::cout << "maxX: " << maxX << " maxY: " << maxY << " maxZ: " << maxZ << std::endl;

	aveX = sumX / vertexIndices.size();
	aveY = sumY / vertexIndices.size();
	aveZ = sumZ / vertexIndices.size();
	scaleX = (1.0 - maxX) * 10 + 1;
	scaleY = (1.0 - maxY) * 10 + 1;
	scaleZ = (1.0 - maxZ) * 10 + 1;

	if (scaleX > scaleY) {
		if (scaleY > scaleZ)
			scaleAll = scaleZ;
		else
			scaleAll = scaleY;
	}
	else if (scaleX < scaleY) {
		if (scaleX < scaleZ)
			scaleAll = scaleX;
		else
			scaleAll = scaleZ;
	}
	std::cout << "aveX: " << aveX << " aveY: " << aveY << " aveZ: " << aveZ << std::endl;

	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		outvertex.push_back(vertex);
	}
	for (unsigned int i = 0; i < uvIndices.size(); i++) {
		unsigned int uvIndex = uvIndices[i];
		glm::vec2 vertex = temp_uvs[uvIndex - 1];
		outuv.push_back(vertex);
	}
	for (unsigned int i = 0; i < normalIndices.size(); i++) {
		unsigned int normalIndex = normalIndices[i];
		glm::vec3 vertex = temp_normals[normalIndex - 1];
		outnormal.push_back(vertex);
	}

	return outvertex.size();
}
int loadObj_normalize_center(const char* filename)
{
	FILE* objFile;

	fopen_s(&objFile, filename, "rb");

	if (objFile == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(objFile, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(objFile, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

			if (vertex.x < minX) minX = vertex.x;
			if (vertex.y < minY) minY = vertex.y;
			if (vertex.z < minZ) minZ = vertex.z;
			if (vertex.x > maxX) maxX = vertex.x;
			if (vertex.y > maxY) maxY = vertex.y;
			if (vertex.z > maxZ) maxZ = vertex.z;
			sumX += vertex.x;
			sumY += vertex.y;
			sumZ += vertex.z;

			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(objFile, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(objFile, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(objFile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	std::cout << "minX: " << minX << " minY: " << minY << " minZ: " << minZ << std::endl;
	std::cout << "maxX: " << maxX << " maxY: " << maxY << " maxZ: " << maxZ << std::endl;

	aveX = sumX / vertexIndices.size();
	aveY = sumY / vertexIndices.size();
	aveZ = sumZ / vertexIndices.size();
	scaleX = maxX - minX;
	scaleY = maxY - minY;
	scaleZ = maxZ - minZ;

	glm::vec3 temp;

	std::cout << "aveX: " << aveX << " aveY: " << aveY << " aveZ: " << aveZ << std::endl;

	std::cout << "scaleX: " << scaleX << " scaleY: " << scaleY << " scaleZ: " << scaleZ << std::endl;

	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		temp = temp_vertices[vertexIndex - 1];
		temp.x = temp.x - minX;
		temp.y = temp.y - minY;
		temp.z = temp.z - minZ;

		temp.x = ((temp.x * 2.0f) / scaleX) - 1.0f;
		temp.y = ((temp.y * 2.0f) / scaleY) - 1.0f;
		temp.z = ((temp.z * 2.0f) / scaleZ) - 1.0f;

		outvertex.push_back(temp);
		//glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		//outvertex.push_back(vertex);
	}
	for (unsigned int i = 0; i < uvIndices.size(); i++) {
		unsigned int uvIndex = uvIndices[i];
		glm::vec2 vertex = temp_uvs[uvIndex - 1];
		outuv.push_back(vertex);
	}
	for (unsigned int i = 0; i < normalIndices.size(); i++) {
		unsigned int normalIndex = normalIndices[i];
		glm::vec3 vertex = temp_normals[normalIndex - 1];
		outnormal.push_back(vertex);
	}

	return outvertex.size();
}
int loadObj_normalize_center1(const char* filename)
{
	FILE* objFile;

	fopen_s(&objFile, filename, "rb");

	if (objFile == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(objFile, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(objFile, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

			if (vertex.x < minX1) minX1 = vertex.x;
			if (vertex.y < minY1) minY1 = vertex.y;
			if (vertex.z < minZ1) minZ1 = vertex.z;
			if (vertex.x > maxX1) maxX1 = vertex.x;
			if (vertex.y > maxY1) maxY1 = vertex.y;
			if (vertex.z > maxZ1) maxZ1 = vertex.z;
			sumX1 += vertex.x;
			sumY1 += vertex.y;
			sumZ1 += vertex.z;

			temp_vertices1.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(objFile, "%f %f\n", &uv.x, &uv.y);
			temp_uvs1.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(objFile, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals1.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(objFile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices1.push_back(vertexIndex[0]);
			vertexIndices1.push_back(vertexIndex[1]);
			vertexIndices1.push_back(vertexIndex[2]);
			uvIndices1.push_back(uvIndex[0]);
			uvIndices1.push_back(uvIndex[1]);
			uvIndices1.push_back(uvIndex[2]);
			normalIndices1.push_back(normalIndex[0]);
			normalIndices1.push_back(normalIndex[1]);
			normalIndices1.push_back(normalIndex[2]);
		}
	}

	std::cout << "minX: " << minX << " minY: " << minY << " minZ: " << minZ << std::endl;
	std::cout << "maxX: " << maxX << " maxY: " << maxY << " maxZ: " << maxZ << std::endl;

	aveX1 = sumX1 / vertexIndices1.size();
	aveY1 = sumY1 / vertexIndices1.size();
	aveZ1 = sumZ1 / vertexIndices1.size();
	scaleX1 = maxX1 - minX1;
	scaleY1 = maxY1 - minY1;
	scaleZ1 = maxZ1 - minZ1;

	glm::vec3 temp;

	std::cout << "aveX: " << aveX1 << " aveY: " << aveY1 << " aveZ: " << aveZ1 << std::endl;

	std::cout << "scaleX: " << scaleX1 << " scaleY: " << scaleY1 << " scaleZ: " << scaleZ1 << std::endl;

	for (unsigned int i = 0; i < vertexIndices1.size(); i++) {
		unsigned int vertexIndex = vertexIndices1[i];
		temp = temp_vertices1[vertexIndex - 1];
		temp.x = temp.x - minX1;
		temp.y = temp.y - minY1;
		temp.z = temp.z - minZ1;

		temp.x = ((temp.x * 2.0f) / scaleX1) - 1.0f;
		temp.y = ((temp.y * 2.0f) / scaleY1) - 1.0f;
		temp.z = ((temp.z * 2.0f) / scaleZ1) - 1.0f;

		outvertex1.push_back(temp);
		//glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		//outvertex.push_back(vertex);
	}
	for (unsigned int i = 0; i < uvIndices1.size(); i++) {
		unsigned int uvIndex = uvIndices1[i];
		glm::vec2 vertex = temp_uvs1[uvIndex - 1];
		outuv1.push_back(vertex);
	}
	for (unsigned int i = 0; i < normalIndices1.size(); i++) {
		unsigned int normalIndex = normalIndices1[i];
		glm::vec3 vertex = temp_normals1[normalIndex - 1];
		outnormal1.push_back(vertex);
	}

	return outvertex1.size();
}