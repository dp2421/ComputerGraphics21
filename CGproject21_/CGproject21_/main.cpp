#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
//#include <glm/glm/glm.hpp>
//#include <glm/glm/ext.hpp>
//#include <glm/glm/gtc/matrix_transform.hpp>

////////�ʶ� ���� gl��ΰ� �޶� ���� ���� �� �ּ�ó���ϰ� ����ϴ� �ɷ� ����!/////////

//���� �˰Խ�!!

using namespace std;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int, int);
GLvoid CubeInitBuffer();
GLvoid GroudInitBuffer();
GLvoid InitShader();

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

GLuint vao, vbo[2];

//����ü ��ġ ����
GLfloat CubePosX;
GLfloat CubePosZ = 45.0f;

//ī�޶� ��ġ ����
GLfloat CamPosX = 2.0f;
GLfloat CamPosY = 3.0f;
GLfloat CamPosZ = 50.0f;

//ī�޶� ���� ���� ����
GLfloat CamDirX = CubePosX;
GLfloat CamDirZ = CubePosZ - 7.0f;

//wasd check
bool checkW = false;
bool checkA = false;
bool checkS = false;
bool checkD = false;

GLfloat ground[][9] = {
	{   -1.0f, 0.0f, -1.0f,
		-1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, -1.0f   },

	{   1.0f, 0.0f, -1.0f,
		-1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f    }
};

GLfloat ground_color[] = { //Misty Rose
	0.690196, 0.768627, 0.870588,
	0.690196, 0.768627, 0.870588,
	0.690196, 0.768627, 0.870588,

	0.690196, 0.768627, 0.870588,
	0.690196, 0.768627, 0.870588,
	0.690196, 0.768627, 0.870588,

	0.690196, 0.768627, 0.870588,
	0.690196, 0.768627, 0.870588,
	0.690196, 0.768627, 0.870588,

	0.690196, 0.768627, 0.870588,
	0.690196, 0.768627, 0.870588,
	0.690196, 0.768627, 0.870588,

	0.690196, 0.768627, 0.870588,
	0.690196, 0.768627, 0.870588,
	0.690196, 0.768627, 0.870588,

	0.690196, 0.768627, 0.870588,
	0.690196, 0.768627, 0.870588,
	0.690196, 0.768627, 0.870588,

	0.690196, 0.768627, 0.870588,
	0.690196, 0.768627, 0.870588,
	0.690196, 0.768627, 0.870588,

	0.690196, 0.768627, 0.870588,
	0.690196, 0.768627, 0.870588,
	0.690196, 0.768627, 0.870588,

	0.690196, 0.768627, 0.870588,
	0.690196, 0.768627, 0.870588,
	0.690196, 0.768627, 0.870588,

	0.690196, 0.768627, 0.870588,
	0.690196, 0.768627, 0.870588,
	0.690196, 0.768627, 0.870588,

	0.690196, 0.768627, 0.870588,
	0.690196, 0.768627, 0.870588,
	0.690196, 0.768627, 0.870588,

	0.690196, 0.768627, 0.870588,
	0.690196, 0.768627, 0.870588,
	0.690196, 0.768627, 0.870588,
};

GLfloat cube[][9] = {

	//����
	{   -0.1, 0.1, -0.1,
		-0.1, 0.1, 0.1,
		0.1, 0.1, -0.1   },
	{   0.1, 0.1, -0.1,
		-0.1, 0.1, 0.1,
		0.1, 0.1, 0.1   },
		//�Ʒ���
		{   -0.1, -0.1, 0.1,
			-0.1, -0.1, -0.1,
			0.1, -0.1, 0.1   },
		{   0.1, -0.1, 0.1,
			-0.1, -0.1, -0.1,
			0.1, -0.1, -0.1   },
			//�ո�
			{   -0.1, 0.1, 0.1,
				-0.1, -0.1, 0.1,
				0.1, 0.1, 0.1   },
			{   0.1, 0.1, 0.1,
				-0.1, -0.1, 0.1,
				0.1, -0.1, 0.1   },
				//�����ʸ�
				{   0.1, 0.1, 0.1,
					0.1, -0.1, 0.1,
					0.1, 0.1, -0.1   },
				{   0.1, 0.1, -0.1,
					0.1, -0.1, 0.1,
					0.1, -0.1, -0.1   },
					//�޸�
					{   0.1, 0.1, -0.1,
						0.1, -0.1, -0.1,
						-0.1, 0.1, -0.1   },
					{   -0.1, 0.1, -0.1,
						0.1, -0.1, -0.1,
						-0.1, -0.1, -0.1   },
						//���ʸ�
						{   -0.1, 0.1, -0.1,
							-0.1, -0.1, -0.1,
							-0.1, 0.1, 0.1   },
						{   -0.1, 0.1, 0.1,
							-0.1, -0.1, -0.1,
							-0.1, -0.1, 0.1   }
};

