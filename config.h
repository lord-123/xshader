/* modify this file to change the shader and uniforms */
static const char* shaderPath = "/home/mabel/.local/bin/shaders/gradient.frag"; 

/* measured in microseconds */
static const float refreshRate = 30000;

/* shader uniforms */
#define UNIFORM_COUNT 1

GLint timeValue = 0;
void time (GLint uniformId) { glUniform1i(uniformId, timeValue++); }

static const Uniform uniforms[] = {
	{ "time", &time },
};
