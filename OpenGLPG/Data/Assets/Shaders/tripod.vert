#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 ModelView;

out VS_OUT {
    mat3 myNormalMatrix;
} vs_out;

void main()
{
    vs_out.myNormalMatrix = mat3(ModelView);
    gl_Position = vec4(aPos, 1.0);
    gl_Position = ModelView * gl_Position;
}