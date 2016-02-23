#define PI (3.1415926536)
#define TWO_PI (6.2831853072)

uniform float elapsedTime;
uniform bool wrap;
uniform vec2 resolution;
varying vec3 position, normal;
varying float randomOffset;
uniform float val1;
uniform float val2;
uniform float val3;
uniform float val4;
uniform float val5;
uniform float val6;
uniform float val7;
uniform float val8;
uniform float val9;
uniform float val10;


/*

float noise = 1.0;
float speed = 1.0;
float height = 1.0;
float gamma = 1.0;
float contrast = 1.5;
	
uniform vec2 lightPosXY;
uniform int aaSteps;

// exponent. higher=more regular-looking.
// 0 = turbulent and chaotic
// 5 starts to look quite regular (triangles tessellated in equal parts)
//float regularity = sin(time *0.000001 * speed) + noise;
float regularity = noise;

float initialVertexHeight = resolution.x * height;


// Real contrast adjustments by  Miles
vec3 adjust_contrast(vec3 col, vec4 con)
{
vec3 c = con.rgb * vec3(con.a);
vec3 t = (vec3(1.0) - c) / vec3(2.0);
t = vec3(.18);

col = (1.0 - c.rgb) * t + c.rgb * col;

return col;
}


// -------------------------------------------------------
// utilities
float rand(float n)
{
 	return fract(cos(n*89.42)*343.42);
}
vec2 rand(vec2 n)
{
 	return vec2(rand(n.x*23.62-300.0+n.y*34.35),rand(n.x*45.13+256.0+n.y*38.89)); 
}
vec3 rand(vec3 n)
{
 	return vec3(rand(n.xy), rand(n.z));
}
float rand1(vec2 n) {
	
  return fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}

vec4 applyLighting(vec4 inpColor, vec2 uv, vec3 normal, vec3 LightPos, vec4 LightColor, vec4 AmbientColor)
{
    if(distance(uv.xy, LightPos.xy) < 0.01) return vec4(1.,0.,0.,1.);
    vec3 LightDir = vec3(LightPos.xy - uv, LightPos.z);
    float D = length(LightDir);// distance for attenuation
    vec3 N = normalize(normal);
    vec3 L = normalize(LightDir);
    vec3 Diffuse = (LightColor.rgb * LightColor.a) * max(dot(N, L), 0.0);
    vec3 Ambient = AmbientColor.rgb * AmbientColor.a;
    vec3 Intensity = Ambient + Diffuse ;
    vec3 FinalColor = inpColor.rgb * Intensity;
    return vec4(FinalColor, inpColor.a);
}
vec3 calcNormalForTriangle(vec3 a, vec3 b, vec3 c)
{
    vec3 dir = cross(b - a, c - a);
	vec3 normal = normalize(dir);
    return normal;
}
float sgn(vec2 p1, vec2 p2, vec2 p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}
bool IsPointInTriangle (vec2 pt, vec2 v1, vec2 v2, vec2 v3)
{
    bool b1, b2, b3;
    b1 = sgn(pt, v1, v2) < 0.0;
    b2 = sgn(pt, v2, v3) < 0.0;
    b3 = sgn(pt, v3, v1) < 0.0;
    return ((b1 == b2) && (b2 == b3)) ;
}


// -------------------------------------------------------
// the algorithm

// calculates the normal within the given triangle. iteration is done inside here.
// only call this if you know that uv is within the specified triangle.
vec3 getNormalForTopLevelTriangle(vec3 a, vec3 b, vec3 c, vec2 uv, out float debug, float startingHeight)
{
    // points come in with no height info, so generate them.
    a.z = rand1(a.xy);
    b.z = rand1(b.xy);
    c.z = rand1(c.xy);
    
    debug = 0.0;

    float heightChange = startingHeight;
    for(int i = 0; i < 12; ++ i)
    {
        float ab = distance(a,b);
        float bc = distance(b,c);
        float ca = distance(c,a);
        // split the longest segment in 2. we will first have to rotate the
        // triangle points so the longest segment is AB. makes subdividing easier later.
	    vec3 temp;
        if(ca > ab && ca > bc)
        {
            // ca is longest; rotate to CAB
            temp = c;
            c = b;
            b = a;
            a = temp;
        }
        else if(bc > ca && bc > ab)
        {
            // bc is longest; rotate to BCA
            temp = a;
            a = b;
            b = c;
            c = temp;
        }
        
        // find a random point between A and B, creating 2 new sub-triangles
        // favor the center of the segment. it makes sense because heightChange creates an angle you don't want to be too steep.
        float pos = rand(distance(a,b)) - 0.5;// from -0.5 to 0.5
        // very important: this pushes the point towards 0 (the center of the segment).
        // thus, the higher the exponent, the more things look like a triangle grid.
        pos = pow(pos, regularity) * 2.0;
        pos = (pos + 1.) / 2.0;// pos is now 0-1        
        vec3 d = mix(a, b, pos);
        d.z += heightChange * ((rand(pos) * 2.0) - 1.0);// find random height for the new point
        if(IsPointInTriangle(uv, a.xy, d.xy, c.xy))// triangle 1: ADC
            b = d;
        else// triangle 2: DBC
            a = d;

        // honestly i would expect 0.5 to work here but it really is way too subtle. i don't really understand why.
        heightChange *= 0.7;
    }
        
    return calcNormalForTriangle(b,c,a);
}


// -------------------------------------------------------

void main()
{

		float speed = 300.*val3;
		float scale = 800.0*val2;	
	vec3 uv = mod(position + speed * elapsedTime, scale) / scale;
		uv *= 1. - abs(normal);
	//vec2 uv = position.yz/4000.0*3.0- vec2(15.0);
    //vec2 uv = gl_FragCoord.xy;
    vec2 lightPosXY = vec2(val1,val2);

    vec4 col = vec4(vec3(0.5), 1.0);// background

    vec3 normal2 = vec3(0.);
    vec3 a, b, c;// triangle points

    // top-level rectangle size. we will manually divide the rect into 2 triangles.
    vec2 tileSize = val1;//resolution.xy / 1.0;
    //vec2 tilePosition = floor(uv / tileSize) * tileSize;// snap to grid of size
	vec2 tilePosition = floor(uv / tileSize) * tileSize;// snap to grid of size
    // the goal of everything here is to compute the normal of this fragment.
    // make sure these are clockwise for normal calculation
    a = vec3(tilePosition.x, tilePosition.y, 0.0);
    b = vec3(tilePosition.x+tileSize.x, tilePosition.y, 0.0);
    c = vec3(tilePosition.x, tilePosition.y+tileSize.y, 0.0);

	
    float debug1, debug2;
    if(IsPointInTriangle(uv, a.xy, b.xy, c.xy))
    {
    	normal2 = getNormalForTopLevelTriangle(a, b, c, uv, debug1, initialVertexHeight);
    }
    else
    {
        a = vec3(tilePosition.x+tileSize.x, tilePosition.y, 0.0);
        b = vec3(tilePosition.x+tileSize.x, tilePosition.y+tileSize.y, 0.0);
        c = vec3(tilePosition.x, tilePosition.y+tileSize.y, 0.0);
        normal2 = getNormalForTopLevelTriangle(a, b, c, uv, debug1, initialVertexHeight);
    }
    
    // lighting
    vec3 lightPos = vec3(lightPosXY, resolution.x / 2.);
    vec4 lightColor = vec4(1.0,1.0,1.0,1.);
    vec4 ambientColor = vec4(0.2,0.2,0.2,1.);
    col = applyLighting(col, uv, vec3(normal2), lightPos, lightColor, ambientColor);
    col.rgb = pow(abs(col.rgb), vec3(1.0 / gamma));
    col.rgb = adjust_contrast(col.rgb, vec4(contrast));
    gl_FragColor = col;
}

*/



