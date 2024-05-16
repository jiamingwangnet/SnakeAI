#include "ImagerLoader.h"
#include <fstream>
#include <vector>
#include "FileManager.h"
#include <stdexcept>
#include <GL/glew.h>

GEngine::GLTexture GEngine::ImagerLoader::LoadPNG(std::string path)
{
    GLTexture texture = {};

    std::vector<unsigned char> pngData;
    std::vector<unsigned char> decodedData;
    unsigned long width = 0, height = 0;

    if (!FileManager::ReadToBuffer(path, pngData)) // load the file
    {
        throw std::exception{((std::string)"Could not load PNG " + path + " to buffer.").c_str()};
    }
    int err = decodePNG(decodedData, width, height, pngData.data(), pngData.size());
    if (err != 0) // decode into RGBA
    {
        //perror(((std::string)"decodePNG failed on " + path + " with error code " + std::to_string(err)).c_str());
        throw std::exception{ ((std::string)"decodePNG failed on " + path + " with error code " + std::to_string(err)).c_str()};
    }

    texture.width = (int)width;
    texture.height = (int)height;

    glGenTextures(1, &texture.id); // create a texture
    glBindTexture(GL_TEXTURE_2D, texture.id); // bind the created texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, decodedData.data()); // specify the texture's data

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // texture wrap idk
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // use (b1)linear sampling when the texture is magnified
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0); // unbind

    return texture;
}
