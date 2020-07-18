#version 330

layout (location = 0) in vec4 position;

uniform sampler2DRect pos_texture;
uniform vec2 res;

void main()
{
    vec2 verPos = position.xy;
    vec4 pos = texture(pos_texture,verPos);
    pos.xy = 2. * pos.xy - 1.;
    pos.x *= res.x / res.y;
    gl_Position = pos;
}

/*
#version 120

uniform sampler2DRect pos_texture;

void main()
{
    vec2 verPos = gl_Vertex.xy;
    vec4 pos = texture2DRect(pos_texture,verPos);
    pos.xy = 2. * pos.xy - 1.;
    gl_Position = vec4(verPos,0.0,1.);
}
*/
