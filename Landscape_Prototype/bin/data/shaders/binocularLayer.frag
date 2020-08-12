#version 150


uniform float uTime;
uniform vec2 uMouse;
uniform vec2 uResolution;
uniform float uAlpha;
uniform float uMagnification;
uniform sampler2DRect tex0;

uniform vec2 uPosition;

out vec4 fragColor;

float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
                 43758.5453123);
}

float map(float value, float min1, float max1, float min2, float max2){
	float perc = (value - min1) / (max1 - min1);
	return perc * (max2 - min2) + min2;
}

vec2 map(vec2 value, vec2 min1, vec2 max1, vec2 min2, vec2 max2){
	vec2 perc = (value - min1) / (max1 - min1);
	return perc * (max2 - min2) + min2;
}

vec2 invY(vec2 v){
    return vec2(v.x, 1.0f - v.y);
}

void main(){

    vec2 eyeCenter = uPosition;

    //normalized frag coords
    vec2 normCoord = vec2(gl_FragCoord.x/uResolution.x, gl_FragCoord.y/uResolution.y);

    // tie this to float from OF to scale whole thing up/down
    float bScale = 1.0;

    // vals in pixels
    float eyeRad = 180.0 * bScale;
    float eyeWidth = 220.0 * bScale;
    float border = 30 * bScale;
    vec2 leftPos = eyeCenter - vec2(eyeWidth / 2.0,0);
    vec2 rightPos = eyeCenter + vec2(eyeWidth / 2.0,0);

    float leftDist = distance(gl_FragCoord.xy, leftPos);
    float rightDist = distance(gl_FragCoord.xy, rightPos);
    
    vec4 outputColor = vec4( 0.0, 0.0, 0.0, 0.0);

    vec4 overlay = vec4(0,0,0, 0.6);
    
    
    // inside binoculars
    float borderDist = eyeRad + border;
    if ( leftDist < eyeRad || rightDist < eyeRad){

        float magnification = 2.0;
        vec2 rawImgRes = vec2(4912, 2763);

        vec2 eyeCenterNorm = eyeCenter / uResolution;
        vec2 eyeCenterNormInvY = invY(eyeCenterNorm);
        vec2 magCenter = eyeCenterNormInvY * rawImgRes;

        // offset by center of the binoculars in the raw image space

        vec2 fragToEyeCenter = gl_FragCoord.xy - eyeCenter;
        // invert the relative coord too
        fragToEyeCenter *= vec2(1.0, -1.0);

        vec2 imgCoord = magCenter + fragToEyeCenter / uMagnification;

        vec4 img = texture(tex0, imgCoord);
        outputColor = img;

        //outputColor = vec4(eyeCenterNorm.y, 0,0,1);

    } else if(leftDist < borderDist || rightDist < borderDist) {
        // border
        float distToEdgeNorm;

        if(gl_FragCoord.x < eyeCenter.x) {
            distToEdgeNorm = (leftDist - eyeRad) / border;
        } else {
            distToEdgeNorm = (rightDist - eyeRad) / border;
        }

        vec4 borderCol = vec4(vec3(0.5), overlay.a);
        outputColor = mix(borderCol, overlay, distToEdgeNorm);
        //outputColor = vec4(1,0,1,1);

    } else {
    
        // background

        outputColor = overlay;
    }

    //if (distance(gl_FragCoord.xy, uMouse) < 100) outputColor = vec4(1,0,0,1);


    fragColor = outputColor;
    fragColor.a *= uAlpha;

}







