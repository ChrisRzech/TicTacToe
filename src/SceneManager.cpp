#include "SceneManager.hpp"

SceneManager* SceneManager::instance;

SceneManager::~SceneManager()
{
    for(const std::pair<const std::string, Scene*>& pair : m_scenes)
        delete pair.second;
}

void SceneManager::changeScene(const std::string& name)
{
    if(instance == nullptr)
        instance = new SceneManager;
    
    if(instance->m_currentScene != nullptr)
        instance->m_currentScene->exit();
    
    instance->m_currentScene = instance->m_scenes.at(name);
    instance->m_currentScene->enter();
}

Scene& SceneManager::getScene()
{
    if(instance == nullptr)
        instance = new SceneManager;
    
    return *instance->m_currentScene;
}

SceneManager::SceneManager()
    : m_currentScene{nullptr}
{
    
}