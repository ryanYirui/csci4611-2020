#version 330

in vec3 position_in_eye_space;
in vec3 normal_in_eye_space;

out vec4 color;

uniform vec3 light_in_eye_space;
uniform vec4 Ia, Id, Is;

uniform vec4 ka, kd, ks;
uniform float s;


void main() {
    vec3 l,e,n,h;
    l = normalize(light_in_eye_space - position_in_eye_space);  //unit vector to light
    e = normalize(-position_in_eye_space);  //unit vector to eye
    n = normalize(normal_in_eye_space); //normal
    h = normalize(1+e);  
  
    vec4 diffuse = kd * Id * max(dot(n, l), 0);
    vec4 specular = ks *Is * pow(max(dot(n, h), 0), s);
    color = ka * Ia + diffuse + specular;
}
