#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

typedef struct {
	char* name;
	void (*setValue)(GLint);
} Uniform;

#include "config.h"

static Display*			display;
static int 			screen;
static Window 			window;
static GLint			visualAttributes[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
static XVisualInfo* 		visualInfo;
static GLXContext 		glContext;
static XWindowAttributes 	windowAttributes;

static GLuint 			programId;

GLint				uniformIds[UNIFORM_COUNT];

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
		buffer[length] = '\0'; /* fread does not 0 terminate strings */
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
	if (shaderCompiled != GL_TRUE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);
		
		
		char message[maxLength];
		glGetShaderInfoLog(shaderId, maxLength, &maxLength, message);

		printf("%s\n", &(message[0]));

		printf("\n\tunable to compile shader %d!\n\nSource:\n%s\n", shaderId, shaderSource);
		glDeleteShader(shaderId);
		shaderId = 0;
	}

	return shaderId;
}

Bool loadProgram()
{
	programId = glCreateProgram();

	GLuint shader = loadShader(readFile(SHADER_PATH), GL_FRAGMENT_SHADER);

	if (shader == 0)
	{
		glDeleteProgram(programId);
		programId = 0;
		return False;
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
		return False;
	}

	glDeleteShader(shader);

	return True;
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

void drawScreen() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);
	 glVertex2f(-1., -1.);
	 glVertex2f( 1., -1.);
	 glVertex2f( 1.,  1.);
	 glVertex2f(-1.,  1.);
	glEnd();
}

int main()
{
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

	if (!loadProgram())
	{
		printf("\n\tshader program failed to load\n\n");
		exit(0);
	}

	getUniforms();

	glUseProgram(programId);

	while(1)
	{
		setUniforms();

		XGetWindowAttributes(display, window, &windowAttributes);
		glViewport(0, 0, windowAttributes.width, windowAttributes.height);
		drawScreen(); 
		glXSwapBuffers(display, window);

		usleep(DELAY);
	}
}
