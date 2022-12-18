#include "model.h"
#include "camera.h"
#include "light.h"

namespace renderer
{
    struct Instance
    {
        Model& model;
        glm::mat4 transform;
        Instance(Model& model) : model(model), transform(glm::mat4(1.0f)) {};
    };

    void render(std::vector<Instance>& instances, Shader& shader, Camera& cam, Light& light)
    {
        shader.use();
        shader["view"] = cam.get_view();
        shader["projection"] = cam.get_projection();
        shader["camera_pos"] = cam.get_pos();

        shader["light_position"] = light.position;
        shader["light_ambient"] = light.ambient;
        shader["light_diffuse"] = light.diffuse;
        shader["light_specular"] = light.specular;
        shader["light_color"] = light.color;
        for(auto& i : instances)
        {
            shader["model"] = i.transform;
            i.model.draw(shader);
        }
    }
}




