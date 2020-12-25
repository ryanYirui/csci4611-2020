#version 330

// CSci-4611 Assignment 5:  Art Render

// TODO: You need to calculate per-fragment shading here using a toon shading model

in vec3 position_in_eye_space;
in vec3 normal_in_eye_space;

out vec4 color;

uniform vec3 light_in_eye_space;
uniform vec4 Ia, Id, Is;

uniform vec4 ka, kd, ks;
uniform float s;

uniform sampler2D diffuse_ramp;
uniform sampler2D specular_ramp;

// like what I did in the worksheet I have to assign the l e n  and h with the value
void main() {
    vec3 l,e,n,h;
    l = normalize(light_in_eye_space - position_in_eye_space);
    //normal vector
    n = normalize(normal_in_eye_space);
    e = normalize(-position_in_eye_space);
    h = normalize(1+e);
    // color here is based on diffuse_ramp and the specular_ramp

    vec2 diff = vec2(0.5*dot(n, l)+0.5,0);
    vec4 diffuse = kd * Id* texture(diffuse_ramp,diff);

    float temp = pow(max(dot(h,n),0),s);
    vec2 spec = vec2(temp,0);
    vec4 specular = ks * Is * texture(specular_ramp,spec);
    color = ka * Ia + diffuse + specular; //conside the light distribution
}
