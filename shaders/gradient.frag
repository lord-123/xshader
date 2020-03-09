#version 330

out vec4 fragColour;

void main()
{
	vec2 uv = gl_FragCoord.xy / vec2(1920, 1080);

	vec3 col = 0.5 + 0.5*cos(uv.xyx + vec3(0, 2, 4));

	fragColour = vec4(col, 1.0);
}
