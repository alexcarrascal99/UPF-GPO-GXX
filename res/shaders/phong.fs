uniform sampler2D u_color_texture;
uniform sampler2D u_normal_texture;

uniform vec3 u_camera_position;
uniform vec3 u_light_position;
uniform vec3 u_light_color;
uniform vec3 u_ambient_light;

uniform mat4 u_model;

varying vec3 v_world_position;
varying vec3 v_world_normal;
varying vec2 v_uv;

void main()
{
    vec4 texColor = texture2D(u_color_texture, v_uv);

    vec3 Ka = texColor.rgb;
    vec3 Kd = texColor.rgb;

    float spec = texColor.a;
    vec3 Ks = vec3(spec);

    vec3 normalTex = texture2D(u_normal_texture, v_uv).rgb;
    normalTex = normalTex * 2.0 - 1.0;
    normalTex = normalize((u_model * vec4(normalTex, 0.0)).xyz);

    vec3 vertexNormal = normalize(v_world_normal);
    vec3 N = normalize(mix(vertexNormal, normalTex, 0.3));

    vec3 L = normalize(u_light_position - v_world_position);
    vec3 V = normalize(u_camera_position - v_world_position);
    vec3 R = reflect(-L, N);

    vec3 Ia = u_ambient_light * Ka;

    float diff = max(dot(N,L),0.0);
    vec3 Id = u_light_color * Kd * diff;

    float specular = pow(max(dot(R,V),0.0), 32.0);
    vec3 Is = u_light_color * Ks * specular;

    vec3 color = Ia + Id + Is;

    gl_FragColor = vec4(color,1.0);
}