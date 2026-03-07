varying vec2 v_uv;
uniform float u_aspect;
uniform int u_exercise; 
uniform int u_subtask;  
uniform sampler2D u_texture;
uniform float u_time;

void main()
{
    vec2 uv_tex = v_uv;
    vec2 uv = v_uv;
    uv.x *= u_aspect;
    vec3 color = vec3(0.0); 
    if (u_exercise == 0) 
    {
        if (u_subtask == 0) { 
            vec3 azul = vec3(0.0, 0.0, 1.0);
            vec3 rojo = vec3(1.0, 0.0, 0.0);
            color = mix(azul, rojo, uv.x);
        }
        else if (u_subtask == 1) {
            vec2 centro = vec2(0.5 * u_aspect, 0.5);
            float d = distance(uv, centro);
            color = vec3(d * 0.5);
        }
        else if (u_subtask == 2) {
			float rejillaX = sin(uv.x * 20.0);
            float rejillaY = sin(uv.y * 20.0);
            color = vec3(rejillaX, 0.0, rejillaY);
        }

		else if (u_subtask == 3) {
			float grid = 20.0;
    		vec2 uvPix = floor(uv * grid) / grid;
			float r = uvPix.x / u_aspect;
			float g = uvPix.y;
			float b = 0.0;
    		color = vec3(r, g, b);
		}

		else if (u_subtask == 4) {
			float escala = 10.0;
    		float cx = floor(uv.x * escala);
    		float cy = floor(uv.y * escala);
   		    float checker = mod(cx + cy, 2.0);
    		color = vec3(checker);
		}

		else if (u_subtask == 5) {
			float grad = smoothstep(0.0, 1.0, uv.y);

			vec3 verdeFondo = vec3(0.0, 0.15, 0);
			vec3 verdeOnda  = vec3(0.0, 0.9, 0.0);

			verdeFondo /= grad;
			verdeOnda  *= grad;

			float amplitud = 0.22;
			float frecuencia = 2.0;

			float onda = 0.5 + amplitud * sin((uv.x / u_aspect) * 6.28318 * frecuencia);

			float mascara = step(uv.y, onda);

			color = mix(verdeFondo, verdeOnda, mascara);
		}
        gl_FragColor = vec4(color, 1.0);
	}
else if (u_exercise == 1) 
{
    vec3 tex = texture2D(u_texture, uv_tex).rgb;
        if (u_subtask == 0) {
            float gray = dot(tex, vec3(0.299, 0.587, 0.114));
            color = vec3(gray);
        }   
        else if (u_subtask == 1) {
            color = 1.0 - tex;
        }
        else if (u_subtask == 2) {
            vec3 yellow = vec3(1,1,0.0);
            float gray = dot(tex, vec3(0.299, 0.587, 0.114));
            color = vec3(gray) * yellow;
        }
        else if (u_subtask == 3) {
            float gray = dot(tex, vec3(0.299, 0.587, 0.114));
            float bw = step(0.5, gray);
            color = vec3(bw);
        }
        else if(u_subtask == 4){
            float d = distance(uv_tex, vec2(0.5,0.5));
            color = vec3(tex) * (1.0 - 1.5*d);
        }
        else if(u_subtask == 5){
            color =  tex + 
            texture2D(u_texture, uv_tex + vec2(0.01,0)).rgb + 
            texture2D(u_texture, uv_tex - vec2(0.01,0.00)).rgb+
            texture2D(u_texture, uv_tex + vec2(0.00,0.01)).rgb +
            texture2D(u_texture, uv_tex - vec2(0.00,0.01)).rgb;
            color /= 5.0;
        }
        gl_FragColor = vec4(color, 1.0);
}
else if(u_exercise == 2){
     vec3 tex = texture2D(u_texture, uv_tex).rgb;
    if (u_subtask == 0)
    {
        float blocks = 30.0 + 10.0 * abs(sin(u_time));
        vec2 uv_pixel = floor(uv_tex * blocks) / blocks;
        color = texture2D(u_texture, uv_pixel).rgb;
    }
    else if(u_subtask == 1){
     float blocks = 250.0;
     vec2 uv_pixel = floor(uv_tex * blocks) / blocks;
     uv_pixel.y += 0.03 * sin(uv.x * 20.0 + u_time * 3.0);
     color = texture2D(u_texture, uv_pixel).rgb;
    }

     gl_FragColor = vec4(color, 1.0);

}
}