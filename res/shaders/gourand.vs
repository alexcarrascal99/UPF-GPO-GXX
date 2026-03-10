uniform mat4 u_model;
uniform mat4 u_viewprojection;

uniform vec3 u_camera_position;
uniform vec3 u_ambient_light;
uniform vec3 u_light_position;
uniform vec3 u_light_color;

uniform vec4 u_material_ambient;
uniform vec4 u_material_diffuse;
uniform vec4 u_material_specular;
uniform float u_shininess;

varying vec4 v_color;

void main()
{
    vec3 world_position = (u_model * vec4(gl_Vertex.xyz, 1.0)).xyz;
    vec3 N = normalize((u_model * vec4(gl_Normal.xyz, 0.0)).xyz);
    vec3 L = normalize(u_light_position - world_position);
    vec3 V = normalize(u_camera_position - world_position);
    vec3 R = reflect(-L, N);

    vec3 Ia = u_ambient_light * u_material_ambient.rgb;

    float diff = max(dot(N, L), 0.0);
    vec3 Id = u_light_color * u_material_diffuse.rgb * diff;

    float spec = pow(max(dot(R, V), 0.0), u_shininess);
    vec3 Is = u_light_color * u_material_specular.rgb * spec;

    vec3 Ip = Ia + Id + Is;

    v_color = vec4(Ip, 1.0);

    gl_Position = u_viewprojection * vec4(world_position, 1.0);
}