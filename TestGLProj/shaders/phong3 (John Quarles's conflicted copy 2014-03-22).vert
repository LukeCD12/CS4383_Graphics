varying vec3 N;
varying vec3 L;
varying vec3 E;
varying vec3 H;
varying vec4 eyePosition;
varying vec2 texCoordsInterpolated;
 
attribute vec3 vertexPosition;
attribute vec3 vertexNormal;
attribute vec2 vertexTextureCoordinates;

uniform vec4 lightPosition;
uniform mat4 Projection;
uniform mat4 ModelView;

 uniform vec4 lightDiffuse;
 uniform vec4 lightSpecular; 
 uniform vec4 lightAmbient;
 uniform vec4 surfaceDiffuse;
 uniform vec4 surfaceSpecular;
 uniform float shininess;
 uniform vec4 surfaceAmbient;
 uniform vec4  surfaceEmissive;
 uniform int useTexture;
void main()
{
	if(useTexture ==1){
		texCoordsInterpolated = vertexTextureCoordinates;
	}
    gl_Position = Projection * ModelView * vec4(vertexPosition, 1.0);

    eyePosition = ModelView * vec4(vertexPosition, 1.0);
    vec4 eyeLightPos = lightPosition;
	
	N = normalize(ModelView * vec4(vertexNormal,0.0)).xyz;
    L = normalize(eyeLightPos.xyz - eyePosition.xyz);
    E = -normalize(eyePosition.xyz);
    H = normalize(L + E);
}

