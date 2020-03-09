#version 330

uniform int time;
out vec4 fragColour;

void main()
{
	vec2 uv = gl_FragCoord.xy / vec2(1920.0, 1080.0);
	
	float newTime = float(time);
	vec3 col = 0.5 + 0.5*cos(newTime/100+uv.xyx + vec3(0.0, 2.0, 4.0));

	fragColour = vec4(col, 1.0);
}
