#include "model.h"
#include "util/logger.h"

#include <GL/glew.h>

void Model::load(const std::string& filepath, const std::string& path_di)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        logger::log::error("Assimp: %s", importer.GetErrorString());
        return;
    }

    _directory = filepath.substr(0, filepath.find_last_of('/'));

    process_node(scene->mRootNode, scene);
}

void Model::draw(Shader& shader) const
{
    for(u32 i = 0; i < _meshes.size(); i++)
        _meshes[i].draw(shader);
}

void Model::process_node(aiNode *node, const aiScene *scene)
{
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        _meshes.push_back(process_mesh(mesh, scene));			
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        process_node(node->mChildren[i], scene);
    }
}

Mesh Model::process_mesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<u32> indices;
    std::vector<Texture> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;
        
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }
        
        if(mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texture_coords = vec;
            logger::log::debug("(%f, %f)", vec.x, vec.y);
        }
        else
            vertex.texture_coords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);        
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    

    std::vector<Texture> diffuseMaps = load_material_textures(material, aiTextureType_DIFFUSE);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<Texture> specularMaps = load_material_textures(material, aiTextureType_SPECULAR);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture> normalMaps = load_material_textures(material, aiTextureType_HEIGHT);
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture> heightMaps = load_material_textures(material, aiTextureType_AMBIENT);
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // logger::log::trace("%d", textures.size());

    return Mesh(vertices, textures, indices);
}

std::vector<Texture> Model::load_material_textures(aiMaterial* mat, aiTextureType type)
{
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str_tmp;
        mat->GetTexture(type, i, &str_tmp);
        std::string str = std::string(str_tmp.C_Str());
        
        bool skip = false;
        if(_tex_loaded.find(str) != _tex_loaded.end())
        {
            textures.push_back(_tex_loaded[str]);
            skip = true;
        }
        
        if(!skip)
        {
            Texture texture;
            std::string path =  _directory + '/' + str;
            switch(type)
            {
                case aiTextureType_DIFFUSE: 
                    texture = Texture(Texture::Type::DIFFUSE, path);
                    break;
                case aiTextureType_SPECULAR:
                    texture = Texture(Texture::Type::SPECULAR, path);
                    break;
                case aiTextureType_HEIGHT:
                    texture = Texture(Texture::Type::NORMAL, path);
                    break;
                case aiTextureType_AMBIENT:
                    texture = Texture(Texture::Type::HEIGHT, path);
                    break;
                default:
                    break;
            }

            textures.push_back(texture);
            _tex_loaded[str] = texture;
        }
    }
    return textures;
}