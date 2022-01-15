#include "Texture.h"

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
	type = texType;

    // Stores the width, height, and the number of color channels of the image
    int widthImg, heightImg, numColCh;
    // STBI by default reads images from the TL corner to BR
    // OpenGL likes to read images from BL -> TR
    // By flipping the image vertically, OpenGL will read the image
    // in the correct way
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

    // Generates an OpenGL texture
    glGenTextures(1, &ID);
    // Assigns the texture to a unit
    glActiveTexture(slot);
    glBindTexture(type, ID);

    // Set the algo used when resizing
    // minimise. 'nearest neighbor'
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // magnify
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Configures the way the texture repeats
    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Uncomment these lines to use GL_CLAMP_TO_BORDER
    // float flatColour[] = {1.0f, 1.0f, 1.0f, 1.0f};
    // glTextParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

    // Assigns the read image to the OpenGL Texture object
    glTexImage2D(type, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
    // Creates MipMaps
    glGenerateMipmap(type);

    // Clear out image
    stbi_image_free(bytes);
    // Unbind for safety
    glBindTexture(type, 0);
}

void Texture::textUnit(Shader& shader, const char* uniform, GLuint unit)
{
    GLuint tex0Uni = glGetUniformLocation(shader.ID, uniform);
    shader.Activate();
    glUniform1i(tex0Uni, 0);
}
void Texture::Bind()
{
    glBindTexture(type, ID);
}
void Texture::Unbind()
{
    glBindTexture(type, 0);
}
void Texture::Delete()
{
    glDeleteTextures(1, &ID);
}