// -------------------------------------------------------
// parameters...

// 0 = random triangle color
// 1 = stylized, based on distance to triangle
// 2 = reveal raw distance to triangle
// 3 = no color, only bump map
#define COLOR_MODE 1

const int iterationCount = 14;

const float iterationColorInfluenceFactor = 0.75;// how much do we attenuate color every iteration

// exponent. higher=tries to keep triangles equal sized. more regular-looking.
// 0 = turbulent and chaotic
// 5 starts to look quite "regular"
const float regularity = 2.2;

float initialVertexHeight = resolution.x * 0.05;// basically, how much height variation. how bumpy.
const float iterationNormalInfluenceFactor = 0.7;
const float heightRandomness = 1.;// 0 = don't randomize heights; make them look uniform by just adding the same every iteration. 1=completely random.


// -------------------------------------------------------
// utilities
float saturate(float n) { return clamp(n,0.,1.); }

// c64 palette because why not.
vec3 color0 = vec3(0,0,0);// black
vec3 color1 = vec3(1,1,1);// white
vec3 color2 = vec3(0.41,0.22,0.17);// red
vec3 color3 = vec3(0.44,0.64,0.70);// cyan
vec3 color4 = vec3(0.44,0.24,0.53);// violet
vec3 color5 = vec3(0.35,0.55,0.26);// green
vec3 color6 = vec3(0.21,0.16,0.47);// blue
vec3 color7 = vec3(0.72,0.78,0.44);// yellow
vec3 color8 = vec3(0.44,0.31,0.15);// orange
vec3 color9 = vec3(0.26,0.22,0);// brown
vec3 colorA = vec3(0.60,0.40,0.35);// light red
vec3 colorB = vec3(0.27,0.27,0.27);// grey1
vec3 colorC = vec3(0.42,0.42,0.42);// grey2
vec3 colorD = vec3(0.60,0.82,0.52);// light green
vec3 colorE = vec3(0.42,0.37,0.71);// light blue
vec3 colorF = vec3(0.58,0.58,0.58);// grey3
vec3 getPalette(int i)
{
    if(i == 0) return color6;
    if(i == 1) return color3;
    if(i == 2) return color5;
    if(i == 3) return color9;
    if(i == 4) return color7;
    return color8;
}




