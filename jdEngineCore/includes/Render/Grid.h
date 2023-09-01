#pragma once
#include <glad/glad.h>

class Grid final
{
public:
    Grid();
    void Draw();
    ~Grid() noexcept;
private:
    GLuint VAO;
};