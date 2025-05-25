#include "SceneLoader.h"

#include <fstream>
#include <sstream>
#include <vector>

#include "Logging.h"
#include "Texture.h"
#include "CubeMesh.h"
#include "SphereMesh.h"

bool REngine::SceneLoader::load(const std::string& file, Scene* scene) {
    scene->nodes.clear();

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

        if (tokens[0] == "camera") {
            if (tokens.size() < 8) {
                ERROR("Line \"" << line << "\" has incorrect amount of parameters");
                continue;
            }
            try {
                scene->camera.position.x = std::stof(tokens[1]);
                scene->camera.position.y = std::stof(tokens[2]);
                scene->camera.position.z = std::stof(tokens[3]);
                DEBUG("Loaded camera position: " << scene->camera.position.x << ", " << scene->camera.position.y << ", " << scene->camera.position.z);
                scene->camera.rotation.x = std::stof(tokens[4]);
                scene->camera.rotation.y = std::stof(tokens[5]);
                scene->camera.rotation.z = std::stof(tokens[6]);
                DEBUG("Loaded camera rotation: " << scene->camera.rotation.x << ", " << scene->camera.rotation.y << ", " << scene->camera.rotation.z);
                scene->camera.fov = std::stof(tokens[7]);
                DEBUG("Loaded camera fov: " << scene->camera.fov);
            } catch(const std::exception& e) {
                ERROR("Line \"" << line << "\" could not be interpreted");
                continue;
            }
        } else if (tokens[0] == "lighting") {
            if (tokens.size() < 10) {
                ERROR("Line \"" << line << "\" has incorrect amount of parameters");
                continue;
            }
            try {
                scene->skyColor.x = std::stof(tokens[1]);
                scene->skyColor.y = std::stof(tokens[2]);
                scene->skyColor.z = std::stof(tokens[3]);
                DEBUG("Loaded sky color: " << scene->skyColor.x << ", " << scene->skyColor.y << ", " << scene->skyColor.z);
                scene->lightingColor.x = std::stof(tokens[4]);
                scene->lightingColor.y = std::stof(tokens[5]);
                scene->lightingColor.z = std::stof(tokens[6]);
                DEBUG("Loaded lighting color: " << scene->lightingColor.x << ", " << scene->lightingColor.y << ", " << scene->lightingColor.z);
                scene->lightingPosition.x = std::stof(tokens[7]);
                scene->lightingPosition.y = std::stof(tokens[8]);
                scene->lightingPosition.z = std::stof(tokens[9]);
                DEBUG("Loaded lighting position: " << scene->lightingPosition.x << ", " << scene->lightingPosition.y << ", " << scene->lightingPosition.z);
            } catch(const std::exception& e) {
                ERROR("Line \"" << line << "\" could not be interpreted");
                continue;
            }
        } else {
            if (tokens.size() < 21) {
                ERROR("Line \"" << line << "\" has incorrect amount of parameters");
                continue;
            }
            SceneNode node;
            if (tokens[0] == "cube")
                node.mesh = (Mesh*)new CubeMesh();
            else if (tokens[0] == "sphere")
                node.mesh = (Mesh*)new SphereMesh(40, 40);
            else {
                ERROR("Line \"" << line << "\" has incorrect mesh type");
                continue;
            }
            try {
                DEBUG("Loading node \"" << tokens[0] << "\"");
                node.position.x = std::stof(tokens[1]);
                node.position.y = std::stof(tokens[2]);
                node.position.z = std::stof(tokens[3]);
                DEBUG("Loaded node position: " << node.position.x << ", " << node.position.y << ", " << node.position.z);
                node.rotation.x = std::stof(tokens[4]);
                node.rotation.y = std::stof(tokens[5]);
                node.rotation.z = std::stof(tokens[6]);
                DEBUG("Loaded node rotation: " << node.rotation.x << ", " << node.rotation.y << ", " << node.rotation.z);
                node.scale.x = std::stof(tokens[7]);
                node.scale.y = std::stof(tokens[8]);
                node.scale.z = std::stof(tokens[9]);
                DEBUG("Loaded node scale: " << node.scale.x << ", " << node.scale.y << ", " << node.scale.z);
                node.ambient.x = std::stof(tokens[10]);
                node.ambient.y = std::stof(tokens[11]);
                node.ambient.z = std::stof(tokens[12]);
                DEBUG("Loaded node ambient: " << node.ambient.x << ", " << node.ambient.y << ", " << node.ambient.z);
                node.diffuse.x = std::stof(tokens[13]);
                node.diffuse.y = std::stof(tokens[14]);
                node.diffuse.z = std::stof(tokens[15]);
                DEBUG("Loaded node diffuse: " << node.diffuse.x << ", " << node.diffuse.y << ", " << node.diffuse.z);
                node.specular.x = std::stof(tokens[16]);
                node.specular.y = std::stof(tokens[17]);
                node.specular.z = std::stof(tokens[18]);
                DEBUG("Loaded node specular: " << node.specular.x << ", " << node.specular.y << ", " << node.specular.z);
                node.shininess = std::stof(tokens[19]);
                DEBUG("Loaded node shininess: " << node.shininess);
                node.distort = tokens[20] == "true";
                DEBUG("Loaded node distort: " << node.distort);
                if (tokens.size() >= 22 && !tokens[21].empty()) {
                    node.texturePath = tokens[21];
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

bool REngine::SceneLoader::save(const std::string& file, Scene* scene) {
    // TODO: To be implemented
    ERROR("SceneLoader::save not implemented");
    return false;
}
