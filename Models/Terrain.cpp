#include "Terrain.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>
#include "../stb_image.h"

template<typename Iter>
struct SeedSeq
{
    Iter _begin;
    Iter _end;

    SeedSeq(Iter begin, Iter end) : _begin(begin), _end(end) {}

    template<typename Iter2>
    void generate(Iter2 b, Iter2 e) {
        std::copy(_begin, _begin + (e - b), b);
    }
};

std::vector<std::vector<float>> generateHeightMap(const HillAlgorithmParameters& ha)
{
    std::vector<std::vector<float>> result(ha.rows, std::vector<float>(ha.columns, 0.0f));

    std::random_device rd;
    uint32_t numbers[624];
    std::generate(numbers, std::end(numbers), std::ref(rd));
    SeedSeq sseq(numbers, std::end(numbers));
    std::mt19937 generator(sseq);

    std::uniform_int_distribution<int> hillRadiusDist(ha.hillRadiusMin, ha.hillRadiusMax);
    std::uniform_int_distribution<int> hillCenterRowDist(0, ha.rows - 1);
    std::uniform_int_distribution<int> hillCenterColDist(0, ha.columns - 1);
    std::uniform_real_distribution<float> hillHeightDist(ha.hillMinHeight, ha.hillMaxHeight);

    for (int i = 0; i < ha.numHills; i++)
    {
        const auto hillCenterRow = hillCenterRowDist(generator);
        const auto hillCenterCol = hillCenterColDist(generator);
        const auto hillRadius = hillRadiusDist(generator);
        const auto hillHeight = hillHeightDist(generator);
        const auto r2 = hillRadius * hillRadius;

        for (auto row = hillCenterRow - hillRadius; row < hillCenterRow + hillRadius; row++)
        {
            for (auto col = hillCenterCol - hillRadius; col < hillCenterCol + hillRadius; col++)
            {
                if (row < 0 || row >= ha.rows || col < 0 || col >= ha.columns) {
                    continue;
                }

                const auto x2x1 = hillCenterCol - col;
                const auto y2y1 = hillCenterRow - row;
                const float height = static_cast<float>(r2 - x2x1 * x2x1 - y2y1 * y2y1);

                if (height < 0.0f) {
                    continue;
                }

                const float factor = height / static_cast<float>(r2);
                result[row][col] += hillRadius * factor;
                result[row][col] = result[row][col] > 1.0f ? 1.0f : result[row][col];
            }
        }
    }

    return result;
}

std::vector<std::vector<float>> getHeightDataFromImage(const std::string& fileName)
{
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChanals;
    const auto imageData = stbi_load(fileName.c_str(), &width, &height, &nrChanals, 0);
    
    if (!imageData)
    {
        std::cerr << "Failed to load heightmap image " << fileName << "!" << std::endl;
        return {};
    }

    std::vector<std::vector<float>> result(height, std::vector<float>(width));
    auto pixelPtr = &imageData[0];
    for (auto i = 0; i < height; i++)
    {
        for (auto j = 0; j < width; j++)
        {
            result[i][j] = static_cast<float>(*pixelPtr) / 255.0f;
            pixelPtr += nrChanals;
        }
    }

    stbi_image_free(imageData);
    return result;
}

void setVertices(const std::vector<std::vector<float>>& heightData, std::vector<Vertex>& vertices, int rows, int columns)
{
    size_t indx = 0;
    const float irows = 1.0f / static_cast<float>(rows - 1);
    const float icols = 1.0f / static_cast<float>(columns - 1);

    for (int i = 0; i < rows; i++)
    {
        const float rowFactor = static_cast<float>(i) * irows;
        for (int j = 0; j < columns; j++)
        {
            const float colFactor = static_cast<float>(j) * icols;
            vertices[indx++].Position = glm::vec3(-0.5f + colFactor, heightData[i][j], -0.5f + rowFactor);
        }
    }
}

