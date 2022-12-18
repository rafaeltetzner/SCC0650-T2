#include "model.h"
#include "camera.h"

namespace renderer
{
    struct Instance
    {
        Model& model;
        glm::mat4 transform;
        Instance(Model& model) : model(model), transform(glm::mat4(1.0f)) {};
    };

    void render(std::vector<Instance>& instances, Shader& shader, Camera& cam)
    {
        shader.use();
        shader["view"] = cam.get_view();
        shader["projection"] = cam.get_projection();
        for(auto& i : instances)
        {
            shader["model"] = i.transform;
            i.model.draw(shader);
        }
    }
}




