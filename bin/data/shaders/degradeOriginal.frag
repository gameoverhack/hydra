uniform sampler2DRect ImageTexture;

uniform float rWidth;
uniform float rHeight;
uniform vec3 rColor1;
uniform vec3 rColor2;

float rand(vec2 n){
    return 0.5 + 0.5 *
    fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}

// Function to get a texel data from a texture with GL_NEAREST property.
// Bi-Linear interpolation is implemented in this function with the
// help of nearest four data.
vec4 tex2DBiLinear( sampler2DRect textureSampler_i, vec2 texCoord_i, float fWidth, float fHeight ){
	float texelSizeX = 1.0 / fWidth; //size of one texel
	float texelSizeY = 1.0 / fHeight; //size of one texel

	int nX = int( texCoord_i.x * fWidth );
	int nY = int( texCoord_i.y * fHeight );
	vec2 texCoord_New = vec2( ( float( nX ) + 0.5 ) / fWidth,
							  ( float( nY ) + 0.5 ) / fHeight );
	// Take nearest two data in current row.
    vec4 p0q0 = texture2DRect(textureSampler_i, texCoord_New);
    vec4 p1q0 = texture2DRect(textureSampler_i, texCoord_New + vec2(texelSizeX, 0));

    float m = rand(p0q0.xy) / 1.5;

//    p0q0.r *= rand(rColor1.xy);
//    p0q0.g *= rand(p0q0.xy);
//    p0q0.b *= rand(rColor1.xy);

//    p1q0.r *= rand(p0q0.xy);
//    p1q0.g *= rand(rColor1.xy);
//    p1q0.b *= rand(p0q0.xy);

//    if(rColor1.z > 0.0 && rColor1.z < 0.3){
//        p0q0.r *= rand(rColor1.xy);
//        p0q0.g *= rand(p0q0.xy);
//        p0q0.b *= rand(rColor1.xy);
//
//        p1q0.r *= rand(p0q0.xy);
//        p1q0.g *= rand(rColor1.xy);
//        p1q0.b *= rand(p0q0.xy);
//    }else if(rColor1.z > 0.3 && rColor1.z < 0.6){
//        p0q0.r *= rand(p0q0.xy);
//        p0q0.g *= rand(rColor1.xy);
//        p0q0.b *= rand(rColor1.xy);
//
//        p1q0.r *= rand(rColor1.xy);
//        p1q0.g *= rand(p0q0.xy);
//        p1q0.b *= rand(rColor1.xy);
//    }else{
//        m = 0.6;
//        p0q0.r *= rand(rColor1.xy);
//        p0q0.g *= rand(rColor1.xy);
//        p0q0.b *= rand(rColor1.xy);
//
//        p1q0.r *= rand(p0q0.xy);
//        p1q0.g *= rand(rColor1.xy);
//        p1q0.b *= rand(rColor1.xy);
//    }


	// Take nearest two data in bottom row.
    vec4 p0q1 = texture2DRect(textureSampler_i, texCoord_New + vec2(0, texelSizeY));
    vec4 p1q1 = texture2DRect(textureSampler_i, texCoord_New + vec2(texelSizeX , texelSizeY));
//
//    p0q1.r *= rand(rColor2.xy);
//    p0q1.g *= rand(rColor2.xy);
//    p0q1.b *= rand(p0q0.xy);

//    p1q1.r *= rand(p0q0.xy);
//    p1q1.g *= rand(rColor2.xy);
//    p1q1.b *= rand(p0q0.xy);

//    if(rColor2.z > 0.0 && rColor2.z < 0.3){
//        p0q1.r *= rand(rColor2.xy);
//        p0q1.g *= rand(rColor2.xy);
//        p0q1.b *= rand(p0q0.xy);
//
//        p1q1.r *= rand(p0q0.xy);
//        p1q1.g *= rand(rColor2.xy);
//        p1q1.b *= rand(p0q0.xy);
//    }else if(rColor2.z > 0.3 && rColor2.z < 0.6){
//        p0q1.r *= rand(p0q0.xy);
//        p0q1.g *= rand(rColor2.xy);
//        p0q1.b *= rand(rColor2.xy);
//
//        p1q1.r *= rand(rColor2.xy);
//        p1q1.g *= rand(p0q0.xy);
//        p1q1.b *= rand(rColor2.xy);
//    }else{
//        m = 0.6;
//        p0q1.r *= rand(rColor2.xy);
//        p0q1.g *= rand(rColor2.xy);
//        p0q1.b *= rand(rColor2.xy);
//
//        p1q1.r *= rand(rColor2.xy);
//        p1q1.g *= rand(p0q0.xy);
//        p1q1.b *= rand(rColor2.xy);
//    }



    float a = fract( texCoord_i.x * fWidth ); // Get Interpolation factor for X direction.
											 // Fraction near to valid data.

	// Interpolation in X direction.
    vec4 pInterp_q0 = mix( p0q0, p1q0, a ); // Interpolates top row in X direction.
    vec4 pInterp_q1 = mix( p0q1, p1q1, a ); // Interpolates bottom row in X direction.

    float b = fract( texCoord_i.y * fHeight ); // Get Interpolation factor for Y direction.

    vec4 original = texture2DRect(textureSampler_i, texCoord_i);
    vec4 finale = mix(mix(original, pInterp_q0, m), mix(original, pInterp_q1, m),   b);//mix( pInterp_q0, pInterp_q1, b ); // Interpolate in Y direction.

    //mix (texture2DRect(textureSampler_i, texCoord_i), finale, 0.7)

    return finale;
    //return texture2DRect(textureSampler_i, texCoord_i);
}



// Shader for creating GL_LINEAR interpolation in shader.
void main()
{
    vec4 Data;
//    if(rColor1.z > 0.6){
//        Data = tex2DBiLinear( ImageTexture, gl_TexCoord[0].st , rWidth+0.05, rHeight+0.05);
//    }else{
        Data = tex2DBiLinear( ImageTexture, gl_TexCoord[0].st , rWidth*6.0, rHeight*6.0);
//    }

	gl_FragColor = Data;
}

