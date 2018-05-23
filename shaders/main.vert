#version 330

in vec3 position;
in vec3 normal;
in vec3 texture;

out vec3 normal_;
out vec3 texture_;
uniform mat4 MVPmatrix;

void main() {
    gl_Position = MVPmatrix * vec4(position, 1.0);
    normal_ = normal;
    texture_ = texture;
}