#version 430

uniform float offset;

mat4 buildRotateX(float rad);
mat4 buildRotateY(float rad);
mat4 buildRotateZ(float rad);

void main(void) {
    mat4 rot_tran = buildRotateZ(1);
    if (gl_VertexID == 0) gl_Position = rot_tran * vec4(0.25 + offset, -0.25, 0.0, 1.0);
    else if (gl_VertexID == 1) gl_Position = rot_tran * vec4(-0.25 + offset, -0.25, 0.0, 1.0);
    else gl_Position = rot_tran * vec4(0.25 + offset, 0.25, 0.0, 1.0);
}
