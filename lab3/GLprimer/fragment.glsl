#version 330 core

in float uTime;
in vec3 uNormal;
in vec2 UV;
in mat4 uTranslation;

out vec4 finalcolor;

void main() 
{
    vec3 light = vec3(uTranslation * vec4(0.0, 0.0, 0.0, 1.0));

    vec3 R = 2.0 * dot(uNormal, light) * uNormal - light;
    float dotNL = max(dot(uNormal, light), 0.0); 
    float dotRV = max(dot(R, vec3(0,0,1)), 0.0);

    if (dotNL == 0.0) {
	    dotRV = 0.0;  // Do not show highlight on the dark side
    }
    
    vec3 ka = vec3(0.8, 0.97, 0.02);
    vec3 Ia = vec3(0.1);

    vec3 kd = vec3(0.8, 0.97, 0.02);
    vec3 Id = vec3(0.75);

    vec3 ks = vec3(0.5);
    vec3 Is = vec3(0.5);

    vec3 shading = Ia * ka + Id * kd * dotNL + Is * ks * pow(dotRV, 100); 
    finalcolor = vec4(shading, 1.0);
}