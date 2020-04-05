#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#include <GL/glew.h>
#endif

//#pragma comment(lib, "glew32.lib")
#include <GL/gl.h>
#include <GL/glu.h>

#ifdef __linux__
#include <unistd.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#endif

typedef struct {
	char* name;
	void (*setValue)(GLint);
} Uniform;

#include "config.h"

#ifdef __linux__
Display* display;
int					screen;
Window				window;
GLint				visualAttributes[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
XVisualInfo* visualInfo;
GLXContext			glContext;
XWindowAttributes	windowAttributes;
#endif

#ifdef _WIN32
HWND	hwnd;
HDC		hdc;
PIXELFORMATDESCRIPTOR pfd =
{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_TYPE_RGBA,
		24,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
};
#endif

int width;
int height;

GLuint			programId;
GLuint			uniformIds[UNIFORM_COUNT];

const char* readFile(char* filepath) {
	char* buffer = 0;
	long length;
	FILE* f = fopen(filepath, "rb");

	if (f)
	{
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		buffer = malloc(length + 1);
		if (buffer)
		{
			fread(buffer, 1, length, f);
		}
		fclose(f);
		buffer[length] = '\0'; // fread does not 0 terminate strings
	}

	return buffer;
}

GLuint loadShader(const char* shaderSource, GLenum shaderType)
{
	GLuint shaderId = glCreateShader(shaderType);

	glShaderSource(shaderId, 1, (const GLchar**)&shaderSource, NULL);

	glCompileShader(shaderId);

	GLint shaderCompiled = GL_FALSE;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderCompiled);
	/*if (shaderCompiled != GL_TRUE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

		char message[maxLength];
		glGetShaderInfoLog(shaderId, maxLength, &maxLength, message);

		printf("%s\n", &(message[0]));
		printf("\n\tunable to compile shader %d!\n\nSource:\n%s\n", shaderId, shaderSource);

		glDeleteShader(shaderId);
		shaderId = 0;
	}*/

	return shaderId;
}

BOOL loadProgram()
{
	programId = glCreateProgram();

	GLuint shader = loadShader(readFile(SHADER_PATH), GL_FRAGMENT_SHADER);

	if (shader == 0)
	{
		glDeleteProgram(programId);
		programId = 0;
		return FALSE;
	}

	glAttachShader(programId, shader);

	glLinkProgram(programId);

	GLint programSuccess = GL_TRUE;
	glGetProgramiv(programId, GL_LINK_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE)
	{
		printf("\n\nerror linking program %d!\n", programId);
		glDeleteShader(shader);
		glDeleteProgram(programId);
		programId = 0;
		return FALSE;
	}

	glDeleteShader(shader);

	return TRUE;
}

void getUniforms()
{
	for (int i = 0; i < UNIFORM_COUNT; i++)
	{
		Uniform* current = uniforms + i;
		uniformIds[i] = glGetUniformLocation(programId, current->name);
	}
}

void setUniforms()
{
	for (int i = 0; i < UNIFORM_COUNT; i++)
	{
		Uniform* current = uniforms + i;
		current->setValue(uniformIds[i]);
	}
}

void sleep(int microseconds)
{
#ifdef __linux__
	usleep(microseconds);
#endif

#ifdef _WIN32
	Sleep(microseconds / 1000);
#endif
}

void drawScreen() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);
	glVertex2f(-1., -1.);
	glVertex2f(1., -1.);
	glVertex2f(1., 1.);
	glVertex2f(-1., 1.);
	glEnd();
}

#ifdef _WIN32
/* credit to Barnack for these two function https://stackoverflow.com/a/56132585/9010685 */
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	HWND p = FindWindowEx(hwnd, NULL, L"SHELLDLL_DefView", NULL);
	HWND* ret = (HWND*)lParam;

	if (p)
		*ret = FindWindowEx(NULL, hwnd, L"WorkerW", NULL);

	return TRUE;
}

HWND get_wallpaper_window()
{
	HWND progman = FindWindow(L"Progman", NULL);

	/*
	   send 0x052C to progman; this message directs progman to spawn a
	   WorkerW behind the desktop icons. If it is already there, nothing
	   happens
	*/
	SendMessageTimeout(progman, 0x052C, 0, 0, SMTO_NORMAL, 1000, NULL);

	HWND wallpaper_hwnd = NULL;
	EnumWindows(EnumWindowsProc, (LPARAM)&wallpaper_hwnd);

	return wallpaper_hwnd;
}
#endif

int main()
{


#ifdef __linux__
	display = XOpenDisplay(NULL);
	screen = DefaultScreen(display);

	if (display == NULL)
	{
		printf("\n\tcannot connect to X server\n\n");
		exit(0);
	}

	visualInfo = glXChooseVisual(display, 0, visualAttributes);

	if (visualInfo == NULL)
	{
		printf("\n\tno appropriate visual found\n\n");
		exit(0);
	}
	else
	{
		printf("\n\tvisual %p selected\n", (void*)visualInfo->visualid);
	}

	window = RootWindow(display, screen);

	glContext = glXCreateContext(display, visualInfo, NULL, GL_TRUE);
	glXMakeCurrent(display, window, glContext);

	XSelectInput(display, window, ExposureMask);

	XGetWindowAttributes(display, window, &windowAttributes);

	width = windowAttributes.width;
	height = windowAttributes.height;
#endif

#ifdef _WIN32
	hwnd = get_wallpaper_window();
	hdc = GetDC(hwnd);

	SetPixelFormat(hdc, ChoosePixelFormat(hdc, &pfd), &pfd);

	HGLRC context = wglCreateContext(hdc);

	wglMakeCurrent(hdc, context);

	if (GLEW_OK != glewInit())
		exit(1);

	RECT rect;
	GetClientRect(hwnd, &rect);
	width = rect.right;
	height = rect.bottom;
#endif

	if (!loadProgram())
	{
		printf("\n\tshader program failed to load\n\n");
		exit(0);
	}

	getUniforms();

	glUseProgram(programId);

	glViewport(0, 0, width, height);

	while (1)
	{
		setUniforms();

		drawScreen();

#ifdef __linux__
		glXSwapBuffers(display, window);
#endif

#ifdef _WIN32
		SwapBuffers(hdc);
#endif

		sleep(DELAY);
	}
}