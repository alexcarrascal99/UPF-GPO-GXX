uniform mat4 u_model;
uniform mat4 u_viewprojection;

varying vec3 v_world_position;
varying vec3 v_world_normal;
varying vec2 v_uv;

void main()
{
    vec4 worldPos = u_model * vec4(gl_Vertex.xyz, 1.0);

    v_world_position = worldPos.xyz;
    v_world_normal = normalize(mat3(u_model) * gl_Normal);
    v_uv = gl_MultiTexCoord0.xy;

    gl_Position = u_viewprojection * worldPos;
}