#version 330

in vec4 vColor;
in vec2 TexCoord;
in vec4 DirLightSpacePos;
in vec3 Normal;
in vec3 FragPos;

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
uniform sampler2D dirShadowMap;

uniform Material material;

uniform vec3 eyePosition;

float CalcDirShadowFactor(DirectionalLight light) {

    vec3 projCoords = DirLightSpacePos.xyz / DirLightSpacePos.w;
    projCoords = (projCoords * 0.5f) + 0.5f;

    float current = projCoords.z;
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(light.direction);

    float bias = max(0.05f * (1.0f - dot(normal, lightDirection)), 0.005f);
    float shadow = 0.0f;

    vec2 texelSize = 1.0f / textureSize(dirShadowMap, 0);
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y < 1; ++y) {
            float pcfDepth = texture(dirShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += current - bias > pcfDepth ? 1.0f : 0.0f;
        }
    }
    
    shadow /= 9;

    if (projCoords.z > 1.0f) {
        shadow = 0.0f;
    }

    return shadow;
};

vec4 CalcLightByDirection(Light light, vec3 direction, float shadowFactor) {

    vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;

    float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);
    vec4 diffuseColor = vec4(light.color, 1.0f) * light.diffuseIntensity * diffuseFactor;

    vec4 specularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    if (diffuseFactor > 0.0f) {

        vec3 fragToEye = normalize(eyePosition - FragPos);
	    vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal)));

        float specularFactor = dot(fragToEye, reflectedVertex);

	    if (specularFactor > 0.0f) {

	        specularFactor = pow(specularFactor, material.shininess);
	        specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
	    }
    }

    return (ambientColor + (1.0f - shadowFactor) * (diffuseColor + specularColor));
}

vec4 CalcDirectionalLight() {
    float shadowFactor = CalcDirShadowFactor(directionalLight);
    return CalcLightByDirection(directionalLight.base, directionalLight.direction, shadowFactor);
}

vec4 CalcPointLight(PointLight light) {

    vec3 direction = FragPos - light.position;
	float distance = length(direction);
	direction = normalize(direction);

	vec4 color = CalcLightByDirection(light.base, direction, 0.0f);
	float attenuation = light.exponent * distance * distance +
		light.linear * distance +
		light.constant;

	return (color / attenuation);
}

vec4 CalcSpotLight(SpotLight light) {

    vec4 color = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    vec3 rayDirection = normalize(FragPos - light.base.position);
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

    color = texture(texSamp, TexCoord) * lightColor;
}