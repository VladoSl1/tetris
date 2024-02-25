#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;

void main()
{
	TexCoords = aTexCoord;
	gl_Position = projection * model * vec4(aPos, 0.0f, 1.0);
}

//#version 330 core
//layout(location = 0) in vec2 aPos;
//layout(location = 1) in vec2 aTexCoord;
//layout(location = 2) in mat4 aModel;
//layout(location = 3) in vec3 aModelColor;
//
//out vec2 TexCoords;
//out vec3 modelColor;
//
//uniform mat4 projection;
//
//void main()
//{
//	TexCoords = aTexCoord;
//	modelColor = aModelColor
//	gl_Position = projection * model * vec4(aPos, 0.0f, 1.0);
//}