float rand(float n)
{
 	return fract(cos(n*89.42)*343.42);
}
vec2 rand(vec2 n)
{
 	return vec2(rand(n.x*23.62-300.0+n.y*34.35),rand(n.x*45.13+256.0+n.y*38.89)); 
}
vec3 rand(vec3 n)
{
 	return vec3(rand(n.xy), rand(n.z));
}
float rand1(vec2 n) {
	
  return fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}
float rand1(vec3 n) {
	return rand1(vec2(rand1(n.xy), n.z));
}
float nsin(float a)
{
    return (sin(a)+1.)/2.;
}
vec4 applyLighting(vec4 inpColor, vec2 uv, vec3 normal, vec3 LightPos, vec4 LightColor, vec4 AmbientColor)
{
    if(distance(uv.xy, LightPos.xy) < 0.01) return vec4(1.,0.,0.,1.);
    vec3 LightDir = vec3(LightPos.xy - uv, LightPos.z);
    float D = length(LightDir);// distance for attenuation
    vec3 N = normalize(normal);
    vec3 L = normalize(LightDir);
    vec3 Diffuse = (LightColor.rgb * LightColor.a) * max(dot(N, L), 0.0);
    vec3 Ambient = AmbientColor.rgb * AmbientColor.a;
    vec3 Intensity = Ambient + Diffuse;
    vec3 FinalColor = inpColor.rgb * Intensity;
    return vec4(FinalColor, inpColor.a);
}
vec3 calcNormalForTriangle(vec3 a, vec3 b, vec3 c)
{
    vec3 dir = cross(b - a, c - a);
	vec3 normal = normalize(dir);
    return normal;
}
float sgn(vec2 p1, vec2 p2, vec2 p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}
bool IsPointInTriangle (vec2 pt, vec2 v1, vec2 v2, vec2 v3)
{
    bool b1, b2, b3;
    b1 = sgn(pt, v1, v2) < 0.0;
    b2 = sgn(pt, v2, v3) < 0.0;
    b3 = sgn(pt, v3, v1) < 0.0;
    return ((b1 == b2) && (b2 == b3));
}

