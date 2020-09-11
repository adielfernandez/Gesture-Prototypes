#version 150


uniform float uTime;
uniform vec2 uMouse;
uniform vec2 uResolution;
uniform vec2 uLeft;
uniform vec2 uRight;

uniform sampler2DRect tex1;
uniform sampler2DRect tex2;
uniform sampler2DRect tex3;
uniform sampler2DRect tex4;
uniform sampler2DRect tex5;

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

//------------------------------------------------------------
//------------------------------------------------------------
//	Simplex 3D Noise
//	by Ian McEwan, Ashima Arts
//------------------------------------------------------------
//------------------------------------------------------------
vec4 permute(vec4 x){return mod(((x*34.0)+1.0)*x, 289.0);}
vec4 taylorInvSqrt(vec4 r){return 1.79284291400159 - 0.85373472095314 * r;}

// Varies from -1 to 1
float snoise(vec3 v){
    const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
    const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);
    
    // First corner
    vec3 i  = floor(v + dot(v, C.yyy) );
    vec3 x0 =   v - i + dot(i, C.xxx) ;
    
    // Other corners
    vec3 g = step(x0.yzx, x0.xyz);
    vec3 l = 1.0 - g;
    vec3 i1 = min( g.xyz, l.zxy );
    vec3 i2 = max( g.xyz, l.zxy );
    
    //  x0 = x0 - 0. + 0.0 * C
    vec3 x1 = x0 - i1 + 1.0 * C.xxx;
    vec3 x2 = x0 - i2 + 2.0 * C.xxx;
    vec3 x3 = x0 - 1. + 3.0 * C.xxx;
    
    // Permutations
    i = mod(i, 289.0 );
    vec4 p = permute( permute( permute(
                                       i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
                              + i.y + vec4(0.0, i1.y, i2.y, 1.0 ))
                     + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));
    
    // Gradients
    // ( N*N points uniformly over a square, mapped onto an octahedron.)
    float n_ = 1.0/7.0; // N=7
    vec3  ns = n_ * D.wyz - D.xzx;
    
    vec4 j = p - 49.0 * floor(p * ns.z *ns.z);  //  mod(p,N*N)
    
    vec4 x_ = floor(j * ns.z);
    vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)
    
    vec4 x = x_ *ns.x + ns.yyyy;
    vec4 y = y_ *ns.x + ns.yyyy;
    vec4 h = 1.0 - abs(x) - abs(y);
    
    vec4 b0 = vec4( x.xy, y.xy );
    vec4 b1 = vec4( x.zw, y.zw );
    
    vec4 s0 = floor(b0)*2.0 + 1.0;
    vec4 s1 = floor(b1)*2.0 + 1.0;
    vec4 sh = -step(h, vec4(0.0));
    
    vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
    vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;
    
    vec3 p0 = vec3(a0.xy,h.x);
    vec3 p1 = vec3(a0.zw,h.y);
    vec3 p2 = vec3(a1.xy,h.z);
    vec3 p3 = vec3(a1.zw,h.w);
    
    //Normalise gradients
    vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
    p0 *= norm.x;
    p1 *= norm.y;
    p2 *= norm.z;
    p3 *= norm.w;
    
    // Mix final noise value
    vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
    m = m * m;
    return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1),
                                 dot(p2,x2), dot(p3,x3) ) );
}

// -------------------------------------------------------

// Modulo 289 without a division (only multiplications)
vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

// Modulo 7 without a division
vec3 mod7(vec3 x) {
  return x - floor(x * (1.0 / 7.0)) * 7.0;
}

// Permutation polynomial: (34x^2 + x) mod 289
vec3 permute(vec3 x) {
  return mod289((34.0 * x + 1.0) * x);
}

void main(){
    //normalized frag coords
    vec2 normCoord = vec2(gl_FragCoord.x/uResolution.x, gl_FragCoord.y/uResolution.y);

    // use left and right edges of cross section to determine where we are in perlin space
    float scaleXY = 0.1f;
    float scaleZ = 0.0f;

    // walk a line from left to right based on normCoord.x
    vec2 scaledLeft = uLeft * scaleXY;
    vec2 scaledRight = uRight * scaleXY;

    vec2 leftToRight = scaledRight - scaledLeft;
    vec2 noiseCoord = scaledLeft + leftToRight * normCoord.x;

   float z = 300;
    // ----- First Layer -----
    float n1Scale = 0.05f;
    float n1 = snoise(vec3(noiseCoord.x * n1Scale, noiseCoord.y * n1Scale, z));
    vec4 img1 = texture(tex1, gl_FragCoord.xy);
    vec4 layer1Col = img1; // vec4(0, 0.5, 1, 1);
    float layer1Level = map(n1, -1, 1, 0.05, 0.3);

    // ----- Second Layer -----
    float n2Scale = 0.01f;
    float n2 = snoise(vec3(noiseCoord.x * n2Scale, noiseCoord.y * n2Scale, z * 2));
    vec4 img2 = texture(tex2, gl_FragCoord.xy);
    vec4 layer2Col = img2; // vec4(1, 0.5, 0, 1);
    float layer2Level = map(n2, -1, 1, 0.1, 0.2);

    // ----- Third Layer -----
    float n3Scale = 0.01f;
    float n3 = snoise(vec3(noiseCoord.x * n3Scale, noiseCoord.y * n3Scale, z * 3));
    vec4 img3 = texture(tex4, gl_FragCoord.xy);
    vec4 layer3Col = img3; // vec4(1, 0, 1, 1);
    float layer3Level = map(n3, -1, 1, 0.1, 0.2);

    // ----- Fourth Layer -----
    float n4Scale = 0.01f;
    float n4 = snoise(vec3(noiseCoord.x * n4Scale, noiseCoord.y * n4Scale, z * 4));
    vec4 img4 = texture(tex3, gl_FragCoord.xy);
    vec4 layer4Col = img4; // vec4(1, 1, 0, 1);
    float layer4Level = map(n4, -1, 1, 0.1, 0.2);

    // ----- Fifth Layer -----
    float n5Scale = 0.5f;
    float n5 = snoise(vec3(noiseCoord.x * n5Scale, noiseCoord.y * n5Scale, z * 5));
    vec4 img5 = texture(tex5, gl_FragCoord.xy);
    vec4 layer5Col = img5; // vec4(1, 0, 0, 1);
    float layer5Level = map(n5, -1, 1, 0.1, 0.2);

    if (normCoord.y < layer1Level) {
        fragColor = layer1Col;    
    } else if (normCoord.y < layer1Level + layer2Level) {
        fragColor = layer2Col;
    } else if (normCoord.y < layer1Level + layer2Level + layer3Level) {
        fragColor = layer3Col;
    } else if (normCoord.y < layer1Level + layer2Level + layer3Level + layer4Level) {
        fragColor = layer4Col;
    }  else {
        fragColor = layer5Col;
    }

    
    //if (normCoord.x < 0.9 ) fragColor = vec4(1, 1, 0, 1);
}







