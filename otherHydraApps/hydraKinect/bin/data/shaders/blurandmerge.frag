uniform sampler2DRect blurmask;
uniform sampler2DRect imagesource;
void main(void) {
    vec4 imageTexel = texture2DRect(imagesource, gl_TexCoord[0].xy);
    vec4 maskTexel = texture2DRect(blurmask, gl_TexCoord[0].xy);
    gl_FragColor = maskTexel.rgba * imageTexel.rgba;
}