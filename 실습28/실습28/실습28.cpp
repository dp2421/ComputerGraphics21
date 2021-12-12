#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

using namespace std;
unsigned int texture[2];

GLvoid drawScene(GLvoid);
GLvoid Reshape(int, int);

void make_vertexShaders();
void make_fragmentShaders();
void make_shaderProgram();
GLchar* vertexsource, * fragmentsource;
GLuint vertexShader, fragmentShader;
GLuint shaderID;
GLuint s_program;
GLvoid InitTexture();
//GLvoid initBuffer();

GLvoid Keyboard(void (*func)(unsigned char key, int x, int y));
GLvoid TimeFunction(int value);

GLint result;
GLchar errorLog[512];

GLchar* filetobuf(const char* file);

GLuint vao[6], vbo[3];

bool checkCube = true;
bool checkTet = false;

bool checkCamRad_x = false;
bool checkCamRad_X = false;

bool checkCamRad_y = false;
bool checkCamRad_Y = false;

GLfloat camRad_X = 0.0f;
GLfloat camRad_Y = 0.0f;

GLfloat cube[][9] = {

	//����
	{   -0.1, 0.1, -0.1,
		-0.1, 0.1, 0.1,
		0.1, 0.1, -0.1   },
	{   0.1, 0.1, -0.1,
		-0.1, 0.1, 0.1,
		0.1, 0.1, 0.1   },
};

GLfloat cube2[][9] = {
	//�Ʒ���
	{   -0.1, -0.1, 0.1,
		-0.1, -0.1, -0.1,
		0.1, -0.1, 0.1   },
	{   0.1, -0.1, 0.1,
		-0.1, -0.1, -0.1,
		0.1, -0.1, -0.1   },
};

GLfloat cube3[][9] = {
	//�ո�
	{   -0.1, 0.1, 0.1,
		-0.1, -0.1, 0.1,
		0.1, 0.1, 0.1   },
	{   0.1, 0.1, 0.1,
		-0.1, -0.1, 0.1,
		0.1, -0.1, 0.1   },
};

GLfloat cube4[][9] = {
	//�����ʸ�
	{   0.1, 0.1, 0.1,
		0.1, -0.1, 0.1,
		0.1, 0.1, -0.1   },
	{   0.1, 0.1, -0.1,
		0.1, -0.1, 0.1,
		0.1, -0.1, -0.1   },

};

GLfloat cube5[][9] = {
	//�޸�
	{   0.1, 0.1, -0.1,
		0.1, -0.1, -0.1,
		-0.1, 0.1, -0.1   },
	{   -0.1, 0.1, -0.1,
		0.1, -0.1, -0.1,
		-0.1, -0.1, -0.1   },

};

GLfloat cube6[][9] = {
	//���ʸ�
	{   -0.1, 0.1, -0.1,
		-0.1, -0.1, -0.1,
		-0.1, 0.1, 0.1   },
	{   -0.1, 0.1, 0.1,
		-0.1, -0.1, -0.1,
		-0.1, -0.1, 0.1   }
};

GLfloat cube_color[] = {
	0.466667, 0.533333, 0.6,
	0.466667, 0.533333, 0.6,
	0.466667, 0.533333, 0.6,

	0.466667, 0.533333, 0.6,
	0.466667, 0.533333, 0.6,
	0.466667, 0.533333, 0.6,

	0.466667, 0.533333, 0.6,
	0.466667, 0.533333, 0.6,
	0.466667, 0.533333, 0.6,

	0.466667, 0.533333, 0.6,
	0.466667, 0.533333, 0.6,
	0.466667, 0.533333, 0.6,

	0.466667, 0.533333, 0.6,
	0.466667, 0.533333, 0.6,
	0.466667, 0.533333, 0.6,

	0.466667, 0.533333, 0.6,
	0.466667, 0.533333, 0.6,
	0.466667, 0.533333, 0.6,

	0.466667, 0.533333, 0.6,
	0.466667, 0.533333, 0.6,
	0.466667, 0.533333, 0.6,

	0.466667, 0.533333, 0.6,
	0.466667, 0.533333, 0.6,
	0.466667, 0.533333, 0.6,

	0.466667, 0.533333, 0.6,
	0.466667, 0.533333, 0.6,
	0.466667, 0.533333, 0.6,

	0.466667, 0.533333, 0.6,
	0.466667, 0.533333, 0.6,
	0.466667, 0.533333, 0.6,

	0.466667, 0.533333, 0.6,
	0.466667, 0.533333, 0.6,
	0.466667, 0.533333, 0.6,

	0.466667, 0.533333, 0.6,
	0.466667, 0.533333, 0.6,
	0.466667, 0.533333, 0.6,
};

