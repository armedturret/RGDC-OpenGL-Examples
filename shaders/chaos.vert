#version 330 core
in vec3 in_pos;
in vec2 in_uv;

out vec3 fragCoord;
out vec2 texCoord;

uniform mat4 mvp;

void main()
{
	fragCoord = in_pos;
	texCoord = in_uv;
	
	gl_Position = mvp * vec4(in_pos, 1.0);
}