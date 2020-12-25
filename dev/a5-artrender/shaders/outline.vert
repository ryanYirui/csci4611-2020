#version 330

// CSci-4611 Assignment 5:  Art Render

// TODO: You need to modify this vertex shader to move the edge vertex along
// the normal away from the mesh surface IF you determine that the vertex
// belongs to a silhouette edge.


uniform mat4 model_view_matrix;
uniform mat4 normal_matrix;
uniform mat4 proj_matrix;
uniform float thickness;

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 left_normal;
layout(location = 3) in vec3 right_normal;

void main() {
    //create vectex left and right eyepoint  v
    vec3 v, left_n,right_n, final;
    //add outline
    final = vertex + (normal*thickness);
    left_n = (normal_matrix * vec4(left_normal,0)).xyz;
    right_n = (normal_matrix * vec4(right_normal,0)).xyz;
    v = (model_view_matrix * vec4(vertex,1)).xyz;

     
    if (dot(left_n,v) * dot(right_n,v)<=0){
      gl_Position = proj_matrix * model_view_matrix * vec4(final,1);
    }
    else{
    gl_Position = proj_matrix * model_view_matrix * vec4(vertex,1);
    }
}
