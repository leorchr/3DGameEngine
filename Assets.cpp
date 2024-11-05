#include "Assets.h"
#include "ComputeShader.h"
#include "Font.h"
#include "Log.h"
#include "RendererOGL.h"
#include "Texture.h"
#include <document.h>
#include <fstream>
#include <istreamwrapper.h>
#include <sstream>

using namespace std;

std::map<string, Texture> Assets::textures;
std::map<string, Shader> Assets::shaders;
std::map<string, Mesh> Assets::meshes;
std::map<string, Font> Assets::fonts;
std::map<string, string> Assets::texts;
std::map<string, std::vector<std::vector<int>>> Assets::maps;
std::map<std::string, ComputeShader> Assets::computeShaders;

Texture Assets::loadTexture(IRenderer& renderer, const string& filename, const string& name)
{
    textures[name] = loadTextureFromFile(renderer, filename.c_str());
    return textures[name];
}

Texture& Assets::getTexture(const string& name)
{
    if (textures.find(name) == end(textures))
    {
        std::ostringstream loadError;
        loadError << "Texture " << name << " does not exist in assets manager.";
        Log::error(LogCategory::Application, loadError.str());
    }
    return textures[name];
}

Shader Assets::loadShader(const std::string& vShaderFile, const std::string& fShaderFile, const std::string& tcShaderFile, const std::string& teShaderFile, const std::string& gShaderFile, const std::string& name)
{
    shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, tcShaderFile, teShaderFile, gShaderFile);
    return shaders[name];
}

Shader& Assets::getShader(const std::string& name)
{
    if (shaders.find(name) == end(shaders))
    {
        std::ostringstream loadError;
        loadError << "Shader " << name << " does not exist in assets manager.";
        Log::error(LogCategory::Application, loadError.str());
    }
    return shaders[name];
}

Mesh Assets::loadMesh(const string& filename, const string& name)
{
    meshes[name] = loadMeshFromFile(filename);
    return meshes[name];
}

Mesh& Assets::getMesh(const std::string& name)
{
    if (meshes.find(name) == end(meshes))
    {
        std::ostringstream loadError;
        loadError << "Mesh " << name << " does not exist in assets manager.";
        Log::error(LogCategory::Application, loadError.str());
    }
    return meshes[name];
}

Font Assets::loadFont(const string& filename, const string& name)
{
    fonts[name] = loadFontFromFile(filename);
    return fonts[name];
}

Font& Assets::getFont(const std::string& name)
{
    if (fonts.find(name) == end(fonts))
    {
        std::ostringstream loadError;
        loadError << "Font " << name << " does not exist in assets manager.";
        Log::error(LogCategory::Application, loadError.str());
    }
    return fonts[name];
}

void Assets::loadText(const string& filename)
{
    texts.clear();
    // Try to open the file
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::ostringstream loadError;
        loadError << "Text file " << filename << " not found.";
        Log::error(LogCategory::Application, loadError.str());
        return;
    }
    // Read the entire file to a string stream
    std::stringstream fileStream;
    fileStream << file.rdbuf();
    std::string contents = fileStream.str();
    // Open this file in rapidJSON
    rapidjson::StringStream jsonStr(contents.c_str());
    rapidjson::Document doc;
    doc.ParseStream(jsonStr);
    if (!doc.IsObject())
    {
        std::ostringstream loadError;
        loadError << "Text file " << filename << " is not valid JSON.";
        Log::error(LogCategory::Application, loadError.str());
        return;
    }
    // Parse the text map
    const rapidjson::Value& actions = doc["TextMap"];
    for (rapidjson::Value::ConstMemberIterator itr = actions.MemberBegin(); itr != actions.MemberEnd(); ++itr)
    {
        if (itr->name.IsString() && itr->value.IsString())
        {
            texts.emplace(itr->name.GetString(), itr->value.GetString());
        }
    }
    Log::info("Loaded localization file: " + filename);

}

const string& Assets::getText(const string& key)
{
    static string errorMsg("**KEY NOT FOUND**");
    auto iter = texts.find(key);
    if (iter != end(texts))
    {
        return iter->second;
    }
    else
    {
        return errorMsg;
    }
}

// Loads a font from file
std::vector<std::vector<int>> Assets::loadMap(const string& filename, const string& name) {
    maps[name] = loadMapFromFile(filename);
    return maps[name];
}

// Retrieves a stored font
std::vector<std::vector<int>> Assets::getMap(const std::string& name) {
    if (maps.find(name) == end(maps))
    {
        std::ostringstream loadError;
        loadError << "Map " << name << " does not exist in assets manager.";
        Log::error(LogCategory::Application, loadError.str());
    }
    return maps[name];
}

ComputeShader Assets::loadComputeShader(const std::string& cShaderFile, const std::string& name)
{
    computeShaders[name] = loadComputeShaderFromFile(cShaderFile);
    return computeShaders[name];
}

ComputeShader& Assets::getComputeShader(const std::string& name)
{
    if (computeShaders.find(name) == end(computeShaders))
    {
        std::ostringstream loadError;
        loadError << "Shader " << name << " does not exist in assets manager.";
        Log::error(LogCategory::Application, loadError.str());
    }
    return computeShaders[name];
}

