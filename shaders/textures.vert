#version 330 core
in vec3 in_pos;
in vec2 in_uv;

out vec2 texCoord;

void main()
{
	texCoord = in_uv;
	
	gl_Position = vec4(in_pos, 1.0);
}