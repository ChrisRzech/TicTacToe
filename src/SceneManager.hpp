#pragma once
#include "Scene.hpp"
#include <unordered_map>

class SceneManager
{
public:
    ~SceneManager();
    
    template<typename SceneType, typename... SceneTypeParams>
    static void addScene(const std::string& name, const SceneTypeParams&...);
    static void changeScene(const std::string& name);
    static Scene& getScene();

private:
    SceneManager();
    SceneManager(const SceneManager&) = delete;
    void operator=(const SceneManager&) = delete;
    
    static SceneManager* instance;
    
    Scene* m_currentScene;
    std::unordered_map<std::string, Scene*> m_scenes;
};

template<typename SceneType, typename... SceneTypeParams>
void SceneManager::addScene(const std::string& name, const SceneTypeParams&... params)
{
    if(instance == nullptr)
        instance = new SceneManager;
    
    instance->m_scenes.insert(std::make_pair(name, new SceneType{params...}));
}