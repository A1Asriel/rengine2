#include "SceneLoader.h"

#include <fstream>
#include <sstream>
#include <vector>

#include "Logging.h"
#include "Texture.h"

bool SceneLoader::load(const std::string& file, Scene* scene) {
    scene->nodes.clear();
    scene->camera.position = glm::vec3(0.0f);
    scene->camera.rotation = glm::vec3(0.0f);
    scene->camera.fov = 45.0f;

    std::ifstream input(file);
    if (!input.is_open()) {
        return false;
    }

    std::string line;

    std::getline(input, line);
    if (line != "[RENGINE MAP FORMAT V1.0]") {
        ERROR("Invalid file format");
        return false;
    }

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

        SceneNode node;
        if (tokens[0] == "camera") {
            if (tokens.size() < 8) {
                ERROR("Line \"" << line << "\" has incorrect amount of parameters");
                continue;
            }
            try {
                scene->camera.position.x = std::stof(tokens[1]);
                scene->camera.position.y = std::stof(tokens[2]);
                scene->camera.position.z = std::stof(tokens[3]);
                scene->camera.rotation.x = std::stof(tokens[4]);
                scene->camera.rotation.y = std::stof(tokens[5]);
                scene->camera.rotation.z = std::stof(tokens[6]);
                scene->camera.fov = std::stof(tokens[7]);
            } catch(const std::exception& e) {
                ERROR("Line \"" << line << "\" could not be interpreted");
                scene->camera.position = glm::vec3(0.0f);
                scene->camera.rotation = glm::vec3(0.0f);
                scene->camera.fov = 45.0f;
                continue;
            }
        } else {
            if (tokens.size() < 11) {
                ERROR("Line \"" << line << "\" has incorrect amount of parameters");
                continue;
            }
            if (tokens[0] == "cube")
                node.mesh = MeshType::Cube;
            else if (tokens[0] == "sphere")
                node.mesh = MeshType::Sphere;
            else {
                ERROR("Line \"" << line << "\" has incorrect mesh type");
                continue;
            }
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
                node.distort = tokens[10] == "true";
                if (tokens.size() >= 12 && !tokens[11].empty()) {
                    node.texturePath = tokens[11];
                    DEBUG("Loaded texture path: " << node.texturePath);
                }
            } catch(const std::exception& e) {
                ERROR("Line \"" << line << "\" could not be interpreted");
                continue;
            }
            scene->nodes.push_back(node);
        }
    }

    return true;
}

bool SceneLoader::save(const std::string& file, Scene* scene) {
    // TODO: To be implemented
    ERROR("SceneLoader::save not implemented");
    return false;
}
