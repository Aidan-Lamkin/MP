#version 410 core

// uniform inputs
uniform mat4 mvpMatrix;                 // the precomputed Model-View-Projection Matrix
uniform mat3 normalMatrix;
uniform vec3 lightColor;
uniform vec3 lightDirection;
uniform vec3 pointLightColor;
uniform vec3 pointLightPosition;
uniform vec3 materialColor;             // the material color for our vertex (& whole object)
uniform mat4 modelMtx;


// attribute inputs
layout(location = 0) in vec3 vPos;      // the position of this specific vertex in object space
in vec3 vNormal;

// varying outputs
layout(location = 0) out vec3 color;    // color to apply to this vertex

void main() {
    // transform & output the vertex in clip space
    gl_Position = mvpMatrix * vec4(vPos, 1.0);

    vec3 newLightDirection = normalize(-1 *lightDirection);

    vec3 newNormalVector = vNormal * normalMatrix;

    //vec3 finalColor = lightColor * materialColor * max(dot(newLightDirection, newNormalVector),0);
    // Lighting used goes for a flat, "banded" approach.
    // the dotV value per point is categorized
    // by the below if/else checks and an exaggerated value
    // is returned depending on the result.
    float _diffuseThreshA = 0.95;
    float _diffuseThreshB = 0.5;
    float _diffuseThreshC = 0.25;

    //Point Light
    vec3 pointLight;
    vec4 reletivePosition = modelMtx * vec4(vPos, 1.0);
    float x = reletivePosition[0];
    float y = reletivePosition[1];
    float z = reletivePosition[2];
    vec3 xyz = vec3(x,y,z);
    float pointLightDistance = sqrt(pow(x-pointLightPosition[0],2) + pow(y-pointLightPosition[1],2) + pow(z-pointLightPosition[2],2));
    vec3 pointLightDirection = vec3(normalize(xyz-pointLightPosition));
    vec3 pointDiffuse = lightColor*materialColor*max(dot(pointLightDirection, newNormalVector),0);
    vec3 pointAmbiant = pointLightColor*materialColor*0.1;
    pointLight = pointDiffuse + pointAmbiant;
    float pointAttenuation = 1.0/(1+pointLightDistance+pow(pointLightDistance,2));
    pointLight = pointLight*pointAttenuation;

    //Spot Light

    //Directional Light
    if(max(dot(newLightDirection, newNormalVector),0) >= _diffuseThreshA){
        color = lightColor * materialColor * 1.5;
    } else if(max(dot(newLightDirection, newNormalVector),0) >= _diffuseThreshB && max(dot(newLightDirection, newNormalVector),0) < _diffuseThreshA) {
        color = lightColor * materialColor;
    } else if(max(dot(newLightDirection, newNormalVector),0) >= _diffuseThreshC && max(dot(newLightDirection, newNormalVector),0) < _diffuseThreshB) {
        color = lightColor * materialColor * 0.6;
    }
    else color = lightColor * materialColor * 0.3;
    color += pointLight;
}