#version 330

in vec3 normal_;
in vec3 texture_;
uniform sampler2D sample;
uniform vec3 color;
uniform vec3 light_position;

out vec4 final_color;

void main() {
    vec3 texture_color =texture(sample, texture_.xy).xyz;
    vec3 L = normalize(light_position);
    vec3 N = normalize(normal_);
    //float Idiff = max(dot(L, N), 0.0);
    float Idiff = abs(dot(L, N));
    final_color = vec4(color + Idiff*vec3(0.2f,0.2f,0.2f) ,1.0f);
}