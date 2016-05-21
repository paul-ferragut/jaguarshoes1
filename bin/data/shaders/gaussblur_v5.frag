// Blur shader from: http://callumhay.blogspot.ca/2010/09/gaussian-blur-shader-glsl.html
#version 120

#define VERTICAL_BLUR_5


uniform float sigma;     // The sigma value for the gaussian function: higher value means more blur
// A good value for 9x9 is around 3 to 5
// A good value for 7x7 is around 2.5 to 4
// A good value for 5x5 is around 2 to 3.5
// ... play around with this based on what you need :)

uniform float blurSize;  // This should usually be equal to
// 1.0 / texture_pixel_width for a horizontal blur, and
// 1.0 / texture_pixel_height for a vertical blur.

uniform sampler2DRect blurSampler;  // Texture that will be blurred by this shader

const float pi = 3.14159265;


uniform vec2 posLight;

uniform vec2 resolution;




// The following are all mutually exclusive macros for various 
// seperable blurs of varying kernel size
#if defined(VERTICAL_BLUR_9)
const float numBlurPixelsPerSide = 4.0;
const vec2  blurMultiplyVec      = vec2(0.0, 1.0);
#elif defined(HORIZONTAL_BLUR_9)
const float numBlurPixelsPerSide = 4.0;
const vec2  blurMultiplyVec      = vec2(1.0, 0.0);
#elif defined(VERTICAL_BLUR_7)
const float numBlurPixelsPerSide = 3.0;
const vec2  blurMultiplyVec      = vec2(0.0, 1.0);
#elif defined(HORIZONTAL_BLUR_7)
const float numBlurPixelsPerSide = 3.0;
const vec2  blurMultiplyVec      = vec2(1.0, 0.0);
#elif defined(VERTICAL_BLUR_5)
const float numBlurPixelsPerSide = 2.0;
const vec2  blurMultiplyVec      = vec2(0.0, 1.0);
#elif defined(HORIZONTAL_BLUR_5)
const float numBlurPixelsPerSide = 2.0;
const vec2  blurMultiplyVec      = vec2(1.0, 0.0);
#else
// This only exists to get this shader to compile when no macros are defined
const float numBlurPixelsPerSide = 0.0;
const vec2  blurMultiplyVec      = vec2(0.0, 0.0);
#endif

float blendLighten(float base, float blend) {
    return max(blend,base);
}

vec3 blendLighten(vec3 base, vec3 blend) {
    return vec3(blendLighten(base.r,blend.r),blendLighten(base.g,blend.g),blendLighten(base.b,blend.b));
}

vec3 blendLighten(vec3 base, vec3 blend, float opacity) {
    return (blendLighten(base, blend) * opacity + blend * (1.0 - opacity));
}

float blendAdd(float base, float blend) {
    return min(base+blend,1.0);
}

vec3 blendAdd(vec3 base, vec3 blend) {
    return min(base+blend,vec3(1.0));
}

vec3 blendAdd(vec3 base, vec3 blend, float opacity) {
    return (blendAdd(base, blend) * opacity + blend * (1.0 - opacity));
}

vec3 blendMultiply(vec3 base, vec3 blend) {
    return base*blend;
}

vec3 blendMultiply(vec3 base, vec3 blend, float opacity) {
    return (blendMultiply(base, blend) * opacity + blend * (1.0 - opacity));
}

float blendDarken(float base, float blend) {
    return min(blend,base);
}

vec3 blendDarken(vec3 base, vec3 blend) {
    return vec3(blendDarken(base.r,blend.r),blendDarken(base.g,blend.g),blendDarken(base.b,blend.b));
}

vec3 blendDarken(vec3 base, vec3 blend, float opacity) {
    return (blendDarken(base, blend) * opacity + blend * (1.0 - opacity));
}

float blendScreen(float base, float blend) {
    return 1.0-((1.0-base)*(1.0-blend));
}

vec3 blendScreen(vec3 base, vec3 blend) {
    return vec3(blendScreen(base.r,blend.r),blendScreen(base.g,blend.g),blendScreen(base.b,blend.b));
}

vec3 blendScreen(vec3 base, vec3 blend, float opacity) {
    return (blendScreen(base, blend) * opacity + blend * (1.0 - opacity));
}


void main() {
    
    // Incremental Gaussian Coefficent Calculation (See GPU Gems 3 pp. 877 - 889)

    vec2 normalisedPos=posLight/resolution;//;pos/resolution;//

    vec2 uv = gl_FragCoord.xy / resolution.xy;
    vec2 trans = uv -normalisedPos;//- 1.02.0 * uv -
    float part = -dot(trans, trans) + 1.0;
    //float part2 = -dot(trans, trans) - 1.0;
    float calc =  smoothstep(0.8, 1.0, part);
    float calc2 =  smoothstep(0.95, 1.0, part);


    vec3 incrementalGaussian;
    incrementalGaussian.x = 1.0 / (sqrt(2.0 * pi) * sigma);
    incrementalGaussian.y = exp(-0.5 / (sigma * sigma));
    incrementalGaussian.z = incrementalGaussian.y * incrementalGaussian.y;
    
    vec4 avgValue = vec4(0.0, 0.0, 0.0, 0.0);
    float coefficientSum = 0.0;
    
    // Take the central sample first...
    avgValue += texture2DRect(blurSampler, gl_TexCoord[0].xy) * incrementalGaussian.x;
    coefficientSum += incrementalGaussian.x;
    incrementalGaussian.xy *= incrementalGaussian.yz;
    
    // Go through the remaining 8 vertical samples (4 on each side of the center)1.0-
//blurSize=(calc*2.0)*blurSize;
float blurSizeCopy=blurSize;
blurSizeCopy*=(1.0-calc2);

    for (float i = 1.0; i <= numBlurPixelsPerSide; i++) { 
        avgValue += texture2DRect(blurSampler, gl_TexCoord[0].xy - i * blurSizeCopy * 
        blurMultiplyVec) * incrementalGaussian.x;         
        avgValue += texture2DRect(blurSampler, gl_TexCoord[0].xy + i * blurSizeCopy * 
        blurMultiplyVec) * incrementalGaussian.x;         
        coefficientSum += 2.0 * incrementalGaussian.x;
        incrementalGaussian.xy *= incrementalGaussian.yz;
    }
    vec4 fragC =  avgValue / coefficientSum;


    //float rangeY=radiusP[i]*(resolution.x/resolution.y);
    //posLight

    
//+ay*ay
vec3 blend= blendMultiply(vec3(fragC.rgb),vec3(texture2DRect(blurSampler, uv)),1.0);//min(fragC+blend,1.0);

calc-=blend.r;
    gl_FragColor =vec4(vec3(0.0,0.0,0.0),calc);//1.0-



// gl_FragColor = texture2DRect(blurSampler, gl_TexCoord[0].xy);
}