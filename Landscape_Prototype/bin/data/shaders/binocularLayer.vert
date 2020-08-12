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
    
    //gl_TexCoord[0] = gl_MultiTexCoord0;
    
    //get our current vertex position so we can modify it
   // vec4 pos = modelViewProjectionMatrix * position;
    
//     vec2 m = mouse - resolution/2.0;
    
// //    vec2 distToCenter = distance(vec2( pos.x, pos.y), resolution/2.0);
//     float mouseToVert = length(distance(m, pos.xy));
    
//     if( mouseToVert < 100 ){
        
//         pos.y += 100;
        
//         gl_FrontColor = vec4(1.0, 0.0, 0.0, 1.0);
        
//     } else {
//         gl_FrontColor = gl_Color;
//     }
 
    //finally set the pos to be that actual position rendered
    gl_Position =  position + vec4(-uResolution / 2.0,0,0);
    
    
}