// THANK YOU IQ / https://www.shadertoy.com/view/XsXSz4
// signed distance to a 2D triangle
float sdTriangle( in vec2 p0, in vec2 p1, in vec2 p2, in vec2 p )
{
	vec2 e0 = p1 - p0;
	vec2 e1 = p2 - p1;
	vec2 e2 = p0 - p2;

	vec2 v0 = p - p0;
	vec2 v1 = p - p1;
	vec2 v2 = p - p2;

	vec2 pq0 = v0 - e0*clamp( dot(v0,e0)/dot(e0,e0), 0.0, 1.0 );
	vec2 pq1 = v1 - e1*clamp( dot(v1,e1)/dot(e1,e1), 0.0, 1.0 );
	vec2 pq2 = v2 - e2*clamp( dot(v2,e2)/dot(e2,e2), 0.0, 1.0 );
    
    vec2 d = min( min( vec2( dot( pq0, pq0 ), v0.x*e0.y-v0.y*e0.x ),
                       vec2( dot( pq1, pq1 ), v1.x*e1.y-v1.y*e1.x )),
                       vec2( dot( pq2, pq2 ), v2.x*e2.y-v2.y*e2.x ));

	return -sqrt(d.x)*sign(d.y);
}

// -------------------------------------------------------
// the algorithm

// calculates the normal & coloring within the given triangle. iteration is done inside here.
// only call this if you know that uv is within the specified triangle.
vec4 doStuffForThisTriangle(vec3 a, vec3 b, vec3 c, vec2 uv, out vec3 normal)
{
    // points come in with no height info, so generate them.
    a.z = rand1(a.xy) * initialVertexHeight;
    b.z = rand1(b.xy) * initialVertexHeight;
    c.z = rand1(c.xy) * initialVertexHeight;
    vec4 fragColor = vec4(vec3(0.5),1.0);
    
    float colorMix = 1.0;
    float heightAffect = initialVertexHeight;
    for(int i = 0; i < iterationCount; ++ i)
    {
        // the idea: split this triangle in 2, by finding the longest segment
        // and splitting it. To do this, first find the longest segment,
        // then rotate the triangle vertices so AB is the longest segment.
        float ab = distance(a.xy,b.xy);
        float bc = distance(b.xy,c.xy);
        float ca = distance(c.xy,a.xy);
	    vec3 temp = c;
        if(ca > ab && ca > bc)
        {
            // ca is longest; rotate to CAB
            c = b;
            b = a;
            a = temp;
        }
        else if(bc > ab)
        {
            // bc is longest; rotate to BCA
            c = a;
            a = b;
            b = temp;
        }

        // find a random point between A and B to split at, creating 2 new sub-triangles
        float pos = rand1(a.xy + b.xy) - 0.5;// from -0.5 to 0.5
        // this pushes the point towards 0 (the center of the segment).
        float sgn = sign(pos);// workaround for negative values in pow() - thanks for FabriceNeyret2 for pointing out this bug.
        // thus, the higher the exponent, the more things look like a regular triangle grid.
        pos = pow(abs(pos), regularity) * 2.0 * sgn;
        pos = (pos + 1.) / 2.0;// pos is now 0-1, favoring 0.5
        vec3 d = mix(a, b, pos);
        
        // perturb the new point's height. one way to make this look really accurate
        // would be to consider the xy position and the angle it will form with other vertices.
        float randomHeight = (rand1(a.xy + b.xy + pos) * 2.0) - 1.0;// -1 to 1.
        randomHeight *= heightAffect;
        d.z += mix(heightAffect, randomHeight, heightRandomness);// find random height for the new point
        
        float distToEdge = sdTriangle(a.xy, d.xy, c.xy, uv);// distance from uv to the triangle
        if(distToEdge < 0.)// triangle 1: ADC
            b = d;
        else
        {
            // triangle 2: DBC
            a = d;
			distToEdge = sdTriangle(a.xy, b.xy, c.xy, uv);
        }

        // colorize this pixel...
#if COLOR_MODE == 0
        float tintAmt = rand1(a.xy+b.xy+c.xy);// pow(dist, 0.2);
        fragColor = vec4(mix(fragColor.rgb, getPalette(i), tintAmt * colorMix), 1.0);
#endif
#if COLOR_MODE == 1 || COLOR_MODE == 2
        // we know the distance to the edge.
        // using the center of the triangle we can get a gradient from 0-1 from edge to center
        vec2 center = (a.xy + b.xy + c.xy) / 3.0;
        float distToCenter = distance(uv, center);
        distToEdge = -distToEdge;
        float tintAmt = distToEdge / (distToCenter + distToEdge);
        tintAmt = pow(tintAmt, 1.2);
        tintAmt = 1.0 - tintAmt;
#if COLOR_MODE == 1
        fragColor = vec4(mix(fragColor.rgb, getPalette(i), tintAmt * colorMix), 1.0);
#endif
#if COLOR_MODE == 2
        fragColor = vec4(vec3(tintAmt),1.);
#endif
#endif
        // honestly i would expect 0.5 to work here but it really is way too subtle. i don't really understand why.
		heightAffect *= iterationNormalInfluenceFactor;
        colorMix *= iterationColorInfluenceFactor;
    }

    // i thought i kept everything clockwise, not sure why i need to flip this.
    normal = -calcNormalForTriangle(a,b,c);
    return fragColor;
}




