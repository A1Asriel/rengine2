#ifndef SCENELOADER_H
#define SCENELOADER_H

#include "Scene.h"

class SceneLoader {
public:
    static bool load(const std::string& file, Scene* scene);
    static bool save(const std::string& file, Scene* scene);
};

#endif
