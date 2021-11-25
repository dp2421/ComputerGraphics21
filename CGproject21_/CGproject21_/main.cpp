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

////////너랑 나랑 gl경로가 달라서 서로 상대방 거 주석처리하고 사용하는 걸로 하자!/////////

//아항 알게써!!

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

//육면체 위치 변수
GLfloat CubePosX;
GLfloat CubePosZ = 45.0f;

//카메라 위치 변수
GLfloat CamPosX = 2.0f;
GLfloat CamPosY = 3.0f;
GLfloat CamPosZ = 50.0f;

//카메라가 보는 방향 변수
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

	//윗면
	{   -0.1, 0.1, -0.1,
		-0.1, 0.1, 0.1,
		0.1, 0.1, -0.1   },
	{   0.1, 0.1, -0.1,
		-0.1, 0.1, 0.1,
		0.1, 0.1, 0.1   },
		//아랫면
		{   -0.1, -0.1, 0.1,
			-0.1, -0.1, -0.1,
			0.1, -0.1, 0.1   },
		{   0.1, -0.1, 0.1,
			-0.1, -0.1, -0.1,
			0.1, -0.1, -0.1   },
			//앞면
			{   -0.1, 0.1, 0.1,
				-0.1, -0.1, 0.1,
				0.1, 0.1, 0.1   },
			{   0.1, 0.1, 0.1,
				-0.1, -0.1, 0.1,
				0.1, -0.1, 0.1   },
				//오른쪽면
				{   0.1, 0.1, 0.1,
					0.1, -0.1, 0.1,
					0.1, 0.1, -0.1   },
				{   0.1, 0.1, -0.1,
					0.1, -0.1, 0.1,
					0.1, -0.1, -0.1   },
					//뒷면
					{   0.1, 0.1, -0.1,
						0.1, -0.1, -0.1,
						-0.1, 0.1, -0.1   },
					{   -0.1, 0.1, -0.1,
						0.1, -0.1, -0.1,
						-0.1, -0.1, -0.1   },
						//왼쪽면
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
		/////////////////////////////카메라가 이동할 수 있는게 짜기 편할 것 같아서 x,y,z로 이동할 수 있게 해놨어!!!///////////////////////
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
	glm::mat4 Scale = glm::mat4(1.0f);
	Scale = glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 2.0f, 50.0f));

	unsigned int TransformLocation = glGetUniformLocation(s_program, "modelTransForm"); //--- 버텍스 세이더에서 모델링 변환 위치 가져오기
	glUniformMatrix4fv(TransformLocation, 1, GL_FALSE, glm::value_ptr(Scale)); //--- modelTransform 변수에 변환 값 적용하기

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

	GLuint TransformLocation = glGetUniformLocation(s_program, "modelTransForm"); //--- 버텍스 세이더에서 모델링 변환 위치 가져오기
	glUniformMatrix4fv(TransformLocation, 1, GL_FALSE, glm::value_ptr(Mat_Cube)); //--- modelTransform 변수에 변환 값 적용하기

	CubeInitBuffer();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
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
	glEnable(GL_DEPTH_TEST);
	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
	glBindVertexArray(vao); //--- VAO를 바인드하기
	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(10, TimeFunction, 1);

	glutMainLoop();
}

GLvoid CubeInitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
	glBindVertexArray(vao); //--- VAO를 바인드하기
	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기

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