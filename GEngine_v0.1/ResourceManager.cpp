#include "ResourceManager.h"

GEngine::TextureCache GEngine::ResourceManager::textureCache;
GEngine::MaterialCache GEngine::ResourceManager::materialCache;

GEngine::GLTexture GEngine::ResourceManager::GetTexture(const std::string& texturePath)
{
    return textureCache.GetTexture(texturePath);
}

GEngine::Material GEngine::ResourceManager::GetMaterial(const std::string& texturePath, const std::string& vertShaderPath, const std::string& fragShaderPath)
{
    return materialCache.GetMaterial(texturePath, vertShaderPath, fragShaderPath);
}
