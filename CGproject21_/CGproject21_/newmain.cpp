#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <random>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
//#include <glm/glm/glm.hpp>
//#include <glm/glm/ext.hpp>
//#include <glm/glm/gtc/matrix_transform.hpp>


////////너랑 나랑 gl경로가 달라서 서로 상대방 거 주석처리하고 사용하는 걸로 하자!/////////


////////////////////미래의 상대방에게 전하는 메시지////////////////////
//아항 알게써!!
//계산 쉽게 전체적인 단위 변경
//객체 크기 1X1X1, 이동 2씩, 맵크기 60(-30~30)
//속도 수정 완료,,근데 뭔가 더 있어보이게 하고싶은데 이건 다 끝나고 여유로울 때 수정해야지

using namespace std;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int, int);
GLvoid CubeInitBuffer();
GLvoid GroundInitBuffer();
//GLvoid InitShader();

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

GLuint vao[2], vbo[2], ebo;

//육면체 위치 변수
GLfloat CubePosX;
GLfloat CubePosZ = 30.0f;

//카메라 위치 변수
GLfloat CamPosX = 2.0f;
GLfloat CamPosY = 3.0f;
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

//맵(1이 장애물이 있는 칸, 0은 없는 칸)
int map[30] = { 0,0,0,0,1,1,1,0,0,1,1,0,0,0,1,0,0,0,1,1,0,0,0,0,1,1,1,0,0,0 };

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

BB getbb_cube(float centerx, float centerz)
{
	return BB(centerx - 0.5f, centerz - 0.5f, centerx + 0.5f, centerz + 0.5f);
}

BB getbb_car(float centerx, float centerz)
{
	return BB(centerx - 1.0f, centerz - 0.5f, centerx + 1.0f, centerz + 0.5f);
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

GLfloat ground_color[][3] = {
	0.690196, 0.768627, 0.870588,
	0.690196, 0.768627, 0.870588,
	0.690196, 0.768627, 0.870588,
	0.690196, 0.768627, 0.870588,
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

GLfloat cube_color[][3] = {
	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,
	1, 0.713725, 0.756863,
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
	case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

float cnt[3] = { -10.f };	//도로의 끝인지, 체크하는 변수
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
	//바닥 그리기=========================================================================================================================
	glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 2.0f, 30.0f));

	unsigned int TransformLocation = glGetUniformLocation(s_program, "modelTransForm"); //--- 버텍스 세이더에서 모델링 변환 위치 가져오기
	glUniformMatrix4fv(TransformLocation, 1, GL_FALSE, glm::value_ptr(Scale)); //--- modelTransform 변수에 변환 값 적용하기
	glBindVertexArray(vao[0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

GLvoid Cube()
{
	glm::mat4 Trans = glm::translate(glm::mat4(1.0f), glm::vec3(CubePosX, 0.2f, CubePosZ));
	glm::mat4 Mat_Cube = Trans;

	GLuint TransformLocation = glGetUniformLocation(s_program, "modelTransForm"); //--- 버텍스 세이더에서 모델링 변환 위치 가져오기
	glUniformMatrix4fv(TransformLocation, 1, GL_FALSE, glm::value_ptr(Mat_Cube)); //--- modelTransform 변수에 변환 값 적용하기
	glBindVertexArray(vao[1]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

GLvoid Car()
{
	int mapcnt = 0;
	glm::mat4 Trans = glm::mat4(1.f);
	for (int i = 0; i < 30; i++)
	{
		//자동차 위치 변수
		GLfloat CarPosX = -10.0f;
		GLfloat CarPosZ = 30 - 2 * i;
		GLfloat car = 30 - 2 * 0;
		if (map[i] == 1)
		{
			glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.f, 0.7f, 1.f));
			glm::mat4 BottomScale = glm::scale(glm::mat4(1.f), glm::vec3(2.f, 1.f, 1.f));
			glm::mat4 UpperTrans = glm::translate(glm::mat4(1.f), glm::vec3(0, 0.7, 0));
			glm::mat4 Mat_Car = glm::mat4(1.f);
			glm::mat4 Trans_Car = glm::mat4(1.f);
			Trans = glm::translate(glm::mat4(1.0f), glm::vec3(CarPosX, 0.6f, CarPosZ));
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
			Trans_Car = Mat_Car * UpperTrans * Scale;
			GLuint TransformLocation = glGetUniformLocation(s_program, "modelTransForm"); //--- 버텍스 세이더에서 모델링 변환 위치 가져오기
			glUniformMatrix4fv(TransformLocation, 1, GL_FALSE, glm::value_ptr(Trans_Car)); //--- modelTransform 변수에 변환 값 적용하기
			glBindVertexArray(vao[1]);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			Trans_Car = Mat_Car * BottomScale;
			glUniformMatrix4fv(TransformLocation, 1, GL_FALSE, glm::value_ptr(Trans_Car)); //--- modelTransform 변수에 변환 값 적용하기
			glBindVertexArray(vao[1]);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

			if (Collide(getbb_cube(CubePosX, CubePosZ), getbb_car(CarPosX, CarPosZ)) == true)
			{
				cout << "충돌" << endl;
			}

			mapcnt++;
		}
	}



}
void drawScene() //--- glutDisplayFunc()함수로 등록한 그리기 콜백 함수
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(s_program);

	Camera(); //카메라
	Projection(); //투영
	Ground(); //바닥 그리기
	Cube(); //객체 그리기
	Car();
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
	glEnable(GL_DEPTH_TEST);

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

	glBufferData(GL_ARRAY_BUFFER, sizeof(ground_color), ground_color, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);	//첫 번째 인자: 인덱스
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); // GL_ELEMENT_ARRAY_BUFFER 버퍼 유형으로 바인딩
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ground_element), ground_element, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(2);	//vao에 vbo를 묶어줌
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

	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_color), cube_color, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);	//첫 번째 인자: 인덱스
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); // GL_ELEMENT_ARRAY_BUFFER 버퍼 유형으로 바인딩
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubelement), cubelement, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(2);	//vao에 vbo를 묶어줌
}

//GLvoid InitShader()
//{
//	make_vertexShaders(); //--- 버텍스 세이더 만들기
//	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
//	//-- shader Program
//	s_program = glCreateProgram();
//
//	glAttachShader(s_program, vertexShader);
//	glAttachShader(s_program, fragmentShader);
//	glLinkProgram(s_program);
//	
//	//--- 세이더 삭제하기
//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader);
//	//--- Shader Program 사용하기
//	glUseProgram(s_program);
//}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

void make_vertexShaders()
{
	vertexsource = filetobuf("vertex.glsl");

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
	fragmentsource = filetobuf("fragment.glsl");
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