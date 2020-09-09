#version 150


uniform float uTime;
uniform vec2 uMouse;
uniform vec2 uResolution;
uniform sampler2DRect tex0;
uniform vec4 uColor;

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
    //normalized frag coords
    //vec2 normCoord = vec2(gl_FragCoord.x/uResolution.x, gl_FragCoord.y/uResolution.y);

    // incoming image is grayscale, but make the 
    // image transparent based on red channel
    vec4 img = texture(tex0, gl_FragCoord.xy);
    float alpha = img.r;

    fragColor = vec4(uColor.rgb, alpha);

}







