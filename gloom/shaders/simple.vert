#version 430 core

in layout(location=0) vec3 position;
in layout(location=1) vec4 colour;
out layout(location=1) vec4 colourOut;
uniform layout(location=2) mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(position, 1.0f);

    colourOut = colour; // Pass on colour information
}
