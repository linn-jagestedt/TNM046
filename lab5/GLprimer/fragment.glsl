#version 330 core

in float uTime;
in vec3 uNormal;
in vec2 UV;

uniform sampler2D uTexture;

out vec4 finalcolor;

vec2 SineWave( vec2 p ) {
    float pi = 3.14159;
    float A = 0.15;
    float w = 10.0 * pi;
    float t = 30.0*pi/180.0;
    float y = sin( w*p.x + t) * A; 
    return vec2(p.x, p.y+y);
}

void main() 
{
    vec3 light = vec3(vec4(0.7, 0.7, 0.0, 1.0));

    vec3 R = 2.0 * dot(uNormal, light) * uNormal - light;
    float dotNL = max(dot(uNormal, light), 0.0); 
    float dotRV = max(dot(R, vec3(0,0,1)), 0.0);

    if (dotNL == 0.0) {
	    dotRV = 0.0;  // Do not show highlight on the dark side
    }
    
    vec3 ka = texture(uTexture,  UV).rgb * 0.7;
    vec3 Ia = vec3(0.3);

    vec3 kd = texture(uTexture, UV).rgb;
    vec3 Id = vec3(1);

    vec3 ks = vec3(0.75);
    vec3 Is = vec3(0.75);

    vec3 shading = Ia * ka + Id * kd * dotNL + Is * ks * pow(dotRV, 100); 
    finalcolor = vec4(shading, 1.0);
}