GLfloat tet[][9] = {
	{-0.2, -0.2, -0.2, -0.2, -0.2, 0.2, 0.2, -0.2, -0.2},
	{0.2, -0.2, -0.2, -0.2, -0.2, 0.2, 0.2, -0.2, 0.2}, //�Ʒ���
	{0.0, 0.2, 0.0, -0.2, -0.2, 0.2, 0.2, -0.2, 0.2}, //�ո�
	{0.0, 0.2, 0.0, 0.2, -0.2, 0.2, 0.2, -0.2, -0.2}, //�����ʸ�
	{0.0, 0.2, 0.0, 0.2, -0.2, -0.2, -0.2, -0.2, -0.2}, //�޸�
	{0.0, 0.2, 0.0, -0.2, -0.2, -0.2, -0.2, -0.2, 0.2} //���ʸ�
};

GLfloat cube_Texture[][6] = {
	{0.0, 1.0,
	0.0, 0.0,
	1.0, 1.0},
	{1.0, 1.0,
	0.0, 0.0,
	1.0, 0.0},
};

GLfloat tet_Texture[][6] = {
	{1.0, 1.0,
	1.0, 0.0,
	0.0, 1.0},
	{1.0, 0.0,
	0.0, 1.0,
	0.0, 0.0}
};

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'c':
		checkCube = true;
		checkTet = false;
		break;
	case 'p':
		checkTet = true;
		checkCube = false;
		break;
	case 'x':
		checkCamRad_x = true;
		checkCamRad_X = false;
		break;
	case 'X':
		checkCamRad_X = true;
		checkCamRad_x = false;
		break;
	case 'y':
		checkCamRad_y = true;
		checkCamRad_Y = false;
		break;
	case 'Y':
		checkCamRad_Y = true;
		checkCamRad_y = false;
		break;
	case 's':
		checkCamRad_x = false;
		checkCamRad_X = false;
		checkCamRad_y = false;
		checkCamRad_Y = false;
		camRad_X = 0.0f;
		camRad_Y = 0.0f;
		break;
	case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

GLvoid TimeFunction(int value)
{
	if (checkCamRad_x)
	{
		camRad_X += 1.0f;
	}

	if (checkCamRad_X)
	{
		camRad_X -= 1.0f;
	}

	if (checkCamRad_y)
	{
		camRad_Y += 1.0f;
	}

	if (checkCamRad_Y)
	{
		camRad_Y -= 1.0f;
	}

	glutPostRedisplay();
	glutTimerFunc(10, TimeFunction, 1);
}

GLvoid Camera()
{
	//ī�޶�============================================================================================================================
	glm::vec3 camerapos = glm::vec3(0.0f, 1.0f, 2.0f);
	glm::vec3 cameradirection = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraup = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(camerapos, cameradirection, cameraup);

	glm::mat4 Mat_camera_rotate_X = glm::mat4(1.0f);
	glm::mat4 Mat_camera_rotate_Y = glm::mat4(1.0f);

	Mat_camera_rotate_X = glm::rotate(glm::mat4(1.0f), glm::radians(camRad_X), glm::vec3(1.0f, 0.0f, 0.0f));
	Mat_camera_rotate_Y = glm::rotate(glm::mat4(1.0f), glm::radians(camRad_Y), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 View = glm::mat4(1.0f);
	View = view * Mat_camera_rotate_Y * Mat_camera_rotate_X;

	GLuint viewlocation = glGetUniformLocation(s_program, "view");
	glUniformMatrix4fv(viewlocation, 1, GL_FALSE, value_ptr(View));
}

GLvoid Projection()
{
	//����===============================================================================================================================
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(60.0f), (float)800.0 / (float)800.0, 0.1f, 100.0f);
	GLuint Projectionlocation = glGetUniformLocation(s_program, "projection");
	glUniformMatrix4fv(Projectionlocation, 1, GL_FALSE, value_ptr(projection));
}

