#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <random> 
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
//#include <gl/glm/glm.hpp>
//#include <gl/glm/ext.hpp>
//#include <gl/glm/gtc/matrix_transform.hpp> //수현
#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
#include <glm/glm/gtc/matrix_transform.hpp> //예나


using namespace std;
unsigned int texture[8];

GLvoid drawScene(GLvoid);
GLvoid Reshape(int, int);
GLvoid GroundInitBuffer();
GLvoid PlayerInitBuffer();
GLvoid EndInitBuffer();
GLvoid CarInitBuffer();
GLvoid InitTexture();

void make_vertexShaders();
void make_fragmentShaders();
void make_shaderProgram();
GLchar* vertexsource, * fragmentsource;
GLuint vertexShader, fragmentShader;
GLuint shaderID;
GLuint s_program;

GLvoid Keyboard(void (*func)(unsigned char key, int x, int y));
GLvoid TimeFunction(int value);

GLint result;
GLchar errorLog[512];

GLchar* filetobuf(const char* file);

GLuint vao[5], vbo[3], ebo;

//육면체 위치 변수
GLfloat CubePosX = 0.0f;
GLfloat CubePosZ = 0.f;

//카메라 위치 변수
GLfloat CamPosX = 2.0f;
GLfloat CamPosY = 3.0f;
GLfloat CamPosZ = 4.0f;

//카메라가 보는 방향 변수
GLfloat CamDirX = CubePosX;
GLfloat CamDirZ = CubePosZ - 10.0f;

//자동차 움직임
glm::mat4 Carmoving = glm::mat4(1.f);
glm::mat4 Carmoving2 = glm::mat4(1.f);
glm::mat4 Carmoving3 = glm::mat4(1.f);


//충돌 bool값
bool checkCrash1 = false;
bool checkCrash2 = false;

//스테이지 종료 bool값
bool checkStage1 = true;
bool checkStage2 = false;

//스테이지 완료 화면 테스트 bool값
bool checkTest = false;

//맵(1이 장애물이 있는 칸, 0은 없는 칸)
int map1[30] = { 0,0,0,0,1,1,1,0,0,1,1,0,0,0,1,0,0,0,1,1,0,0,0,0,1,1,1,0,0,0 };
int map2[30] = { 0,0,0,1,1,1,0,0,1,1,1,0,0,0,1,1,0,1,0,1,1,1,0,0,1,1,1,0,0,0 };

int loadObj_normalize_center(const char* filename, int j);
struct OBJ {
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
};

OBJ obj[2];

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

GLfloat ending[][3] = {
	-1, -1, 0,
	1, -1, 0,
	-1, 1, 0,
	1, 1, 0,
};
GLuint ground_element[] = { 2, 0, 1, 2, 1, 3, };


GLfloat ground_Texture[] = {
	0, 0.0, 0,
	1, 0.0, 0,
	0, 0.0, 1,
	1, 0.0, 1,
};

GLfloat end_Texture[] = {
	0, 0.0, 0,
	1, 0.0, 0,
	0, 1, 0,
	1, 1, 0,
};

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		CubePosZ -= 2.f;
		CamDirZ -= 2.f;
		CamPosZ -= 2.f;
		break;
	case 'a':
		CubePosX -= 2.f;
		break;
	case 's':
		CubePosZ += 2.f;
		CamDirZ += 2.f;
		CamPosZ += 2.f;
		break;
	case 'd':
		CubePosX += 2.f;
		break;
	case 'x':
		CamPosX += 1.0f;
		CamDirX += 1.f;
		break;
	case 'y':
		CamPosY += 1.0f;
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
		if (checkStage1)
		{
			CubePosX = 0.0f;
			CubePosZ = 0.0f;

			CamPosX = 2.0f;
			CamPosY = 3.0f;
			CamPosZ = 4.0f;
		}
		if (checkStage2)
		{
			CubePosZ = -60.0f;
			CamPosX = 2.0f;
			CamPosY = 5.0f;
			CamPosZ = CubePosZ + 8.f;
		}
		break;
	case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

