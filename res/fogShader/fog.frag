#version 440 core

#include_part

struct Fog
{
    vec3 color;
    float density;
};

float getFogFactorExp2(Fog fogStruct, float fogCoordinates);

#definition_part

float getFogFactorExp2(Fog fogStruct, float fogCoordinates)
{
    float result = exp(-(fogStruct.density * fogStruct.density * fogCoordinates * fogCoordinates));
    result = 1.0 - clamp(result, 0.0, 1.0);
    return result;
}