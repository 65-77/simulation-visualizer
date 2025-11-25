#shader vertex
#version 330 core

layout(location = 0) in vec4 triangle_coordinates;

void main() {
   gl_Position = triangle_coordinates;
}

#shader fragment 
#version 330 core

uniform vec4 u_Color;

layout(location = 0) out vec4 triangle_color;

void main() {
   triangle_color = u_Color;
}