float cnt[3] = { -30.f,-30.f,-30.f };	//도로의 끝인지, 체크하는 변수
GLfloat CarPosZ;
GLvoid TimeFunction(int value)
{
	if (cnt[0] >= 30.f) {
		cnt[0] = -30.f;
		Carmoving = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0));
	}
	else if (cnt[1] >= 30.f) {
		cnt[1] = -30.f;
		Carmoving2 = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0));
	}
	else if (cnt[2] >= 30.f) {
		cnt[2] = -30.f;
		Carmoving3 = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0));
	}
	else {
		Carmoving = glm::translate(Carmoving, glm::vec3(0.1, 0, 0));
		Carmoving2 = glm::translate(Carmoving2, glm::vec3(0.16, 0, 0));
		Carmoving3 = glm::translate(Carmoving3, glm::vec3(0.2, 0, 0));
		cnt[0] += 0.1;
		cnt[1] += 0.16;
		cnt[2] += 0.2;
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

GLvoid Ground(int i)
{
	//바닥 그리기=========================================================================================================================
	glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(30.0f, 2.0f, 30.0f));
	glm::mat4 Move = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -30.f));
	glm::mat4 Rotate = glm::mat4(1.0f);
	Rotate = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 SR = glm::mat4(1.0f);
	SR = Move * Rotate * Scale;
	unsigned int TextureLocation = glGetUniformLocation(s_program, "outTexture");
	glUniform1i(TextureLocation, 0);
	unsigned int TransformLocation = glGetUniformLocation(s_program, "modelTransForm");
	glUniformMatrix4fv(TransformLocation, 1, GL_FALSE, glm::value_ptr(SR));
	glBindVertexArray(vao[0]);
	glActiveTexture(GL_TEXTURE0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindTexture(GL_TEXTURE_2D, texture[i]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

GLvoid Player()
{
	glm::mat4 Rot;
	if (checkStage2 == true)
		Rot = glm::rotate(glm::mat4(1.f), glm::radians(0.f), glm::vec3(0, 1, 0));
	else
		Rot = glm::rotate(glm::mat4(1.f), glm::radians(180.f), glm::vec3(0, 1, 0));
	glm::mat4 Scale = glm::scale(glm::mat4(1.f), glm::vec3(0.7, 0.7, 0.7));
	glm::mat4 Trans = glm::translate(glm::mat4(1.0f), glm::vec3(CubePosX, 0.7f, CubePosZ));
	glm::mat4 Mat_Cube = Trans * Rot * Scale;

	GLuint TransformLocation = glGetUniformLocation(s_program, "modelTransForm");
	glUniformMatrix4fv(TransformLocation, 1, GL_FALSE, glm::value_ptr(Mat_Cube));
	glBindVertexArray(vao[2]);
	glActiveTexture(GL_TEXTURE0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glDrawArrays(GL_TRIANGLES, 0, obj[0].num_Triangle);
}

GLvoid Car1()
{
	int mapcnt = 0;
	glm::mat4 Trans = glm::mat4(1.f);
	for (int i = 0; i < 30; i++)
	{
		//자동차 위치 변수
		CarPosZ = -2 * i;
		if (map1[i] == 1)
		{
			glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
			glm::mat4 BottomScale = glm::scale(glm::mat4(1.f), glm::vec3(2.f, 1.f, 1.f));
			glm::mat4 UpperTrans = glm::translate(glm::mat4(1.f), glm::vec3(0, 0.7, 0));
			glm::mat4 Mat_Car = glm::mat4(1.f);
			Trans = glm::translate(glm::mat4(1.0f), glm::vec3(-30.0f, 0.9f, CarPosZ));
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
			GLuint TransformLocation = glGetUniformLocation(s_program, "modelTransForm");
			glUniformMatrix4fv(TransformLocation, 1, GL_FALSE, glm::value_ptr(Mat_Car));
			glBindVertexArray(vao[3]);
			glActiveTexture(GL_TEXTURE0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBindTexture(GL_TEXTURE_2D, texture[3]);
			glDrawArrays(GL_TRIANGLES, 0, obj[1].num_Triangle);

			if (checkCrash1 == false)
			{
				if (Collide(getbb_player(CubePosX, CubePosZ), getbb_car(cnt[0], CarPosZ)) == true)
				{
					CubePosX = 0.0f;
					CubePosZ = 0.0f;
					CamPosX = 2.0f;
					CamPosY = 3.0f;
					CamPosZ = CubePosZ + 4.f;
					CamDirZ = CubePosZ - 7.f;
				}
			}

			if (CubePosZ < -60.f)
			{
				checkStage2 = true; //스테이지2 시작
				checkStage1 = false; //충돌 시 스테이지1 자동차 사라짐
				CamPosX = 2.0f;
				CamPosY = 5.0f;
				CamPosZ = CubePosZ + 8.f;
				CamDirZ = CubePosZ - 7.f;	//카메라 조정(반대편)
			}
			mapcnt++;
		}
	}
}

GLvoid Car2()
{
	int mapcnt = 0;
	glm::mat4 Trans = glm::mat4(1.f);
	for (int i = 0; i < 30; i++)
	{
		//자동차 위치 변수
		CarPosZ = -2 * i;
		if (map2[i] == 1)
		{
			glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.f, 0.7f, 1.f));
			glm::mat4 BottomScale = glm::scale(glm::mat4(1.f), glm::vec3(2.f, 1.f, 1.f));
			glm::mat4 UpperTrans = glm::translate(glm::mat4(1.f), glm::vec3(0, 0.7, 0));
			glm::mat4 Mat_Car = glm::mat4(1.f);
			Trans = glm::translate(glm::mat4(1.0f), glm::vec3(-30.0f, 0.9f, CarPosZ));
			switch (mapcnt)
			{
			case 0:
			case 3:
			case 5:
			case 8:
				Mat_Car = Carmoving3 * 2.0f * Trans;
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
			GLuint TransformLocation = glGetUniformLocation(s_program, "modelTransForm");
			glUniformMatrix4fv(TransformLocation, 1, GL_FALSE, glm::value_ptr(Mat_Car));
			glBindVertexArray(vao[3]);
			glActiveTexture(GL_TEXTURE0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBindTexture(GL_TEXTURE_2D, texture[4]);
			glDrawArrays(GL_TRIANGLES, 0, obj[1].num_Triangle);

			if (checkCrash2 == false)
			{
				if (Collide(getbb_player(CubePosX, CubePosZ), getbb_car(cnt[0], CarPosZ)) == true)
				{
					CubePosX = 0.0f;
					CubePosZ = -60.0f;
					CamPosX = 2.0f;
					CamPosY = 3.0f;
					CamPosZ = CubePosZ + 4.f;
					CamDirZ = CubePosZ + 7.f;
				}
			}

			if (CubePosZ >= 0)
				checkTest = true;

			mapcnt++;
		}
	}
}

GLvoid BackGround(int i)
{
	glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, 50.0f, 1.0f));
	glm::mat4 Move = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 0.0f, -70.0f));
	glm::mat4 SR = glm::mat4(1.0f);
	SR = Move * Scale;
	unsigned int TextureLocation = glGetUniformLocation(s_program, "outTexture");
	glUniform1i(TextureLocation, 0);
	unsigned int TransformLocation = glGetUniformLocation(s_program, "modelTransForm");
	glUniformMatrix4fv(TransformLocation, 1, GL_FALSE, glm::value_ptr(SR));
	glBindVertexArray(vao[4]);
	glActiveTexture(GL_TEXTURE0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindTexture(GL_TEXTURE_2D, texture[i]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

GLvoid BackGroundL(int i)
{
	glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, 50.0f, 1.0f));
	glm::mat4 Move = glm::translate(glm::mat4(1.f), glm::vec3(-50.0f, 0.0f, -50.0f));
	glm::mat4 Rotate = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 SR = glm::mat4(1.0f);
	SR = Move * Rotate * Scale;
	unsigned int TextureLocation = glGetUniformLocation(s_program, "outTexture");
	glUniform1i(TextureLocation, 0);
	unsigned int TransformLocation = glGetUniformLocation(s_program, "modelTransForm");
	glUniformMatrix4fv(TransformLocation, 1, GL_FALSE, glm::value_ptr(SR));
	glBindVertexArray(vao[4]);
	glActiveTexture(GL_TEXTURE0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindTexture(GL_TEXTURE_2D, texture[i]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

GLvoid BackGroundR(int i)
{
	glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, 50.0f, 1.0f));
	glm::mat4 Move = glm::translate(glm::mat4(1.f), glm::vec3(50.0f, 0.0f, -50.0f));
	glm::mat4 Rotate = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 SR = glm::mat4(1.0f);
	SR = Move * Rotate * Scale;
	unsigned int TextureLocation = glGetUniformLocation(s_program, "outTexture");
	glUniform1i(TextureLocation, 0);
	unsigned int TransformLocation = glGetUniformLocation(s_program, "modelTransForm");
	glUniformMatrix4fv(TransformLocation, 1, GL_FALSE, glm::value_ptr(SR));
	glBindVertexArray(vao[4]);
	glActiveTexture(GL_TEXTURE0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindTexture(GL_TEXTURE_2D, texture[i]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

GLvoid BackGroundBottom(int i)
{
	glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, 1.0f, 50.0f));
	glm::mat4 Move = glm::translate(glm::mat4(1.f), glm::vec3(0, -20.0f, -30.f));
	glm::mat4 Rotate = glm::mat4(1.0f);
	Rotate = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 SR = glm::mat4(1.0f);
	SR = Move * Rotate * Scale;
	unsigned int TextureLocation = glGetUniformLocation(s_program, "outTexture");
	glUniform1i(TextureLocation, 0);
	unsigned int TransformLocation = glGetUniformLocation(s_program, "modelTransForm");
	glUniformMatrix4fv(TransformLocation, 1, GL_FALSE, glm::value_ptr(SR));
	glBindVertexArray(vao[0]);
	glActiveTexture(GL_TEXTURE0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindTexture(GL_TEXTURE_2D, texture[i]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

GLvoid StageClear(int i)
{
	glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(20.f, 20.f, 20.f));
	glm::mat4 Move = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 0.0f, 6.0f));
	glm::mat4 SR = glm::mat4(1.0f);
	SR = Move * Scale;
	unsigned int TextureLocation = glGetUniformLocation(s_program, "outTexture");
	glUniform1i(TextureLocation, 0);
	unsigned int TransformLocation = glGetUniformLocation(s_program, "modelTransForm"); 
	glUniformMatrix4fv(TransformLocation, 1, GL_FALSE, glm::value_ptr(SR));
	glBindVertexArray(vao[4]);
	glActiveTexture(GL_TEXTURE0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindTexture(GL_TEXTURE_2D, texture[i]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

GLvoid Light()
{
	int lightColorLocation = glGetUniformLocation(s_program, "lightColor");
	int lightPosLocation = glGetUniformLocation(s_program, "lightPos");

	if (checkStage2 == true)
		glUniform3f(lightColorLocation, 1, 0.72, 0.57);
	else
		glUniform3f(lightColorLocation, 0.94, 0.97, 1);

	if (checkTest == true)
	{
		glUniform3f(lightPosLocation, 0, 0, 100);
		glUniform3f(lightColorLocation, 1, 1, 1);
	}
	else
		glUniform3f(lightPosLocation, 0, 10, -60);
}

GLvoid InitTexture()
{
	BITMAP* bmp;

	int width[8], height[8], nrChannels[8];
	unsigned char* data[8];
	stbi_set_flip_vertically_on_load(true);

	glGenTextures(8, texture);

	for (int i = 0; i < 8; i++)
	{
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		switch (i)
		{
		case 0:
			data[i] = stbi_load("stage1.png", &width[i], &height[i], &nrChannels[i], 0);
			break;
		case 1:
			data[i] = stbi_load("stage2.png", &width[i], &height[i], &nrChannels[i], 0);
			break;
		case 2:
			data[i] = stbi_load("yellow1.jpg", &width[i], &height[i], &nrChannels[i], 0);
			break;
		case 3:
			data[i] = stbi_load("blue.jpg", &width[i], &height[i], &nrChannels[i], 0);
			break;
		case 4:
			data[i] = stbi_load("green.jpg", &width[i], &height[i], &nrChannels[i], 0);
			break;
		case 5:
			data[i] = stbi_load("end1.jpg", &width[i], &height[i], &nrChannels[i], 0);
			break;
		case 6:
			data[i] = stbi_load("sky.jpg", &width[i], &height[i], &nrChannels[i], 0);
			break;
		case 7:
			data[i] = stbi_load("sky2.jpg", &width[i], &height[i], &nrChannels[i], 0);
			break;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, 3, width[i], height[i], 0, GL_RGB, GL_UNSIGNED_BYTE, data[i]);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data[i]);
	}
}

void drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(s_program);
	Light();
	Camera(); //카메라
	Projection(); //투영

	if (checkTest)
	{
		StageClear(5);
		CamPosY = 0.f;
		CamPosX = 0.0f;
		CamPosZ = 40.0f;
		CamDirX = 0.0f;
		CamDirZ = 10.0f;
	}
	if (checkStage1)
	{
		BackGround(6);
		BackGroundL(6);
		BackGroundR(6);
		BackGroundBottom(6);
		if (checkCrash1 == false)
		{
			CamDirZ = CamPosZ - 10.0f;
			Player(); //객체 그리기
		}
		Ground(0);
		Car1();
	}
	if (checkStage2)
	{
		BackGround(7);
		BackGroundL(7);
		BackGroundR(7);
		BackGroundBottom(7);
		if (checkCrash2 == false)
		{
			CamDirZ = CamPosZ - 5.0f;
			Player();
		}
		Ground(1);
		Car2();
	}
	//glFrontFace(GL_CCW);
	glutPostRedisplay();
	glutSwapBuffers();
}

void main(int argc, char** argv)
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 800);
	glutCreateWindow("길건너 아이들");

	glewExperimental = GL_TRUE;
	glewInit();
	make_shaderProgram();
	InitTexture();
	GroundInitBuffer();
	PlayerInitBuffer();
	CarInitBuffer();
	EndInitBuffer();

	glEnable(GL_DEPTH_TEST);

	glGenBuffers(3, vbo);
	glActiveTexture(GL_TEXTURE0);

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(10, TimeFunction, 1);
	glutMainLoop();
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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &vbo[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground_Texture), ground_Texture, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ground_element), ground_element, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
}

GLvoid EndInitBuffer()
{
	glGenVertexArrays(1, &vao[4]);
	glBindVertexArray(vao[4]);

	glGenBuffers(1, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ending), ending, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ending), ending, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &vbo[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(end_Texture), end_Texture, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ground_element), ground_element, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
}

