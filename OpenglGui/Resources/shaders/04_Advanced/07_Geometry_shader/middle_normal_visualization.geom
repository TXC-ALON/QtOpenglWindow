#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 2) out;

uniform mat4 projection;

void main()
{

    vec3 A = gl_in[0].gl_Position.xyz;
    vec3 B = gl_in[1].gl_Position.xyz;
    vec3 C = gl_in[2].gl_Position.xyz;


    vec3 AB = B - A;
    vec3 AC = C - A;


    vec3 normal = normalize(cross(AB, AC));


    vec4 center = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) / 3.0;


    gl_Position = projection * center;
    EmitVertex();


    float magnitude = 0.2;
    gl_Position = projection * (center + vec4(normal * magnitude, 0.0));
    EmitVertex();

    EndPrimitive();
}