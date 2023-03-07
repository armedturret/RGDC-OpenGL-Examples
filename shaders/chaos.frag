#version 330 core
in vec2 texCoord;
in vec3 fragCoord;

out vec4 FragColor;

uniform sampler2D texUniform;
uniform int pulse;
uniform float time;

void main()
{
	vec4 mult = vec4(1.0);
	
	if(pulse == 1){
		mult = vec4(vec3(sin(time * 2.0f) * 0.25 + 0.5), 1.0) * vec4(0.0f, 1.0f, 0.0f, 1.0f);
	}else{
		float hue = fragCoord.y + time;
		while(hue > 1.0f)
			hue -= 1.0f;
		float s = 1.0f;
		float v = 1.0f;
		
		float H = hue * 360.0f;
		float C = s*v;
		float X = C*(1-abs(mod(H/60.0, 2)-1));
		float m = v-C;
		float r,g,b;
		if(H >= 0 && H < 60){
			r = C,g = X,b = 0;
		}
		else if(H >= 60 && H < 120){
			r = X,g = C,b = 0;
		}
		else if(H >= 120 && H < 180){
			r = 0,g = C,b = X;
		}
		else if(H >= 180 && H < 240){
			r = 0,g = X,b = C;
		}
		else if(H >= 240 && H < 300){
			r = X,g = 0,b = C;
		}
		else{
			r = C,g = 0,b = X;
		}
		mult = vec4(r + m, g + m, b + m, 1.0);
	}

	FragColor = mult * texture(texUniform, texCoord);
}