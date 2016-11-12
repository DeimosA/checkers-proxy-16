#version 430 core

in layout(location=1) vec4 inColour;
out vec4 colour;

void main()
{
    colour = inColour;
}
