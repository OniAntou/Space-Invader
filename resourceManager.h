#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <GL/glut.h>
#include <string>
#include <unordered_map>

class Texture {
public:
    GLuint ID;
    int Width, Height, Channels;
    Texture() : ID(0), Width(0), Height(0), Channels(0) {}
    Texture(GLuint id, int w, int h, int c) : ID(id), Width(w), Height(h), Channels(c) {}
};

class ResourceManager {
public:
    static std::unordered_map<std::string, Texture> Textures;

    static Texture LoadTexture(const char* file, bool alpha, std::string name);
    static Texture GetTexture(std::string name);
    static void Clear();

private:
    ResourceManager() {}
    static Texture loadTextureFromFile(const char* file, bool alpha);
};

#endif // RESOURCE_MANAGER_H