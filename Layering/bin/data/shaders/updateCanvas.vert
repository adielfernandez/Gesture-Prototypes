#version 150

uniform float uTime;
uniform vec2 uMouse;
uniform vec2 uResolution;

in vec4 position;
in vec4 color;
in vec4 normal;
in vec2 texcoord;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

void main(void)
{
    
    //finally set the pos to be that actual position rendered
    gl_Position =  position + vec4(-uResolution / 2.0,0,0);
    
    
}
