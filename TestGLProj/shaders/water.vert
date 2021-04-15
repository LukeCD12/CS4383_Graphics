#version 440 core
out vec3 N;
out vec3 L;
out vec3 E;
out vec3 H;
out float d;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in  vec3 vertexNormal;

uniform vec4 lightPosition;
uniform mat4 Projection;
uniform mat4 ModelView;
uniform float time;

void main()
{
vec3 vertexWave = vertexPosition;

vertexWave.y = 0.1 * sin(0.001 * time + 5.0 * vertexWave.x) * sin(0.001 * time + 5.0 * vertexWave.z);
	
    gl_Position = Projection * ModelView * vec4(vertexWave, 1.0);

    vec4 eyePosition = ModelView * vec4(vertexWave, 1.0);
    vec4 eyeLightPos =  lightPosition;

    N = normalize(ModelView * vec4(vertexNormal, 0.0)).xyz;
    d = length(eyeLightPos.xyz - eyePosition.xyz);
    E = -normalize(eyePosition.xyz);
    L = normalize(eyeLightPos.xyz - eyePosition.xyz);
    H = normalize(L + E);
}