void Assets::clear()
{
    // Delete all textures
    for (auto iter : textures)
        iter.second.unload();
    textures.clear();
    // Delete all shaders
    for (auto iter : shaders)
        iter.second.unload();
    shaders.clear();
    // Delete all fonts
    for (auto iter : fonts)
        iter.second.unload();
    fonts.clear();
    // Delete texts
    texts.clear();
    // Delete all maps
    maps.clear();
}

Texture Assets::loadTextureFromFile(IRenderer& renderer, const string& filename)
{
    Texture texture;
    // Not very elegant, but simpler architecture
    if (renderer.type() == IRenderer::Type::SDL)
    {
        //texture.loadSDL(dynamic_cast<RendererSDL&>(renderer), filename);
    }
    else if (renderer.type() == IRenderer::Type::OGL)
    {
        texture.loadOGL(dynamic_cast<RendererOGL&>(renderer), filename);
    }
    return texture;
}

Shader Assets::loadShaderFromFile(const std::string& vShaderFile, const std::string& fShaderFile, const std::string& tcShaderFile, const std::string& teShaderFile, const std::string& gShaderFile)
{
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string tcCode;
    std::string teCode;
    std::string geometryCode;
    try {
        // Open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // Read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // If tess control shader path is present, also load a tess control shader
        if (tcShaderFile != "") {
            std::ifstream tessControlShaderFile(tcShaderFile);
            std::stringstream tcShaderStream;
            tcShaderStream << tessControlShaderFile.rdbuf();
            tessControlShaderFile.close();
            tcCode = tcShaderStream.str();
        }
        // If tess evaluation shader path is present, also load a tess evaluation shader
        if (teShaderFile != "") {
            std::ifstream tessEvalShaderFile(teShaderFile);
            std::stringstream teShaderStream;
            teShaderStream << tessEvalShaderFile.rdbuf();
            tessEvalShaderFile.close();
            teCode = teShaderStream.str();
        }
        // If geometry shader path is present, also load a geometry shader
        if (gShaderFile != "") {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e) {
        std::ostringstream loadError;
        std::string geomShaderFile = "";
        if (gShaderFile != "")
            geomShaderFile = gShaderFile;

        loadError << "ERROR::SHADER: Failed to read shader files " << vShaderFile << " " << fShaderFile << " "
            << geomShaderFile << "\n"
            << "\n -- --------------------------------------------------- -- "
            << std::endl;
        Log::error(LogCategory::Render, loadError.str());
    }
    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();
    const GLchar* tcShaderCode = tcCode.c_str();
    const GLchar* teShaderCode = teCode.c_str();
    const GLchar* gShaderCode = geometryCode.c_str();
    // 2. Now create shader object from source code
    Shader shader;
    shader.compile(vShaderCode, fShaderCode,
        tcShaderFile != "" ? tcShaderCode : nullptr,
        teShaderFile != "" ? teShaderCode : nullptr,
        gShaderFile != "" ? gShaderCode : nullptr);
    return shader;
}

Mesh Assets::loadMeshFromFile(const string& filename)
{
    Mesh mesh;
    if(mesh.LoadMesh(filename)) Log::info("Loaded mesh " + filename);
    else Log::error(LogCategory::Application, "Failed to load mesh " + filename);
    return mesh;
}

Font Assets::loadFontFromFile(const string& filename)
{
    vector<int> fontSizes = {
        8, 9,
        10, 11, 12, 14, 16, 18,
        20, 22, 24, 26, 28,
        30, 32, 34, 36, 38,
        40, 42, 44, 46, 48,
        52, 56,
        60, 64, 68,
        72
    };

    Font font;
    for (auto& size : fontSizes)
    {
        TTF_Font* ttfFont = TTF_OpenFont(filename.c_str(), size);
        if (ttfFont == nullptr)
        {
            std::ostringstream s;
            s << "Failed to load Font " << filename << " in size " << size;
            Log::error(LogCategory::Application, s.str());
        }
        font.addFontData(size, ttfFont);
    }
    Log::info("Loaded font " + filename);
    return font;
}

std::vector<vector<int>> Assets::loadMapFromFile(const string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        Log::error(LogCategory::Application, "File not found: Map " + filename);
    }


    rapidjson::IStreamWrapper isw(file);
    rapidjson::Document doc;
    doc.ParseStream(isw);

    if (!doc.IsObject())
    {
        std::ostringstream s;
        s << "Map " << filename << " is not valid json";
        Log::error(LogCategory::Application, s.str());
    }
    

    const rapidjson::Value& position = doc["pos"];
    if (!position.IsArray() || position.Size() < 1)
    {
        std::ostringstream s;
        s << "Map " << filename << " has no positions";
        Log::error(LogCategory::Application, s.str());
    }


    std::vector<vector<int>> positions;

    for (const auto& arr : position.GetArray()) {
        std::vector<int> inner_vec;
        for (const auto& val : arr.GetArray()) {
            inner_vec.push_back(val.GetInt());
        }

        positions.push_back(inner_vec);
    }
    return positions;
}

ComputeShader Assets::loadComputeShaderFromFile(const std::string& cShaderFile)
{
    std::string computeCode;
    
    std::ifstream computeShaderFile(cShaderFile);
    std::stringstream cShaderStream;
    cShaderStream << computeShaderFile.rdbuf();
    computeShaderFile.close();

    computeCode = cShaderStream.str();

    const GLchar* cShaderCode = computeCode.c_str();

    //Create the compute shader
    ComputeShader cShader;
    cShader.compile(cShaderCode);
    return cShader;
}