// -------------------------------------------------------

void main( )
{



		float speed = 300.*val3;
		float scale = 1000.0*val2;	
	//vec2 uv = mod(position + speed * elapsedTime, scale) / scale;
	//	uv *= 1. - abs(normal);

    vec2 scrollOffset = vec2(elapsedTime * val4/ 17., 0.0);
   // vec2 uv = /position.xy ;//+ scrollOffset;
  vec2   uv = mod(position.xy + speed * elapsedTime, scale);// +scrollOffset;
    	uv *= 1. - abs(normal);
  
    vec2 lightPosXY = vec2(nsin(elapsedTime * 1.66), nsin(elapsedTime * 1.99));
    lightPosXY = (lightPosXY * 0.75) + 0.125;// not so big movement
    lightPosXY *= resolution.xy;
    lightPosXY += scrollOffset;
    if(val1 > 0.)
	    lightPosXY = vec2(val1,val2) + scrollOffset;
    
    gl_FragColor = vec4(vec3(0.5), 1.0);// background

    vec3 normal = vec3(0.);
    vec3 a, b, c;// triangle points

    // top-level rectangle size. we will manually divide the rect into 2 triangles.
    vec2 tileSize = val1*10.0;
    vec2 tilePosition = floor(uv / tileSize) * tileSize;// snap to grid of size

    // the goal of everything here is to compute the normal of this fragment.
    // make sure these are clockwise for normal calculation
    a = vec3(0., tileSize.y, 0.0);
    b = vec3(tileSize.x, tileSize.y, 0.0);
    c = vec3(0., 0., 0.0);
    a.xy += tilePosition;
    b.xy += tilePosition;
    c.xy += tilePosition;
    if(IsPointInTriangle(uv, a.xy, b.xy, c.xy))
    {
    	gl_FragColor = doStuffForThisTriangle(a, b, c, uv, normal);
    }
    else
    {
        a = vec3(0.,0., 0.0);
        b = vec3(tileSize.x, tileSize.y, 0.0);
        c = vec3(tileSize.x, 0., 0.0);
        a.xy += tilePosition;
        b.xy += tilePosition;
        c.xy += tilePosition;
       	gl_FragColor = doStuffForThisTriangle(a, b, c, uv, normal);
    }
    
    // lighting
#if COLOR_MODE != 2
    vec3 lightPos = vec3(lightPosXY, resolution.x / 2.);
    vec4 lightColor = vec4(0.95,0.9,0.9,1.);
    vec4 ambientColor = vec4(0.5,0.5,0.9,1.);
	gl_FragColor = applyLighting(gl_FragColor, uv, normal, lightPos, lightColor, ambientColor);
    
    // show light position.
    if(distance(uv, lightPosXY) < resolution.x / 75.)
        gl_FragColor = vec4(1.0,1.0,0.,1.0);
#endif
}
