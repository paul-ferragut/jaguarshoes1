// https://www.shadertoy.com/view/Mt2SzR
#version 120

const float pi = 3.14159265;
uniform vec2 resolution;
uniform vec3 color1;
uniform vec3 color2;
uniform float time;
uniform float noiseLevel;

float random(float x) {
 
    return fract(sin(x) * 10000.);
          
}

float noise(vec2 p) {

    return random(p.x + p.y * 10000.);
            
}

vec2 sw(vec2 p) { return vec2(floor(p.x), floor(p.y)); }
vec2 se(vec2 p) { return vec2(ceil(p.x), floor(p.y)); }
vec2 nw(vec2 p) { return vec2(floor(p.x), ceil(p.y)); }
vec2 ne(vec2 p) { return vec2(ceil(p.x), ceil(p.y)); }

float smoothNoise(vec2 p) {

    vec2 interp = smoothstep(0., 1., fract(p));
    float s = mix(noise(sw(p)), noise(se(p)), interp.x);
    float n = mix(noise(nw(p)), noise(ne(p)), interp.x);
    return mix(s, n, interp.y);
        
}

float fractalNoise(vec2 p) {

    float x = 0.;
    x += smoothNoise(p      );
    x += smoothNoise(p * 2. ) / 2.;
    x += smoothNoise(p * 4. ) / 4.;
    x += smoothNoise(p * 8. ) / 8.;
    x += smoothNoise(p * 16.) / 16.;
    x /= 1. + 1./2. + 1./4. + 1./8. + 1./16.;
    return x;
            
}

float movingNoise(vec2 p) {
 
    float x = fractalNoise(p + (time*0.4));
    float y = fractalNoise(p - (time*0.4));
    return fractalNoise(p + vec2(x, y));   
    
}

// call this for water noise function
float nestedNoise(vec2 p) {
    
    float x = movingNoise(p);
    float y = movingNoise(p + 100.);
    return movingNoise(p + vec2(x, y));
    
}
void main()
{
    vec2 uv = gl_FragCoord.xy / resolution.xy;
    float n = nestedNoise(uv * 6.);
    
    //fragColor = vec4(mix(vec3(.9, .0, 0.1), vec3(.1, .2, 1.), n), 1.);
    
    vec4    color=vec4(mix(color1, color2, n), 1.);
    
     float strength = 10.0+(30.0*noiseLevel);
    
    float x = (uv.x + 4.0 ) * (uv.y + 4.0 ) * (time * 10.0);
    vec4 grain = vec4(mod((mod(x, 13.0) + 1.0) * (mod(x, 123.0) + 1.0), 0.01)-0.005) * strength;
    
  
    
   
        grain = 1.0 - grain;
        vec4 colorT=color * grain;
        colorT.a=0.75;
        gl_FragColor = colorT;

    
    
}