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

vec4 ColorAmbient(vec4 matColor, vec4 lightColor){
    return Times(matColor, lightColor);
}


vec4 ColorDiffuse(in vec4 matColor,
                  in vec4 location, 
                  in vec3 normal,
                  in vec4 lightColor,
                  in vec4 lightSource){
    vec3 lightDirection = normalize(vec3(lightSource - location));

    return Times(matColor, lightColor)*max( dot(lightDirection, normal), 0.0f );
}

vec4 ColorSpecular(in vec4 matColor, 
				   in float shininess, 
				   in vec4 camLocation,
                   in vec4 location, 
                   in vec3 normal,
                   in vec4 lightColor,
                   in vec4 lightSource){
    vec4 lightDirection =  lightSource - location;
    lightDirection.w=0;
    lightDirection = normalize(lightDirection);
    vec4 v = normalize(camLocation-location);
    vec3 halfVector = vec3(lightDirection+v);
    halfVector = normalize(halfVector);
    return Times(matColor, lightColor) * pow( max(dot(halfVector, normal), 0.0f) , shininess);
}



layout (location = 0) in  vec3 vPosition;
layout (location = 1) in  vec3 vNormal;
layout (location = 2) in  vec2 texCoords;




uniform mat4 projection;
uniform mat4 v;
uniform mat4 mv;
uniform mat4 mv_normal;

uniform Material material;
uniform Light Lights[3];
uniform vec4 cameraLocation;

out vec2 tCoord;
out vec4 vColor;
out vec3 normal;

void main()
{

    tCoord = texCoords;

    vec4 location = mv * vec4(vPosition,1.0f);

    normal = normalize(vec3(mv_normal*vec4(vNormal,0.0f)));
	//normal = normalize(vec3(vNormal));

	vColor = material.emission;

    for(int light =0; light < 3; ++light){
    	if(!Lights[light].on) continue;

    	switch(Lights[light].type){
    	case 0: // Ambient
    		vColor+=ColorAmbient(material.ambient, Lights[light].color); break;
    	case 1: break;// Point
    	case 2: // Diffuse
    		vColor+=ColorDiffuse(material.diffuse, 
    							 location,
    							 normal,
    							 Lights[light].color,
    							 Lights[light].source); break;
    	case 3: // Specular
			vColor+=ColorSpecular(material.specular, 
							      material.shininess, 
							      cameraLocation,
					              location, 
					              normal,
					              Lights[light].color,
					              Lights[light].source); break;
    	case 4: break;// Spotlight
    	case 5: break;// Distant
    	}

    }
    vColor = min(vColor,vec4(1.0f));


    gl_Position = projection*location;
}
