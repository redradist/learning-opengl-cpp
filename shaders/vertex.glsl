#version 430

layout (location=0) in vec3 position;
out vec4 varyingColor;

uniform mat4 v_matrix;
uniform mat4 proj_matrix;
uniform float tf; // time factor for animation and placement of cubes

mat4 buildTranslate(float x, float y, float z);
mat4 buildRotateX(float rad); // declaration of matrix transformation utility functions
mat4 buildRotateY(float rad); // (GLSL requires functions to be declared prior to invocation)
mat4 buildRotateZ(float rad);

void main(void) {
    float i = gl_InstanceID + tf; // value based on time factor, but different for each cube instance
    float a = sin(203.0 * i/8000.0) * 403.0;
    float b = cos(301.0 * i/4001.0) * 401.0;
    float c = sin(400.0 * i/6003.0) * 405.0;

    // build the rotation and translation matrices to be applied to this cube’s model matrix
    mat4 localRotX = buildRotateX(1 * i);
    mat4 localRotY = buildRotateY(1 * i);
    mat4 localRotZ = buildRotateZ(1 * i);
    mat4 localTrans = buildTranslate(a, b, c);

    // build the model matrix and then the model-view matrix
    mat4 newM_matrix = localTrans * localRotX * localRotY * localRotZ;
    mat4 mv_matrix = v_matrix * newM_matrix;
    gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);
    varyingColor = vec4(position, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}
