#pragma once
#include "ECS/components/Mesh3D.h"
#include "ECS/components/Renderable.h"
#include <string>

/*
* Struct holding all parameters to generate random height data using hill generation algorithm.
* @param rows: Number of desired heightmap rows
* @param columns: Number of desired heightmap columns
* @param numHills: Number of generated hills
* @param hillRadiusMin: Minimal radius of generated hill (in terms of number of heightmap rows / columns)
* @param hillRadiusMax: Maximal radius of generated hill (in terms of number of heightmap rows / columns)
* @param hillMinHeight: Minimal height of generated hill
* @param hillMaxHeight: Maximal height of generated hill
*/
struct HillAlgorithmParameters
{
    HillAlgorithmParameters(int rows, int columns, int numHills, int hillRadiusMin, int hillRadiusMax, float hillMinHeight, float hillMaxHeight)
    {
        this->rows = rows;
        this->columns = columns;
        this->numHills = numHills;
        this->hillRadiusMin = hillRadiusMin;
        this->hillRadiusMax = hillRadiusMax;
        this->hillMinHeight = hillMinHeight;
        this->hillMaxHeight = hillMaxHeight;
    }

    int rows;
    int columns; 
    int numHills; 
    int hillRadiusMin; 
    int hillRadiusMax; 
    float hillMinHeight; 
    float hillMaxHeight; 
};

/*
* Make a terrain mesh from file or configuration structure
* @param path: source file of the heightTexture
* @return Mesh3D: terain mesh
*/
void makeTerrain(const std::string& path, Mesh3D& mesh, Renderable& renderable);

void makeTerrain(const HillAlgorithmParameters& ha, Mesh3D& mesh, Renderable& renderable);