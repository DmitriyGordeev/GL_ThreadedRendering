#include "TextureLoader.h"
#include "IO.h"
#include "Logger.h"


GLuint TextureLoader::loadTexture(const std::string& path) {
    GLuint textureID;

    std::vector<unsigned char> input;
    std::vector<unsigned char> output;
    unsigned long width, height;

    if (!IO::readBinary(path, input))
        Logger::error("[TextureLoader::loadTexture()] Failed to load png!");

    int error = decodePNG(output, width, height, &(input[0]), input.size());
    if (error != 0)
        Logger::error("[TextureLoader::loadTexture()] decodePNG failed width error " + std::to_string(error));

    //  Generate the openGL texture object
    glGenTextures(1, &(textureID));

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D,
                 0, GL_RGBA,
                 width, height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, &(output[0]));

    // Set some texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // TODO: создает ошибку - пофиксить
//    glGenerateMipmap(GL_TEXTURE_2D);

    // finished working with this texture - unbind the context
    glBindTexture(GL_TEXTURE_2D, 0);

//    texture.width = width;
//    texture.height = height;
    return textureID;

}
