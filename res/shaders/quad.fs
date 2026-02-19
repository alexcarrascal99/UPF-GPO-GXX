varying vec2 v_uv;
uniform float u_aspect;
uniform int u_exercise; 
uniform int u_subtask;  

void main()
{
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

			vec3 verdeFondo = vec3(0.0, 0.25, 0.0);
			vec3 verdeOnda  = vec3(0.0, 0.9, 0.0);

			verdeFondo *= grad;
			verdeOnda  *= grad;

			float amplitud = 0.22;
			float frecuencia = 2.0;

			float onda = 0.5 + amplitud * sin((uv.x / u_aspect) * 6.28318 * frecuencia);

			float mascara = step(uv.y, onda);

			color = mix(verdeFondo, verdeOnda, mascara);
		}
	}
    else if (u_exercise == 1) 
    {
		color = vec3(0.0); 
    }

    gl_FragColor = vec4(color, 1.0);
}
