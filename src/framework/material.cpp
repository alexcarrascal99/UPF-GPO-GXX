#include "material.h"


Material::Material()
{
    shader = nullptr;
    color_texture = nullptr;
    normal_texture = nullptr;
    ambient = Vector4(0.1f, 0.1f, 0.1f, 1.0f);
    diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    specular = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    shininess = 32.0f;
}

void Material::Enable(const sUniformData& uniform_data)
{
    shader->Enable();
    shader->SetMatrix44("u_model", uniform_data.model);
    shader->SetMatrix44("u_viewprojection", uniform_data.viewprojection);
    shader->SetUniform3("u_camera_position",
        uniform_data.camera_position.x,
        uniform_data.camera_position.y,
        uniform_data.camera_position.z);

    shader->SetUniform3("u_ambient_light",
        uniform_data.ambient_light.x,
        uniform_data.ambient_light.y,
        uniform_data.ambient_light.z);

    shader->SetUniform3("u_light_position",
        uniform_data.light.position.x,
        uniform_data.light.position.y,
        uniform_data.light.position.z);

    shader->SetUniform3("u_light_color",
        uniform_data.light.color.x,
        uniform_data.light.color.y,
        uniform_data.light.color.z);

    shader->SetUniform4("u_material_ambient",
        ambient.x, ambient.y, ambient.z, ambient.w);

    shader->SetUniform4("u_material_diffuse",
        diffuse.x, diffuse.y, diffuse.z, diffuse.w);

    shader->SetUniform4("u_material_specular",
        specular.x, specular.y, specular.z, specular.w);


    shader->SetFloat("u_shininess", shininess);

    if (color_texture)
        shader->SetTexture("u_color_texture", color_texture);

    if (normal_texture)
        shader->SetTexture("u_normal_texture", normal_texture);
}

void Material::Disable()
{
    if (shader)
        shader->Disable();
}