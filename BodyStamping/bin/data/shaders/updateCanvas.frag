#version 150


uniform float uTime;
uniform vec2 uMouse;
uniform vec2 uResolution;
uniform sampler2DRect tex0;

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

float mapFn(float value, float min1, float max1, float min2, float max2){
	float perc = (value - min1) / (max1 - min1);
	return perc * (max2 - min2) + min2;
}

void main(){
    //normalized frag coords
    vec2 normCoord = vec2(gl_FragCoord.x/uResolution.x, gl_FragCoord.y/uResolution.y);

    vec4 img = texture(tex0, gl_FragCoord.xy);

    float scale = 20.0;
    float speed = 0.1;
    vec2 nCoord = normCoord * vec2(scale);

    // varies from -1 to 1
    vec2 displacement = vec2( snoise(vec3( nCoord.x, nCoord.y, uTime * speed )), 
                              snoise(vec3( nCoord.x, nCoord.y, uTime * speed + 1000 )) );


    // increase displacement in areas of low luminance
    float luminance = (0.2126 * img.r + 0.7152 * img.g + 0.0722 * img.b) * img.a;
    float dispAmt = max( map(luminance, 0.0, 0.3, 3.0, 0.0), 0.0);
    displacement *= vec2(dispAmt); 

    vec2 dispCoord = gl_FragCoord.xy + displacement;

    vec4 imgDisplaced = texture(tex0, dispCoord);

    // base alpha decay on dist from center (0 at center, 1.0 at edge)
    float dist = distance(normCoord, vec2(0.5)) / 0.5;

    float alphaDecay = map(dist, 0, 1.0, 0.004, 0.01);
    imgDisplaced.a = clamp( imgDisplaced.a - alphaDecay , 0.0, 1.0);

    

    fragColor = imgDisplaced;


    //fragColor = vec4(vec3(n), 1);
}







