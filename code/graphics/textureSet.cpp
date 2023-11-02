#include "textureSet.hpp"
#include "gl.hpp"

TextureSet::TextureSet(Texture* initalTextures[], unsigned int count)
{
    for (int i = 0; i < count; i++)
    {
        textures.push_back(initalTextures[i]);
    }
}

TextureSet::TextureSet(std::initializer_list<Texture*> initalTextures)
{
    textures = initalTextures;
}

void TextureSet::bindTextures()
{
    for (int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]->id);
    }
}
