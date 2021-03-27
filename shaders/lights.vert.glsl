#version 330 core


struct Light{
	int type;
	vec4 source;
	vec4 color;
	bool on;
};
/*
    AMBIENT,	0
    POINT,	1
    DIFFUSE,	2
    SPECULAR,	3
    SPOTLIGHT,	4
    DISTANT	5

*/

struct Material{
	vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 emission;
    float shininess;
};

vec4 Times(vec4 m, vec4 l){
	return vec4(m.x*l.x, m.y*l.y, m.z*l.z, 1.0f);
}

vec4 ColorAmbient(vec4 matColor, vec4 lightColor)
{
    return Times(matColor, lightColor);
}


vec4 ColorDiffuse(in vec4 mat_color,
                  in vec4 world_location, 
                  in vec3 normal,
                  in vec4 light_color,
                  in vec4 light_source,
                  in mat4 view_matrix)
{
    vec4 light_vector = (view_matrix*light_source - world_location); //view_matrix*
    vec3 light_direction = normalize(light_vector.xyz);

    return Times(mat_color, light_color)*max( dot(light_direction, normal), 0.0f );
}

vec4 ColorSpecular(in vec4 mat_color, 
				   in float shininess, 
				   in vec4 cam_location,
                   in vec4 world_location, 
                   in vec3 normal,
                   in vec4 light_color,
                   in vec4 light_source,
                   in mat4 view_matrix)
{
    vec4 light_vector = (view_matrix*light_source - world_location);  //view_matrix*
    vec3 light_direction = normalize(light_vector.xyz);
    // view_matrix*cam_location = 0
    vec3 camera_direction = normalize( (view_matrix*cam_location).xyz-world_location.xyz);
    vec3 half_vector = vec3(light_direction+camera_direction);
    half_vector = normalize(half_vector);
    return Times(mat_color, light_color) * pow( max(dot(half_vector, normal), 0.0f) , shininess);
}



layout (location = 0) in  vec3 vPosition;
layout (location = 1) in  vec3 vNormal;
uniform mat4 projection;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
uniform mat4 mvp;
uniform mat4 mv_normal;
uniform bool EmissiveOn;

uniform Material material;
uniform Light Lights[3];
uniform vec4 camera_location;


out vec4 vColor;
out vec3 normal;

void main()
{
    mat4 mv = view_matrix * model_matrix;

    vec4 world_position = mv*vec4(vPosition,1.0f);
    world_position /= world_position.w;
    gl_Position = mvp * vec4(vPosition,1.0f);

    normal = normalize(vec3(mv_normal*vec4(vNormal,0.0f)));
	//normal = normalize(vec3(vNormal));

	if(EmissiveOn) vColor = material.emission;
	else vColor = vec4(0.0f);

    for(int light =0; light < 3; ++light){
    	if(!Lights[light].on) continue;

    	switch(Lights[light].type){
    	case 0: // Ambient
    		vColor+=ColorAmbient(material.ambient, Lights[light].color); break;
    	case 1: break;// Point
    	case 2: // Diffuse
    		vColor+=ColorDiffuse(material.diffuse, 
    							 world_position,
    							 normal,
    							 Lights[light].color,
    							 Lights[light].source,
                                 view_matrix); break;
    	case 3: // Specular
			vColor+=ColorSpecular(material.specular, 
							      material.shininess, 
							      camera_location,
					              world_position, 
					              normal,
					              Lights[light].color,
					              Lights[light].source,
                                  view_matrix); break;
    	case 4: break;// Spotlight
    	case 5: break;// Distant
    	}

    }
    vColor = min(vColor,vec4(1.0f));
}
