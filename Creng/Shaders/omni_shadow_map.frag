#version 330

in vec4 FragPos;

uniform vec3 lightPos;
uniform float farPlane;

void main() {
    
    float fragToLight = length(FragPos.xyz - lightPos);
    fragToLight = fragToLight / farPlane;

    gl_FragDepth = fragToLight;
}