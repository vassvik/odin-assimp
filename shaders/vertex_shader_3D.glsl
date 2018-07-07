#version 440 core

out vec3 position;
out vec3 normal;

uniform mat4 MVP;

struct Triangle {
    vec3 v0;
    float nx;
    vec3 v1;
    float ny;
    vec3 v2;
    float nz;
};

layout (std430, binding = 0) buffer triangle_buffer {
    Triangle triangles[];
};

void main() {
	Triangle t = triangles[gl_InstanceID];

	switch (gl_VertexID) {
	case 0: position = t.v0; break;
	case 1: position = t.v1; break;
	case 2: position = t.v2; break;
	}
	position.xyz = position.xyz;
	normal = normalize(vec3(t.nx, t.ny, t.nz));

    gl_Position = MVP*vec4(position, 1.0);
}
