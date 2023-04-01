#version 330

in vec4 vColor;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

out vec4 color;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

struct Light {
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight {
    Light base;
    vec3 direction;
};

struct PointLight {
    Light base;
    vec3 position;
    float constant;
    float linear;
    float exponent;
};

struct SpotLight {
    PointLight base;
    vec3 direction;
    float edge;
};

struct Material {
    float specularIntensity;
    float shininess;
};

uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D texSamp;
uniform Material material;

uniform vec3 eyePosition;

vec4 CalcLightByDirection(Light light, vec3 direction) {

    vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;

    float diffuseFactor = max(dot(normalize(normal), normalize(direction)), 0.0f);
    vec4 diffuseColor = vec4(light.color, 1.0f) * light.diffuseIntensity * diffuseFactor;

    vec4 specularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    if (diffuseFactor > 0.0f) {

        vec3 fragToEye = normalize(eyePosition - fragPos);
	    vec3 reflectedVertex = normalize(reflect(direction, normalize(normal)));

        float specularFactor = dot(fragToEye, reflectedVertex);

	    if (specularFactor > 0.0f) {

	        specularFactor = pow(specularFactor, material.shininess);
	        specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
	    }
    }

    return (ambientColor + diffuseColor + specularColor);
}

vec4 CalcDirectionalLight() {
    return CalcLightByDirection(directionalLight.base, directionalLight.direction);
}

vec4 CalcPointLight(PointLight light) {

    vec3 direction = fragPos - light.position;
	float distance = length(direction);
	direction = normalize(direction);

	vec4 color = CalcLightByDirection(light.base, direction);
	float attenuation = light.exponent * distance * distance +
		light.linear * distance +
		light.constant;

	return (color / attenuation);
}

vec4 CalcSpotLight(SpotLight light) {

    vec4 color = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    vec3 rayDirection = normalize(fragPos - light.base.position);
    float factor = dot(rayDirection, light.direction);

    if (factor > light.edge) {
        color = CalcPointLight(light.base) * (1.0f - (1.0f - factor) / (1.0f - light.edge));
    }

    return color;
}

vec4 CalcPointLights() {

    vec4 pointLightsColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    for (int i = 0; i < pointLightCount; i++) {
	    pointLightsColor += CalcPointLight(pointLights[i]);
    }
    
    return pointLightsColor;
}

vec4 CalcSpotLights() {

    vec4 spotLightsColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    for (int i = 0; i < spotLightCount; i++) {
	    spotLightsColor += CalcSpotLight(spotLights[i]);
    }
    
    return spotLightsColor;
}

void main() {

    vec4 lightColor = CalcDirectionalLight();
    lightColor += CalcPointLights();
    lightColor += CalcSpotLights();

    color = texture(texSamp, texCoord) * lightColor;
}