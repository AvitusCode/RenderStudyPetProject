# RenderStudyPetProject
!Проект переехал под новую архитектуру (Entity-Component-System)!

Данный "движок" разрабатывается с целью изучения паттеронов разработки, изучения OpenGl, алгоритмов компьютерной графики.
В качестве вдохновения использовался ресурс learnopengl.com

## В проекте имеется:
* Возможность добавления новых сцен (пример находится в scenes/Demo1.cpp)
* Terrain
* Fog system
* Light System (Phong)
* Надстройка над GLSL (позволяет ипользовать возможность добавления функций в другие файлы через заголовки дерективой #include "... .frag"
* Возможность загружать модели через Assimp
* Выстроен графический пайплайн

## Планируется добавить
* Полноценное GUI с использованием ImGui
* Frustum culling
* Quad tree для сцен
* Shadow mapping
* Parallax mapping
* и т.п.
