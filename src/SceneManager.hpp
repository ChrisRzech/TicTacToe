#pragma once
#include "Scene.hpp"
#include <unordered_map>

class SceneManager
{
public:
    ~SceneManager();
    
    static void addScene(const std::string& name, Scene*);
    static void changeScene(const std::string& name);
    static Scene* getScene();

private:
    SceneManager();
    SceneManager(const SceneManager&) = delete;
    void operator=(const SceneManager&) = delete;
    
    static SceneManager* instance;
    
    Scene* m_currentScene;
    std::unordered_map<std::string, Scene*> m_scenes;
};