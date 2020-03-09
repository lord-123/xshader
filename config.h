/* modify this file to change the shader and uniforms */
static const char* shaderPath = "/home/mabel/.local/bin/shaders/gradient.frag"; 

/* measured in microseconds */
static const float refreshRate = 30000;

/* shader uniforms */
GLuint timeUniform;
GLint timeValue = 0;

static void getUniforms(GLuint programId)
{
	timeUniform = glGetUniformLocation(programId, "time");
}

static void setUniforms()
{
	timeValue += 1;
	printf("timevalue: %i\n", timeValue);
	if (timeUniform != -1)
	{
		printf("uniform updated\n");
		glUniform1i(timeUniform, timeValue);
	}
}
