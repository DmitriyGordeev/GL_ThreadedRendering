#version 400

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 color;
uniform sampler2D textureSampler;

void main()
{
	vec4 textureColor = texture(textureSampler, fragmentUV);
	// color = fragmentColor * textureColor;
	color = textureColor;

	// color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
