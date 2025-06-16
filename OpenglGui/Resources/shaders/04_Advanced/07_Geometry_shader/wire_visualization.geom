#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];


uniform mat4 projection;
uniform float radio;

void GenerateLine(int index)
{
    gl_Position = projection * (gl_in[index].gl_Position+vec4(gs_in[index].normal * radio, 0.0));
    EmitVertex();
    gl_Position = projection * (gl_in[(index+1)%3].gl_Position+vec4(gs_in[(index+1)%3].normal * radio, 0.0));
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(0); // first vertex normal
    GenerateLine(1); // second vertex normal
    GenerateLine(2); // third vertex normal
}