GLvoid Cube()
{
	glm::mat4 Trans = glm::mat4(1.0f);
	glm::mat4 Scale = glm::mat4(1.0f);
	glm::mat4 Rotate2 = glm::mat4(1.0f);

	Trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	Scale = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
	Rotate2 = glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 Mat_Cube = glm::mat4(1.0f);
	Mat_Cube = Rotate2 * Scale * Trans;
	GLuint TransformLocation = glGetUniformLocation(s_program, "transform"); //--- ���ؽ� ���̴����� �𵨸� ��ȯ ��ġ ��������

	glUniformMatrix4fv(TransformLocation, 1, GL_FALSE, glm::value_ptr(Mat_Cube)); //--- modelTransform ������ ��ȯ �� �����ϱ�

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

GLvoid Tet()
{
	glm::mat4 Trans = glm::mat4(1.0f);
	glm::mat4 Scale = glm::mat4(1.0f);
	Trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	Scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	glm::mat4 Mat_Cube = glm::mat4(1.0f);
	Mat_Cube = Scale * Trans;
	GLuint TransformLocation = glGetUniformLocation(s_program, "transform"); //--- ���ؽ� ���̴����� �𵨸� ��ȯ ��ġ ��������

	glUniformMatrix4fv(TransformLocation, 1, GL_FALSE, glm::value_ptr(Mat_Cube)); //--- modelTransform ������ ��ȯ �� �����ϱ�

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tet), tet, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_color), cube_color, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

GLvoid InitTexture()
{
	BITMAP* bmp;

	int width[6], height[6], nrChannels[6];

	glGenTextures(1, &texture[0]); //--- �ؽ�ó ����

	glBindTexture(GL_TEXTURE_2D, texture[0]); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load("1.jpg", &width[0], &height[0], &nrChannels[0], 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width[0], height[0], 0, GL_RGB, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ����

	glUseProgram(shaderID);
	int tLocation = glGetUniformLocation(s_program, "outTexture"); //--- outTexture ������ ���÷��� ��ġ�� ������
	glUniform1i(tLocation, 0); //--- ���÷��� 0�� �������� ����
	stbi_image_free(data);

	glGenTextures(1, &texture[1]); //--- �ؽ�ó ����

	glBindTexture(GL_TEXTURE_2D, texture[1]); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data2 = stbi_load("2.jpg", &width[1], &height[1], &nrChannels[1], 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width[1], height[1], 0, GL_RGB, GL_UNSIGNED_BYTE, data2); //---�ؽ�ó �̹��� ����

	glUseProgram(shaderID);
	tLocation = glGetUniformLocation(s_program, "outTexture"); //--- outTexture ������ ���÷��� ��ġ�� ������
	glUniform1i(tLocation, 0); //--- ���÷��� 0�� �������� ����
	stbi_image_free(data2);

	glGenTextures(1, &texture[2]); //--- �ؽ�ó ����

	glBindTexture(GL_TEXTURE_2D, texture[2]); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data3 = stbi_load("3.jpg", &width[2], &height[2], &nrChannels[2], 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width[2], height[2], 0, GL_RGB, GL_UNSIGNED_BYTE, data3); //---�ؽ�ó �̹��� ����

	glUseProgram(shaderID);
	tLocation = glGetUniformLocation(s_program, "outTexture"); //--- outTexture ������ ���÷��� ��ġ�� ������
	glUniform1i(tLocation, 0); //--- ���÷��� 0�� �������� ����
	stbi_image_free(data3);

	glGenTextures(1, &texture[3]); //--- �ؽ�ó ����

	glBindTexture(GL_TEXTURE_2D, texture[3]); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data4 = stbi_load("4.jpg", &width[3], &height[3], &nrChannels[3], 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width[3], height[3], 0, GL_RGB, GL_UNSIGNED_BYTE, data4); //---�ؽ�ó �̹��� ����

	glUseProgram(shaderID);
	tLocation = glGetUniformLocation(s_program, "outTexture"); //--- outTexture ������ ���÷��� ��ġ�� ������
	glUniform1i(tLocation, 0); //--- ���÷��� 0�� �������� ����
	stbi_image_free(data4);

	glGenTextures(1, &texture[4]); //--- �ؽ�ó ����

	glBindTexture(GL_TEXTURE_2D, texture[4]); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data5 = stbi_load("5.jpg", &width[4], &height[4], &nrChannels[4], 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width[4], height[4], 0, GL_RGB, GL_UNSIGNED_BYTE, data5); //---�ؽ�ó �̹��� ����

	glUseProgram(shaderID);
	tLocation = glGetUniformLocation(s_program, "outTexture"); //--- outTexture ������ ���÷��� ��ġ�� ������
	glUniform1i(tLocation, 0); //--- ���÷��� 0�� �������� ����
	stbi_image_free(data5);

	glGenTextures(1, &texture[5]); //--- �ؽ�ó ����

	glBindTexture(GL_TEXTURE_2D, texture[5]); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data6 = stbi_load("6.jpg", &width[5], &height[5], &nrChannels[5], 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width[5], height[5], 0, GL_RGB, GL_UNSIGNED_BYTE, data6); //---�ؽ�ó �̹��� ����

	glUseProgram(shaderID);
	tLocation = glGetUniformLocation(s_program, "outTexture"); //--- outTexture ������ ���÷��� ��ġ�� ������
	glUniform1i(tLocation, 0); //--- ���÷��� 0�� �������� ����
	stbi_image_free(data6);
}

GLvoid initBuffer()
{
	glGenVertexArrays(6, vao);

	//1============================================================================================
	glBindVertexArray(vao[0]);

	glGenBuffers(1, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_color), cube_color, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);	//ù ��° ����: �ε���
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_Texture), cube_Texture, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(2);

	//2============================================================================================
	glBindVertexArray(vao[1]);

	glGenBuffers(1, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube2), cube2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_color), cube_color, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);	//ù ��° ����: �ε���
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_Texture), cube_Texture, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(2);

	//3============================================================================================
	glBindVertexArray(vao[2]);

	glGenBuffers(1, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube3), cube3, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_color), cube_color, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);	//ù ��° ����: �ε���
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_Texture), cube_Texture, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(2);

	//4============================================================================================
	glBindVertexArray(vao[3]);

	glGenBuffers(1, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube4), cube4, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_color), cube_color, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);	//ù ��° ����: �ε���
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_Texture), cube_Texture, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(2);

	//5============================================================================================
	glBindVertexArray(vao[4]);

	glGenBuffers(1, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube5), cube5, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_color), cube_color, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);	//ù ��° ����: �ε���
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_Texture), cube_Texture, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(2);

	//6============================================================================================
	glBindVertexArray(vao[5]);

	glGenBuffers(1, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube6), cube6, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_color), cube_color, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);	//ù ��° ����: �ε���
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_Texture), cube_Texture, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(2);
}

