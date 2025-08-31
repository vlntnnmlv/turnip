#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform float time;

out vec4 finalColor;

void main()
{
    float wave = 0.5 + 0.5 * sin(time + gl_FragCoord.x * 0.05);
    finalColor = vec4(fragColor.rgb * wave, fragColor.a);
}
