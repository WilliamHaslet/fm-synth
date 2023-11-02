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

uniform vec2 screenSize = vec2(1, 1);
uniform vec2 size = vec2(1, 1);
uniform vec2 position = vec2(0, 0);
uniform vec2 rotationOrigin = vec2(0.5, 0.5);
uniform float layer = 0;
uniform mat4 rotation;

void main()
{
    vec4 pos = vec4(in_position, 1);

    // move to center
    pos.x -= 0.5;
    pos.y -= 0.5;

    pos.x *= size.x * 2;
    pos.y *= size.y * 2;

    // move relative to top left 
    pos.x += size.x;
    pos.y -= size.y;

    //pos.x -= size.x * ;
    //pos.y += size.y * rotationOrigin.y;

    // rotate around center
    pos.x -= size.x * 2 * rotationOrigin.x;
    pos.y += size.y * 2 * rotationOrigin.y;

    pos = rotation * pos;

    // correct for center rotation movement
    pos.x += size.x * 2 * rotationOrigin.x;
    pos.y -= size.y * 2 * rotationOrigin.y;

    // start from top left of screen
    pos.x -= screenSize.x;
    pos.y += screenSize.y;

    pos.x += position.x * 2;
    pos.y -= position.y * 2;

    // back to clip space
    pos.x /= screenSize.x;
    pos.y /= screenSize.y;

    pos.z = -layer;

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
