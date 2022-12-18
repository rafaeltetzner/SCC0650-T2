#include "model.h"
#include "util/logger.h"

#include<glm/vec3.hpp>
#include<string>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<unordered_map>

struct OBJMaterialData
{
    OBJMaterialData()
    {
        ka = glm::vec3(0.0f);
        kd = glm::vec3(0.0f);
        ks = glm::vec3(0.0f);
        ns = 1.0f;
        ni = 1.0f;
        d = 0.0f;
        map_kd = "";
        map_ka = "";
        map_ks = "";
    }
    glm::vec3 ka;
    glm::vec3 kd;
    glm::vec3 ks;
    f32 ns;
    f32 ni;
    f32 d;
    std::string map_kd;
    std::string map_ka;
    std::string map_ks;
};

struct OBJFace
{
    OBJFace(const std::string& material) : material_name(material){};
    std::string material_name;
    std::vector<u32> f_position_idx;
    std::vector<u32> f_texcoord_idx;
    std::vector<u32> f_normal_idx;
};

struct OBJVertexData
{
    std::vector<glm::vec3> v_positions;
    std::vector<glm::vec2> vt_texcoords;
    std::vector<glm::vec3> vn_normals;

    std::vector<OBJFace> faces;

    std::unordered_map<std::string, OBJMaterialData> materials;
};

std::unordered_map<std::string, OBJMaterialData> load_mtl(const std::string& mtl)
{
    std::unordered_map<std::string, OBJMaterialData> materials;

    std::stringstream ss;
	std::ifstream in_file(mtl);

	std::string line = "";
	std::string prefix = "";

    std::string name;
    if(!in_file.is_open())
    {
        logger::log::error("Failed to read \"%s\"", mtl.c_str());
        return materials;
    }
    while(std::getline(in_file, line))
    {
        ss.clear();
        ss.str(line);
        ss >> prefix;

        if(prefix == "newmtl")
        {
            ss >> name;
            materials[name] = OBJMaterialData();
        }
        else if(prefix == "Ka")
            ss >> materials[name].ka.x >> materials[name].ka.y >> materials[name].ka.z;
        else if(prefix == "Kd")
            ss >> materials[name].kd.x >> materials[name].kd.y >> materials[name].kd.z;
        else if(prefix == "Ks")
            ss >> materials[name].ks.x >> materials[name].ks.y >> materials[name].ks.z;
        else if(prefix == "Ni")
            ss >> materials[name].ni;
        else if(prefix == "Ns")
            ss >> materials[name].ns;
        else if(prefix == "d")
            ss >> materials[name].d;
        else if(prefix == "map_Kd")
            ss >> materials[name].map_kd;
        else if(prefix == "map_Ka")
            ss >> materials[name].map_ka;
        else if(prefix == "map_Ks")
            ss >> materials[name].map_ks;
    }
    
    return materials;
}

OBJVertexData* load_obj(const std::string& obj)
{
    OBJVertexData* data = new OBJVertexData();

    std::stringstream ss;
	std::ifstream in_file(obj);

	std::string line = "";
	std::string prefix = "";

    // Read file
    if(!in_file.is_open())
    {
        logger::log::error("Failed to read \"%s\"", obj.c_str());
        return nullptr;
    }
    while(std::getline(in_file, line))
    {
        ss.clear();
        ss.str(line);
        ss >> prefix;
        if(prefix == "v")
        {
            glm::vec3 pos;
            ss >> pos.x >> pos.y >> pos.z;
            data->v_positions.push_back(pos);
        }
        else if(prefix == "vt")
        {
            glm::vec2 tex;
            ss >> tex.x >> tex.y;
            data->vt_texcoords.push_back(tex);
        }
        else if(prefix == "vn")
        {
            glm::vec3 nor;
            ss >> nor.x >> nor.y >> nor.z;
            data->vn_normals.push_back(nor);
        }
        else if(prefix == "f")
        {
            int idx_in_face = 0;
            u32 idx;
            if(data->faces.size() == 0)
                data->faces.push_back(OBJFace(""));
            OBJFace& current_face = data->faces[data->faces.size() - 1];
            while(ss >> idx)
            {
                if(idx_in_face == 0)
                    current_face.f_position_idx.push_back(idx);
                else if(idx_in_face == 1)
                    current_face.f_texcoord_idx.push_back(idx);
                else if(idx_in_face == 2)
                    current_face.f_normal_idx.push_back(idx);
                
                if(ss.peek() == '/')
                {
                    idx_in_face++;
                    ss.ignore(1, '/');
                }
                else if(ss.peek() == ' ')
                {
                    idx_in_face++;
                    ss.ignore(1, ' ');
                }

                if(idx_in_face > 2)
                    idx_in_face = 0;
            }
        }
        else if(prefix == "usemtl")
        {
            std::string material_name;
            ss >> material_name;
            data->faces.push_back(OBJFace(material_name));
        }
        else if(prefix == "mtllib")
        {
            std::string mtl;
            ss >> mtl;
            std::string dir = obj.substr(0, obj.find_last_of('/'));
            data->materials = load_mtl(dir + '/' + mtl);
        }            
    }

    return data;
}

Model::Model(const std::string& obj)
{
    OBJVertexData* data = load_obj(obj);

    for(auto& mesh_data : data->faces)
    {
        std::vector<Vertex> vertices;
        vertices.resize(mesh_data.f_position_idx.size(), Vertex());
        for(u32 i = 0; i < vertices.size(); i++)
        {
            vertices[i].position = data->v_positions[mesh_data.f_position_idx[i] - 1];
            vertices[i].normal = data->vn_normals[mesh_data.f_normal_idx[i] - 1];
            vertices[i].texture_coords = data->vt_texcoords[mesh_data.f_texcoord_idx[i] - 1];
        }

        OBJMaterialData material_data = data->materials[mesh_data.material_name];
        Mesh::Material material;
        material.ambient = material_data.ka;
        material.diffuse = material_data.kd;
        material.specular = material_data.ks;
        std::string dir = obj.substr(0, obj.find_last_of('/'));
        if(material_data.map_kd != "")
            material.textures.push_back(Texture(dir + "/" + material_data.map_kd, Texture::Type::DIFFUSE));
        if(material_data.map_ks != "")
            material.textures.push_back(Texture(dir + "/" + material_data.map_ks, Texture::Type::SPECULAR));

        _meshes.push_back(Mesh(vertices, material));
    }

    delete data;
}

void Model::draw(Shader& shader)
{
    for(u32 i = 0; i < _meshes.size(); i++)
        _meshes[i].draw(shader);
}
