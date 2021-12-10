#version 330
in vec3 ex_Color;
in vec3 FragPos;
in vec3  Normal;

out vec4 gl_FragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec3 Alpha;
void main(void) 
{
float ambientLight = 0.5;
vec3 ambient = ambientLight*lightColor;

vec3 normalVector = normalize(Normal);
vec3 lightDir = normalize(lightPos-FragPos);
float diffuseLight=max(dot(normalVector, lightDir),0.0);
vec3 diffuse=diffuseLight*lightColor;


vec3 result = (ambient+diffuse)*ex_Color;
gl_FragColor = vec4(result,Alpha);
}