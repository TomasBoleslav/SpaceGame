#include "Texture2D.hpp"

#include "Errors.hpp"

#include <glad/glad.h>

Texture2D::Texture2D(unsigned int width, unsigned int height, unsigned char* data)
    : m_width(width), m_height(height), m_settings(Settings())
{
    m_textureID = generateTexture(width, height, data, m_settings);
}

Texture2D::Texture2D(unsigned int width, unsigned int height, unsigned char* data, const Settings& settings)
    : m_width(width), m_height(height), m_settings(settings)
{
    m_textureID = generateTexture(width, height, data, settings);
}

void Texture2D::bind() const
{
    GL_CALL(glActiveTexture(GL_TEXTURE0));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, m_textureID));
}

void Texture2D::unbind() const
{
    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}

unsigned int Texture2D::getID() const
{
    return m_textureID;
}

const Texture2D::Settings& Texture2D::getSettings() const
{
    return m_settings;
}

glm::vec2 Texture2D::getSize()
{
    return glm::vec2(static_cast<float>(m_width), static_cast<float>(m_height));
}

unsigned int Texture2D::generateTexture(unsigned int width, unsigned int height, unsigned char* data, const Settings& settings) const
{
    unsigned int textureID;
    GL_CALL(glGenTextures(1, &textureID));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, textureID));
    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, settings.internalFormat, width, height, 0, settings.format, GL_UNSIGNED_BYTE, data));

    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, settings.wrapS));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, settings.wrapT));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, settings.filterMin));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, settings.filterMag));

    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
    return textureID;
}

Texture2D::Settings::Settings()
    : internalFormat(GL_RGB), format(GL_RGB), wrapS(GL_CLAMP_TO_EDGE), wrapT(GL_CLAMP_TO_EDGE), filterMin(GL_LINEAR), filterMag(GL_LINEAR)
{
}
