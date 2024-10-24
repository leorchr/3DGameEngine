#pragma once
#include <map>
#include <string>
#include "Mesh.h"
#include <vector>
using std::map;
using std::string;

class IRenderer;
class Texture;
class Shader;
class Font;

class Assets
{
public:
    static std::map<string, Texture> textures;
    static std::map<string, Shader> shaders;
    static std::map<string, Mesh> meshes;
    static std::map<string, Font> fonts;
    static std::map<string, string> texts;
    static std::map<string, std::vector<std::vector<int>>> maps;

    // Loads a texture from file
    static Texture loadTexture(IRenderer& renderer, const string& filename, const string& name);

    // Retrieves a stored texture
    static Texture& getTexture(const std::string& name);

    // Loads (and generates) a shader program from file loading vertex, fragment (and tessellation control, evaluation,
    // geometry) shader's source code. If tcShaderFile, teShaderFile, gShaderFile are not nullptr, it also loads
    // tessellation and geometry shaders
    static Shader loadShader(const std::string& vShaderFile, const std::string& fShaderFile,
        const std::string& tcShaderFile, const std::string& teShaderFile,
        const std::string& gShaderFile, const std::string& name);

    // Retrieves a stored shader
    static Shader& getShader(const std::string& name);

    // Loads a mesh from file
    static Mesh loadMesh(const string& filename, const string& name);

    // Retrieves a stored mesh
    static Mesh& getMesh(const std::string& name);

    // Loads a font from file
    static Font loadFont(const string& filename, const string& name);

    // Retrieves a stored font
    static Font& getFont(const std::string& name);

    // Loads texts from a localization file
    static void loadText(const string& filename);

    // Retrieves a localized text
    static const string& getText(const string& key);

    // Loads a font from file
    static std::vector<std::vector<int>> loadMap(const string& filename, const string& name);

    // Retrieves a stored font
    static std::vector<std::vector<int>> getMap(const std::string& name);

    // Properly de-allocates all loaded resources
    static void clear();

private:
    Assets() {}

    // Loads a single texture from file
    static Texture loadTextureFromFile(IRenderer& renderer, const string& filename);

    // Loads and generates a shader from file
    static Shader loadShaderFromFile(const std::string& vShaderFile, const std::string& fShaderFile,
        const std::string& tcShaderFile = "", const std::string& teShaderFile = "",
        const std::string& gShaderFile = "");

    static Mesh loadMeshFromFile(const string& filename);

    static Font loadFontFromFile(const string& filename);

    static std::vector<std::vector<int>> loadMapFromFile(const string& filename);
};