#include "SceneLoader.h"

#include <fstream>
#include <sstream>
#include <vector>

#include "Logging.h"
#include "Texture.h"

bool SceneLoader::load(const std::string& file, Scene* scene) {
    std::ifstream input(file);
    if (!input.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(input, line)) {
        std::istringstream iss(line);
        std::string token;
        if (line.length() < 2 || line.substr(0,2) == "//") {
            continue;
        }

        std::vector<std::string> tokens;
        while (std::getline(iss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 10) {
            ERROR("Line \"" << line << "\" has incorrect amount of parameters");
            continue;
        }

        SceneNode node;
        if (tokens[0] == "cube")
            node.mesh = MeshType::Cube;
        else if (tokens[0] == "sphere")
            node.mesh = MeshType::Sphere;
        else
            node.mesh = MeshType::NotImplemented;
        try {
            node.position.x = std::stof(tokens[1]);
            node.position.y = std::stof(tokens[2]);
            node.position.z = std::stof(tokens[3]);
            node.rotation.x = std::stof(tokens[4]);
            node.rotation.y = std::stof(tokens[5]);
            node.rotation.z = std::stof(tokens[6]);
            node.scale.x = std::stof(tokens[7]);
            node.scale.y = std::stof(tokens[8]);
            node.scale.z = std::stof(tokens[9]);
            if (tokens.size() >= 11 && !tokens[10].empty()) {
                node.texturePath = tokens[10];
                DEBUG("Loaded texture path: " << node.texturePath);
            }
        } catch(const std::exception& e) {
            ERROR("Line \"" << line << "\" could not be interpreted");
            continue;
        }

        scene->nodes.push_back(node);
    }

    return true;
}

bool SceneLoader::save(const std::string& file, Scene* scene) {
    // TODO: To be implemented
    ERROR("SceneLoader::save not implemented");
    return false;
}
