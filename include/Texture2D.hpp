#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP

class Texture2D final
{
public:
    struct Settings final
    {
    public:
        Settings();

        unsigned int internalFormat;
        unsigned int format;
        unsigned int wrapS;
        unsigned int wrapT;
        unsigned int filterMin;
        unsigned int filterMag;
    };

    Texture2D() noexcept;
    Texture2D(const Texture2D&) = delete;
    Texture2D(Texture2D&& other) noexcept;
    Texture2D& operator=(const Texture2D&) = delete;
    Texture2D& operator=(Texture2D&& other) noexcept;
    ~Texture2D();

    void create(unsigned int width, unsigned int height, unsigned char* data);
    void create(unsigned int width, unsigned int height, unsigned char* data, const Settings& settings);

    void bind() const;
    void unbind() const;

    inline const Settings& getSettings() const { return m_settings; }

private:
    unsigned int m_textureID;
    unsigned int m_width, m_height;
    Settings m_settings;

    unsigned int generateTexture(unsigned int width, unsigned int height, unsigned char* data, const Settings& settings) const;
    void destroy() const;
};

#endif