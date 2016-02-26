#define PI (3.1415926536)
#define TWO_PI (6.2831853072)

uniform float elapsedTime;
uniform float alpha;
uniform vec2 resolution;
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
varying vec3 position, normal;
varying float randomOffset;

const vec4 on = vec4(1.);
const vec4 off = vec4(vec3(0.), 1.);


float noise = 1.0;

float speed = 2.0;
float height = 1.0;
float gamma = 1.0;
float contrast = 1.5;

const float waves = 19.;

// triangle wave from 0 to 1
float wrap(float n) {
  return abs(mod(n, 2.)-1.)*-1. + 1.;
}

// creates a cosine wave in the plane at a given angle
float wave(float angle, vec2 point) {
  float cth = cos(angle);
  float sth = sin(angle);
  return (cos (cth*point.x + sth*point.y) + 1.) / 0.2;
}

// sum cosine waves at various interfering angles
// wrap values when they exceed 1
float quasi(float interferenceAngle, vec2 point) {
  float sum = 0.;
  for (float i = 0.; i < waves; i++) {
    sum += wave(3.1416*i*interferenceAngle, point);
  }
  return wrap(sum);
}




uniform vec2 lightPosXY;
uniform int aaSteps;

// exponent. higher=more regular-looking.
// 0 = turbulent and chaotic
// 5 starts to look quite regular (triangles tessellated in equal parts)
float regularity = sin(elapsedTime *0.0001 * speed) + noise;
//float regularity = noise;

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

    float heightChange = startingHeight ;
    for(int i = 0; i < val4*30.; ++ i)
    {
        float ab = distance(a+abs(cos(elapsedTime*100.0)),b);
        float bc = distance(b,c+abs(cos(elapsedTime*100.0)));
        float ca = distance(c+abs(cos(elapsedTime*100.0)),a);
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
        pos = pow(pos, 2.0) * 2.0 ; //+cos(elapsedTime);
        pos = (pos + 1.) / 2.0;// pos is now 0-1        
        vec3 d = mix(a, b, pos);
        d.z += heightChange * ((rand(pos) * 2.0) - 1.0 );// find random height for the new point
        if(IsPointInTriangle(uv, a.xy, d.xy, c.xy))// triangle 1: ADC
            b = d;
        else// triangle 2: DBC
            a = d;

        // honestly i would expect 0.5 to work here but it really is way too subtle. i don't really understand why.
        heightChange *= 0.7;
    }
        
    return calcNormalForTriangle(b,c,a);
}



void main() {
	float stages = 6.;
	float stage =round(val1*7.0);//mod(elapsedTime * .6, stages);// 0.; //

	if(stage == 0.) {
		vec2 normPosition =position.xy  * (10000.0*val2);/// resolution.xy/100.;/+ position.yz)
		float b = quasi(elapsedTime*0.002, (normPosition)*val1*2);
		gl_FragColor = vec4(vec3(b), 1.);
		//gl_FragColor = vec4(1.0,1.0,0.0, 1.0);
	} else if(stage < 1.) {
		// diagonal stripes
		const float speed = 50.;
		const float scale = 1.;
		gl_FragColor = vec4(vec3(1., 1., 0.), 1.);
	} else if(stage < 2.) {
		// crazy color bounce
		gl_FragColor = vec4(clamp(mod(elapsedTime + position * 100., 1.) * sin(mod(elapsedTime * 2., TWO_PI)),vec3(0.0),vec3(1.0)), 1.);
	} else if(stage < 3.) {
		// fast rising stripes
		//if(normal.z == 0.) {
			const float speed = 0.0001;
			 float scale = 0.01 * val2;
			gl_FragColor = 
				(mod((-position.x) + (elapsedTime * speed), scale) < (scale / 2.)) ?
				on : off;
		/*} else {
			gl_FragColor = off;
		}*/
	} else if(stage < 5.) {
		// GRID
		float speed = 0.003*val3;
		float scale = 0.01*(val2*10.0);	
		float cutoff = val4;
		vec3 cur = mod(position + speed * elapsedTime, scale) / scale;
		cur *= 1. - abs(normal);
		if(stage < 4.) {
vec4 f=vec4(0.0,0.0,0.0,1.0);
vec2 c=vec2(position.xy*90.);
      if( mod( c.y , 1. ) < 1. ){
        f.z=elapsedTime*0.05,
        f.xy = vec2(
                    c.x / (80. + sin(       f.z      ) * 48.),
                    c.y / (90. + cos( 15. + f.z * 1.5) * 38.)
                   ),
        f = vec4(
                        sin( f.z * -1.8 + f.x *  .5) + cos( f.z * 2.5 + f.y * 2.5 ) + cos(     -f.z + f.y * 2.5),
                        cos( f.z        + f.x * 3. ) + atan(f.z *-.72  + f.y       ) + sin(5. +  f.z  + f.x ),
                        sin( f.z *  2.3 + f.y * 3. ) + cos( f.z * 0.7 + f.y *-3.1 ) + cos(      f.z  + f.x ),
            			0
                     )
               - sin( 5. + f.z * 3.1 + c.y * .01 ) 
               + cos( f.z * .57 + c.x * .0075) * 2. 
               + sin( -f.z * 0.2 + c.x * .005) * 2. 
               - cos( -f.z + c.y * .015) * 0.45;
           }
			gl_FragColor = (max(max(cur.x, cur.y), cur.z) < cutoff) ? off : on;
			gl_FragColor *=f;
			//gl_FragColor = ((cur.x + cur.y + cur.z) < cutoff) ? off : on;
		} else {
			gl_FragColor = (max(max(cur.x, cur.y), cur.z) < cutoff) ? off : on;
		}
	} else if(stage < 6.) {
		// moving outlines
		float speed = 1000. *val3;
		float scale = 6000. *val2;
		float localTime = 5. * randomOffset + elapsedTime;
		gl_FragColor = 
			(mod((-position.x - position.y + position.z) + (localTime * speed), scale) > scale / 2.) ?
			on : off;
	} else if(stage < 7.) {


	 vec2 uv = position.xy*(val2*5.0);//gl_FragCoord.xy;*90000
    vec2 lightPosXY = vec2(val4,val3);

    vec4 col = vec4(vec3(0.5), 1.0);// background +cos(elapsedTime*100.0)
    vec3 normal = vec3(0.);
    vec3 a, b, c;// triangle points

    // top-level rectangle size. we will manually divide the rect into 2 triangles.
    vec2 tileSize = resolution.xy / 1.0;
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
    	normal = getNormalForTopLevelTriangle(a, b, c, uv, debug1, initialVertexHeight);
    }
    else
    {
        a = vec3(tilePosition.x+tileSize.x, tilePosition.y, 0.0);
        b = vec3(tilePosition.x+tileSize.x, tilePosition.y+tileSize.y, 0.0);
        c = vec3(tilePosition.x, tilePosition.y+tileSize.y, 0.0);
        normal = getNormalForTopLevelTriangle(a, b, c, uv, debug1, initialVertexHeight);
    }
    
    // lighting
    vec3 lightPos = vec3(lightPosXY, resolution.x / 2.);
    vec4 lightColor = vec4(1.0*val3,1.0*val3,1.0*val3,1.);
    vec4 ambientColor = vec4(0.2,0.2,0.24,1.);
    col = applyLighting(col, uv, vec3(normal), lightPos, lightColor, ambientColor);
    col.rgb = pow(abs(col.rgb), vec3(1.0 / gamma));
    col.rgb = adjust_contrast(col.rgb, vec4(contrast));
    gl_FragColor = col;
	}

	gl_FragColor.a=alpha;
}
