#version 150
//The fragment shader operates on each pixel in a given polygon

//This is the 3 component float vector that gets outputted to the screen
//for each pixel.
in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 color;

uniform sampler2D mySampler;

void main() {
    //Just hardcode the color to red
    vec4 textureColor = texture(mySampler, fragmentUV);
    
    //Make crazy colors using time and position!
    color = fragmentColor * textureColor;
}