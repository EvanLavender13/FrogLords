@vs vs
layout(binding=0) uniform vs_params {
    mat4 mvp;
};

layout(location=0) in vec3 position;

void main() {
    gl_Position = mvp * vec4(position, 1.0);
}
@end

@fs fs
layout(binding=1) uniform fs_params {
    vec4 color;
};

out vec4 frag_color;

void main() {
    frag_color = color;
}
@end

@program wireframe vs fs
