#shader shared
#version 330 core

struct Varyings
{
    vec2 uv;
};

#shader vertex
layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;

out Varyings v;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    vec4 pos = vec4(in_position, 1);
    pos.x -= 0.5;
    pos.y -= 0.5;
    pos = projectionMatrix * viewMatrix * modelMatrix * pos;
    gl_Position = pos;
    v.uv = in_uv;
}

#shader fragment
in Varyings v;

out vec4 out_color;

uniform vec3 color = vec3(1, 1, 1);

void main()
{
    out_color = vec4(color, 1);
}
