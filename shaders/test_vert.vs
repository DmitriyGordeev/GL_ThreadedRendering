#version 400

in vec2 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;

out vec2 fragmentPosition;
out vec4 fragmentColor;
out vec2 fragmentUV;

// uniform mat4 P;

void main()
{
    //vec4 pos = P * vec4(vertexPosition, 0.0, 1.0);
    //pos.z = 0.0f;
    //pos.w = 1.0f;
    //gl_Position = pos;

	// gl_Position.xy = (P * vec4(vertexPosition, 0.0, 1.0)).xy;
	// gl_Position.z = 0.0;
	// gl_Position.w = 1.0;

	gl_Position = vec4(vertexPosition, 0.0, 1.0);

	fragmentPosition = vertexPosition;
	fragmentColor = vertexColor;
	fragmentUV = vec2(vertexUV.x, 1.0 - vertexUV.y);
}