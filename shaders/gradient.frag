#version 330

out vec4 fragColour;

void main()
{
	fragColour = vec4(cos(gl_FragCoord.xy*3), 1.0, 1.0);
}