GLvoid PlayerInitBuffer()
{
	obj[0].num_Triangle = loadObj_normalize_center("child.obj", 0);
	glGenVertexArrays(2, &vao[2]);
	glGenBuffers(2, &vbo[0]);
	glGenBuffers(2, &vbo[1]);
	glGenBuffers(2, &vbo[2]);

	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, obj[0].outvertex.size() * sizeof(glm::vec3), &obj[0].outvertex[0], GL_STATIC_DRAW);
	GLint pAttribute = glGetAttribLocation(s_program, "vPos");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, obj[0].outnormal.size() * sizeof(glm::vec3), &obj[0].outnormal[0], GL_STATIC_DRAW);
	GLint nAttribute = glGetAttribLocation(s_program, "vNormal");
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, obj[0].outnormal.size() * sizeof(glm::vec3), &obj[0].outuv[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(2);

}

GLvoid CarInitBuffer()
{
	obj[1].num_Triangle = loadObj_normalize_center("car.obj", 1);
	glGenVertexArrays(2, &vao[3]);
	glGenBuffers(2, &vbo[0]);
	glGenBuffers(2, &vbo[1]);
	glGenBuffers(2, &vbo[2]);

	glBindVertexArray(vao[3]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, obj[1].outvertex.size() * sizeof(glm::vec3), &obj[1].outvertex[0], GL_STATIC_DRAW);
	GLint pAttribute = glGetAttribLocation(s_program, "vPos");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, obj[1].outnormal.size() * sizeof(glm::vec3), &obj[1].outnormal[0], GL_STATIC_DRAW);
	GLint nAttribute = glGetAttribLocation(s_program, "vNormal");
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, obj[1].outnormal.size() * sizeof(glm::vec3), &obj[1].outuv[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(2);
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

void make_vertexShaders()
{
	vertexsource = filetobuf("lightvertex.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, (const GLchar**)&vertexsource, 0);
	glCompileShader(vertexShader);

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
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentsource, 0);
	glCompileShader(fragmentShader);

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

int loadObj_normalize_center(const char* filename, int j)
{
	FILE* objFile;

	fopen_s(&objFile, filename, "rb");

	if (objFile == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	while (1) {

		char lineHeader[128];
		int res = fscanf(objFile, "%s", lineHeader);
		if (res == EOF)
			break;
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(objFile, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

			if (vertex.x < obj[j].minX) obj[j].minX = vertex.x;
			if (vertex.y < obj[j].minY) obj[j].minY = vertex.y;
			if (vertex.z < obj[j].minZ) obj[j].minZ = vertex.z;
			if (vertex.x > obj[j].maxX) obj[j].maxX = vertex.x;
			if (vertex.y > obj[j].maxY) obj[j].maxY = vertex.y;
			if (vertex.z > obj[j].maxZ) obj[j].maxZ = vertex.z;
			obj[j].sumX += vertex.x;
			obj[j].sumY += vertex.y;
			obj[j].sumZ += vertex.z;

			obj[j].temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(objFile, "%f %f\n", &uv.x, &uv.y);
			obj[j].temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(objFile, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			obj[j].temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(objFile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			obj[j].vertexIndices.push_back(vertexIndex[0]);
			obj[j].vertexIndices.push_back(vertexIndex[1]);
			obj[j].vertexIndices.push_back(vertexIndex[2]);
			obj[j].uvIndices.push_back(uvIndex[0]);
			obj[j].uvIndices.push_back(uvIndex[1]);
			obj[j].uvIndices.push_back(uvIndex[2]);
			obj[j].normalIndices.push_back(normalIndex[0]);
			obj[j].normalIndices.push_back(normalIndex[1]);
			obj[j].normalIndices.push_back(normalIndex[2]);
		}
	}

	obj[j].aveX = obj[j].sumX / obj[j].vertexIndices.size();
	obj[j].aveY = obj[j].sumY / obj[j].vertexIndices.size();
	obj[j].aveZ = obj[j].sumZ / obj[j].vertexIndices.size();
	obj[j].scaleX = obj[j].maxX - obj[j].minX;
	obj[j].scaleY = obj[j].maxY - obj[j].minY;
	obj[j].scaleZ = obj[j].maxZ - obj[j].minZ;

	glm::vec3 temp;

	for (unsigned int i = 0; i < obj[j].vertexIndices.size(); i++) {
		unsigned int vertexIndex = obj[j].vertexIndices[i];
		temp = obj[j].temp_vertices[vertexIndex - 1];
		temp.x = temp.x - obj[j].minX;
		temp.y = temp.y - obj[j].minY;
		temp.z = temp.z - obj[j].minZ;

		temp.x = ((temp.x * 2.0f) / obj[j].scaleX) - 1.0f;
		temp.y = ((temp.y * 2.0f) / obj[j].scaleY) - 1.0f;
		temp.z = ((temp.z * 2.0f) / obj[j].scaleZ) - 1.0f;

		obj[j].outvertex.push_back(temp);
	}
	for (unsigned int i = 0; i < obj[j].uvIndices.size(); i++) {
		unsigned int uvIndex = obj[j].uvIndices[i];
		glm::vec2 vertex = obj[j].temp_uvs[uvIndex - 1];
		obj[j].outuv.push_back(vertex);
	}
	for (unsigned int i = 0; i < obj[j].normalIndices.size(); i++) {
		unsigned int normalIndex = obj[j].normalIndices[i];
		glm::vec3 vertex = obj[j].temp_normals[normalIndex - 1];
		obj[j].outnormal.push_back(vertex);
	}
	return obj[j].outvertex.size();
}