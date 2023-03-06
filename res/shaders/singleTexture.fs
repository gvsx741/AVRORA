#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{            
	vec4 tex = texture(texture1, TexCoords);
	if(tex.a < 0.1){
		discard;
	}
    FragColor = tex;
}