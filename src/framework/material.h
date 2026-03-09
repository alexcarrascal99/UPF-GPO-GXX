#include "framework.h"
#include "image.h"
#include "shader.h"
#include "utils.h"




struct sLight
{
    Vector3 position;
    Vector3 color;
};

struct sUniformData
{
    Matrix44 model;
    Matrix44 viewprojection;

    Vector3 camera_position;
    Vector3 ambient_light;

    sLight light;
};

class Material
{
public:
    Shader* shader;
    Texture* color_texture;
    Texture* normal_texture;

    Vector4 ambient;
    Vector4 diffuse;
    Vector4 specular;
    float shininess;

    Material();

    void Enable(const sUniformData& uniform_data);
    void Disable();
};