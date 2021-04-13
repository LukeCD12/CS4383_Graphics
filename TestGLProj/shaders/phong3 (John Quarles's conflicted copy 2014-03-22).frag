 varying vec3 N;
 varying vec3 L;
 varying vec3 E;
 varying vec3 H;
 varying vec4 eyePosition;
 varying vec2 texCoordsInterpolated;
 

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
 uniform sampler2D diffuseTexture;

void main()
{
     vec3 Normal = normalize(N);
     vec3 Light  = normalize(lightPosition - eyePosition).xyz;
     vec3 Eye    = normalize(E);
     vec3 Half   = normalize(H);
	float linearAttenuation = 1.0; //min(1, 1/ (.3 * length(lightPosition - eyePosition)));
    float Kd = max(dot(Normal, Light), 0.0);
    float Ks = pow(max(dot(reflect(-Light, Normal),Eye), 0.0), shininess);
    float Ka = 1.0;

    vec4 diffuse  = Kd * lightDiffuse*surfaceDiffuse;
    vec4 specular = Ks * lightSpecular*surfaceSpecular;
    vec4 ambient  = Ka * lightAmbient*surfaceAmbient;
	vec4 texColor = vec4(1.0,1.0,1.0,1.0);
	if(useTexture == 1)
		texColor = texture2D(diffuseTexture,texCoordsInterpolated);
    gl_FragColor = texColor;//surfaceEmissive + ambient + linearAttenuation*( diffuse + specular)*texColor;
}