GLvoid CubeTexture()
{
	Cube();
	glBindVertexArray(vao[0]); //--- ù ��° ������
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]); //--- texture[0]�� ����Ͽ� �������� �׸���.
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(vao[1]); //--- �� ��° ������
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[1]); //--- texture[1]�� ����Ͽ� �������� �׸���.
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(vao[2]); //--- �� ��° ������
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[2]); //--- texture[1]�� ����Ͽ� �������� �׸���.
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(vao[3]); //--- �� ��° ������
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[3]); //--- texture[1]�� ����Ͽ� �������� �׸���.
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(vao[4]); //--- �� ��° ������
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[4]); //--- texture[1]�� ����Ͽ� �������� �׸���.
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(vao[5]); //--- �� ��° ������
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[5]); //--- texture[1]�� ����Ͽ� �������� �׸���.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawScene() //--- glutDisplayFunc()�Լ��� ����� �׸��� �ݹ� �Լ�
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(s_program);

	Camera();
	Projection();
	if (checkCube)
	{
		Cube();
		//CubeTexture();
		checkTet = false;
	}
	if (checkTet)
	{
		Tet();
		checkCube = false;
	}

	glutPostRedisplay();
	glutSwapBuffers();
}

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 800);
	glutCreateWindow("�ǽ�21");

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();
	make_shaderProgram();

	glEnable(GL_DEPTH_TEST);
	glGenVertexArrays(1, &vao[5]); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glBindVertexArray(vao[5]); //--- VAO�� ���ε��ϱ�
	glGenBuffers(3, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[5]);

	//initBuffer();
	InitTexture();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(10, TimeFunction, 1);

	glutMainLoop();
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}

void make_vertexShaders()
{
	vertexsource = filetobuf("vertex.glsl");

	//--- ���ؽ� ���̴� ��ü �����
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexsource, 0);

	//--- ���ؽ� ���̴� �������ϱ�
	glCompileShader(vertexShader);

	//--- �������� ����� ���� ���� ���: ���� üũ

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader ������ ����\n" << errorLog << endl;
		return exit(-1);
	}
}

void make_fragmentShaders()
{
	fragmentsource = filetobuf("fragment.glsl");
	//--- �����׸�Ʈ ���̴� ��ü �����
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentsource, 0);
	//--- �����׸�Ʈ ���̴� ������
	glCompileShader(fragmentShader);
	//--- �������� ����� ���� ���� ���: ������ ���� üũ

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader ������ ����\n" << errorLog << endl;
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
		cerr << "Error : shader program ���� ���� \n" << errorLog << endl;
		exit(-1);
	}

	glUseProgram(s_program);
}

GLchar* filetobuf(const char* file) {
	long length;
	char* buf;

	FILE* fptr = fopen(file, "rb");
	if (fptr == NULL) {
		cout << "���� ���� ����" << endl;
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