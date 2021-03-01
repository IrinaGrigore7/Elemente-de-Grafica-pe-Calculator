#version 330

// TODO: get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture;
//layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 object_color;

// TODO: output values to fragment shader
out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_texture;
out vec3 frag_color;
  float tangentFactor = 0.1;
  vec3 orthogonal(vec3 v) {
    return normalize(abs(v.x) > abs(v.z) ? vec3(-v.y, v.x, 0.0)
    : vec3(0.0, -v.z, v.y));
  }

  // Any function can go here to distort p
  vec3 distorted(vec3 p) {
    return p + vec3(
      cnoise(p / 3.0),
      cnoise(p / 4.0 + 1.0),
      cnoise(p / 5.0 + 2.0)
    );
  }


void main()
{
    vec3 distortedPosition = distorted(v_position);
    vec3 tangent1 = orthogonal(v_normal);
    vec3 tangent2 = normalize(cross(v_normal, tangent1));
    vec3 nearby1 = position + tangent1 * tangentFactor;
    vec3 nearby2 = position + tangent2 * tangentFactor;
    vec3 distorted1 = distorted(nearby1);
    vec3 distorted2 = distorted(nearby2);
    v_normal = normalize(cross(distorted1 - distortedPosition, distorted2 - distortedPosition));
	// TODO: send output to fragment shader
	frag_position = distortedPosition;
	frag_normal = v_normal;
	frag_texture = v_texture;
	frag_color = object_color;
	// TODO: compute gl_Position
	 gl_Position = Projection * View * Model * vec4(distortedPosition, 1.0);
}