GLfloat cube_color[] = { //Light Pink
	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,

	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,

	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,

	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,

	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,

	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,

	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,

	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,

	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,

	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,

	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,

	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,
};

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		CubePosZ -= 0.2f;
		CamDirZ -= 0.2f;
		CamPosZ -= 0.2f;
		checkW = true;
		checkA = false;
		checkS = false;
		checkD = false;
		break;
	case 'a':
		CubePosX -= 0.2f;
		checkW = false;
		checkA = true;
		checkS = false;
		checkD = false;
		break;
	case 's':
		CubePosZ += 0.2f;
		CamDirZ += 0.2f;
		CamPosZ += 0.2f;
		cout << CubePosZ << endl;
		checkW = false;
		checkA = false;
		checkS = true;
		checkD = false;
		break;
	case 'd':
		CubePosX += 0.2f;
		checkW = false;
		checkA = false;
		checkS = false;
		checkD = true;
		break;
		/////////////////////////////ī�޶� �̵��� �� �ִ°� ¥�� ���� �� ���Ƽ� x,y,z�� �̵��� �� �ְ� �س���!!!///////////////////////
	case 'x':
		CamPosX += 1.0f;
		break;
	case 'y':
		CamPosY += 1.0f;
		break;
	case 'z':
		CamPosZ += 1.0f;
		break;
	case 'X':
		CamPosX -= 1.0f;
		break;
	case 'Y':
		CamPosY -= 1.0f;
		break;
	case 'Z':
		CamPosZ -= 1.0f;
		break;
	case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

GLvoid TimeFunction(int value)
{

	glutPostRedisplay();
	glutTimerFunc(10, TimeFunction, 1);
}

GLvoid Camera()
{
	//ī�޶�============================================================================================================================
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
	//����===============================================================================================================================
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(60.0f), (float)800.0 / (float)800.0, 0.1f, 100.0f);
	GLuint Projectionlocation = glGetUniformLocation(s_program, "projectionTransform");
	glUniformMatrix4fv(Projectionlocation, 1, GL_FALSE, value_ptr(projection));
}

GLvoid Ground()
{
	//�ٴ� �׸���=========================================================================================================================
	glm::mat4 Scale = glm::mat4(1.0f);
	Scale = glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 2.0f, 50.0f));

	unsigned int TransformLocation = glGetUniformLocation(s_program, "modelTransForm"); //--- ���ؽ� ���̴����� �𵨸� ��ȯ ��ġ ��������
	glUniformMatrix4fv(TransformLocation, 1, GL_FALSE, glm::value_ptr(Scale)); //--- modelTransform ������ ��ȯ �� �����ϱ�

	GroudInitBuffer();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

GLvoid Cube()
{
	glm::mat4 Trans = glm::mat4(1.0f);
	Trans = glm::translate(glm::mat4(1.0f), glm::vec3(CubePosX, 0.2f, CubePosZ));
	glm::mat4 Scale = glm::mat4(1.0f);
	Scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 Mat_Cube = glm::mat4(1.0f);
	Mat_Cube = Scale * Trans;

	GLuint TransformLocation = glGetUniformLocation(s_program, "modelTransForm"); //--- ���ؽ� ���̴����� �𵨸� ��ȯ ��ġ ��������
	glUniformMatrix4fv(TransformLocation, 1, GL_FALSE, glm::value_ptr(Mat_Cube)); //--- modelTransform ������ ��ȯ �� �����ϱ�

	CubeInitBuffer();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawScene() //--- glutDisplayFunc()�Լ��� ����� �׸��� �ݹ� �Լ�
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(s_program);

	Camera(); //ī�޶�
	Projection(); //����
	Ground(); //�ٴ� �׸���
	Cube(); //��ü �׸���

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
	glutCreateWindow("��ǳ� ģ����");

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();
	make_shaderProgram();
	glEnable(GL_DEPTH_TEST);
	glGenVertexArrays(1, &vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glBindVertexArray(vao); //--- VAO�� ���ε��ϱ�
	glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(10, TimeFunction, 1);

	glutMainLoop();
}

GLvoid CubeInitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glBindVertexArray(vao); //--- VAO�� ���ε��ϱ�
	glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_color), cube_color, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

GLvoid GroudInitBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground_color), ground_color, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

//GLvoid InitShader()
//{
//	make_vertexShaders(); //--- ���ؽ� ���̴� �����
//	make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����
//	//-- shader Program
//	s_program = glCreateProgram();
//
//	glAttachShader(s_program, vertexShader);
//	glAttachShader(s_program, fragmentShader);
//	glLinkProgram(s_program);
//	
//	//--- ���̴� �����ϱ�
//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader);
//	//--- Shader Program ����ϱ�
//	glUseProgram(s_program);
//}

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