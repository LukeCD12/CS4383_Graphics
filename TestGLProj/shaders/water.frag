#version 440 core
 in vec3 N;
 in vec3 L;
 in vec3 E;
 in vec3 H;
 in float d;

 out vec4 color;
void main()
{
     vec3 Normal = normalize(N);
     vec3 Light  = normalize(L);
     vec3 Eye    = normalize(E);
     vec3 Half   = normalize(H);
     // vec3 Half  = normalize(L+E);

    float Kd = max(dot(Normal, Light), 0.0);
    float Ks = pow(max(dot(Half, Normal), 0.0), 280.0);
    float Ka = 1.0;
	float att = min(1.0,1.0/(.01*pow(d,3.0)));
    vec4 diffuse  = Kd * vec4(1.0,0.0,0.0,.5);
    vec4 specular = Ks * vec4(1.0,1.0,1.0,1.0); 
    vec4 ambient  = Ka * vec4(.10,.10,.10,1.0);;
	
    color = ambient + att *(diffuse + specular);
}

