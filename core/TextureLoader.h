#ifndef COLLIDERGAME_TEXTURELOADER_H
#define COLLIDERGAME_TEXTURELOADER_H

#include "picoPNG.h"
#include <string>
#include <vector>
#include <gl/glew.h>

class TextureLoader {
public:
    /* loads texture from file and binds its bytes to
     * generated GL texture ID */
    static GLuint loadTexture(const std::string& path,
                              std::vector<unsigned char>& output);

};


#endif //COLLIDERGAME_TEXTURELOADER_H
