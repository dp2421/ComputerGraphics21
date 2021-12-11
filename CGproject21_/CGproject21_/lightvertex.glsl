#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform vec3 in_Color;
out vec3 ex_Color;

uniform mat4 modelTransForm;
uniform mat4 projectionTransform;
uniform mat4 viewTransform;

out vec3 passColor;

void main(void) 
{
gl_Position = projectionTransform*viewTransform *modelTransForm * vec4(vPos, 1.0);
FragPos = vec3(modelTransForm*vec4(vPos,1.0));
Normal = vNormal;
TexCoord = vTexCoord;
}