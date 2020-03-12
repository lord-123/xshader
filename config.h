/* modify this file to change the shader and uniforms */
#define SHADER_PATH "/home/mabel/.local/bin/shaders/gradient.frag" 

/* measured in microseconds */
#define DELAY 30000

/* shader uniforms */
#define UNIFORM_COUNT 1

/* custom functions used to set uniform values */
GLint timeValue = 0;
void time (GLint uniformId) { glUniform1i(uniformId, timeValue++); }

/* array of all uniforms to pass to the shader */
static const Uniform uniforms[] = {
	{ "time", &time },
};