void setNormals(const std::vector<std::vector<float>>& heightData, std::vector<Vertex>& vertices, int rows, int columns)
{
    std::vector<std::vector<glm::vec3>> tempNorms[2];

    for (uint8_t i = 0; i < 2; i++) {
        tempNorms[i] = std::vector<std::vector<glm::vec3>>(rows - 1, std::vector<glm::vec3>(columns - 1));
    }

    for (int i = 0; i < rows - 1; i++)
    {
        for (int j = 0; j < columns - 1; j++)
        {
            const auto& A = vertices[i * columns + j].Position;
            const auto& B = vertices[i * columns + j + 1].Position;
            const auto& C = vertices[(i + 1) * columns + j + 1].Position;
            const auto& D = vertices[(i + 1) * columns + j].Position;

            const auto triagNormA = glm::cross(B - A, A - D);
            const auto triagNormB = glm::cross(D - C, C - B);

            tempNorms[0][i][j] = glm::normalize(triagNormA);
            tempNorms[1][i][j] = glm::normalize(triagNormB);
        }
    }

    size_t indx = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            bool isFirstRow = i == 0;
            bool isFirstColumn = j == 0;
            bool isLastRow = i == rows - 1;
            bool isLastColumn = j == columns - 1;

            glm::vec3 finalVertexNormal = glm::vec3(0.0f, 0.0f, 0.0f);

            // Look for triangle to the upper-left
            if (!isFirstRow && !isFirstColumn) {
                finalVertexNormal += tempNorms[0][i - 1][j - 1];
            }

            // Look for triangles to the upper-right
            if (!isFirstRow && !isLastColumn) {
                for (uint8_t k = 0; k < 2; k++) {
                    finalVertexNormal += tempNorms[k][i - 1][j];
                }
            }

            // Look for triangle to the bottom-right
            if (!isLastRow && !isLastColumn) {
                finalVertexNormal += tempNorms[0][i][j];
            }

            // Look for triangles to the bottom-right
            if (!isLastRow && !isFirstColumn) {
                for (uint8_t k = 0; k < 2; k++) {
                    finalVertexNormal += tempNorms[k][i][j - 1];
                }
            }

            vertices[indx++].Normal = glm::normalize(finalVertexNormal);
        }
    }
}

void setTextures(const std::vector<std::vector<float>>& heightData, std::vector<Vertex>& vertices, int rows, int columns)
{
    const float stepU = 0.1f;
    const float stepV = 0.1f;
    size_t indx = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            vertices[indx++].TexCoords = glm::vec2(j * stepU, i * stepV);
        }
    }
}

int setIndexes(VertexBuffer& EBO, int rows, int columns)
{
    EBO.makeVBO(((rows - 1) * columns * 2 + rows - 1) * sizeof(GLuint));
    EBO.bindVBO(GL_ELEMENT_ARRAY_BUFFER);
    const int restartIndex = rows * columns;

    for (int i = 0; i < rows - 1; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                const int row = i + k;
                const int index = row * columns + j;
                EBO.addRawData(&index, sizeof(int));
            }
        }

        EBO.addRawData(&restartIndex, sizeof(int));
    }

    EBO.uploadDataToGPU(GL_STATIC_DRAW, true);

    return restartIndex;
}

void TerraniGenerator(const std::vector<std::vector<float>>& heightData, Mesh3D& mesh, Renderable& renderable)
{
    const int rows = static_cast<int>(heightData.size()); // Number of heightmap rows
    const int columns = static_cast<int>(heightData[0].size()); // Number of heightmap columns
    const int numVertices = rows * columns;

    std::vector<Vertex> vertices(numVertices);
    setVertices(heightData, vertices, rows, columns);
    setNormals(heightData, vertices, rows, columns);
    setTextures(heightData, vertices, rows, columns);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    VertexBuffer VBO;
    VBO.makeVBO(numVertices * sizeof(Vertex));
    VBO.bindVBO(GL_ARRAY_BUFFER);
    VBO.addRawData(vertices.data(), numVertices * sizeof(Vertex));
    VBO.uploadDataToGPU(GL_STATIC_DRAW, true);

    glEnableVertexAttribArray(Mesh3D::position_attrib);
    glVertexAttribPointer(Mesh3D::position_attrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glEnableVertexAttribArray(Mesh3D::normal_attrib);
    glVertexAttribPointer(Mesh3D::normal_attrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
    glEnableVertexAttribArray(Mesh3D::texture_attrib);
    glVertexAttribPointer(Mesh3D::texture_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, TexCoords)));

    VertexBuffer EBO;
    int primitiveRestartIndx = setIndexes(EBO, rows, columns);

    glBindVertexArray(0);

    auto& component = renderable.rdata.emplace_back();
    component.VAO = VAO;
    component.drawModeType = GL_TRIANGLE_STRIP;
    component.drawArray = GL_PRIMITIVE_RESTART;
    component.sizeOfDraw = (rows - 1) * 2 * columns + rows - 1;
    renderable.primitiveRestartIndx = primitiveRestartIndx;
    mesh.VBO = std::move(VBO);
    mesh.EBO = std::move(EBO);
    mesh.is_normals = mesh.is_texture = true;
}

void makeTerrain(const std::string& path, Mesh3D& mesh, Renderable& renderable)
{
    std::vector<std::vector<float>> heightData = getHeightDataFromImage(path);
    TerraniGenerator(heightData, mesh, renderable);
}

void makeTerrain(const HillAlgorithmParameters& ha, Mesh3D& mesh, Renderable& renderable)
{
    std::vector<std::vector<float>> heightData = generateHeightMap(ha);
    TerraniGenerator(heightData, mesh, renderable);
}