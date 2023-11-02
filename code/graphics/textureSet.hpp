#pragma once
#include "texture.hpp"
#include <vector>

class TextureSet
{
private:
    std::vector<Texture*> textures;
    
public:
    TextureSet(Texture* initalTextures[], unsigned int count);
    TextureSet(std::initializer_list<Texture*> initalTextures);
    void bindTextures();
};
