#version 330

layout (lines_adjacency) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float _LineWidth;

void main()
{   
    vec3 prev = gl_in[0].gl_Position.xyz;
    vec3 start = gl_in[1].gl_Position.xyz;
    vec3 end = gl_in[2].gl_Position.xyz;
    vec3 next = gl_in[3].gl_Position.xyz;
    
    vec3 lhs = cross(normalize(end-start), vec3(0.0, 0.0, -1.0));
  
    // is previous line segment a zero vector?
    bool colStart = length(start-prev) < 0.0001; // 0.0001 is arbitrary epsilon
    // is next line segment a zero vector?
    bool colEnd = length(end-next) < 0.0001;
    
    vec3 a = normalize(start-prev);
    vec3 b = normalize(start-end);
    vec3 c = (a+b)*0.5;
    
    vec3 startLhs = normalize(c) * sign(dot(c, lhs));
    
    a = normalize(end-start);
    b = normalize(end-next);
    c = (a+b)*0.5;
    
    vec3 endLhs = normalize(c) * sign(dot(c, lhs));
    
    if(colStart)
        startLhs = lhs;
    if(colEnd)
        endLhs = lhs;
    
    float startInvScale = dot(startLhs, lhs);
    float endInvScale = dot(endLhs, lhs);
    
    startLhs /= startInvScale;
    endLhs /= endInvScale;
    
    startLhs *= _LineWidth * 0.5;
    endLhs *= _LineWidth * 0.5;
    
    //startLhs = clamp(startLhs, _LineWidth * -2., _LineWidth * 2.);
    //endLhs = clamp(endLhs, _LineWidth * -2., _LineWidth * 2.);
    
    gl_Position = projection * view * model * vec4(start + startLhs, 1.0);
    EmitVertex();
    
    gl_Position = projection * view * model * vec4(start - startLhs, 1.0);
    EmitVertex();
    
    gl_Position = projection * view * model * vec4(end + endLhs, 1.0);
    EmitVertex();
    
    gl_Position = projection * view * model * vec4(end - endLhs, 1.0);
    EmitVertex();
    
    EndPrimitive();
}
