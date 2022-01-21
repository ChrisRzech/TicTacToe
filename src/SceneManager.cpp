#include "SceneManager.hpp"

SceneManager* SceneManager::m_instance;

SceneManager::~SceneManager()
{
    for(std::pair<std::string, Scene*> pair : m_scenes)
    {
        delete pair.second;
    }
}

void SceneManager::addScene(const std::string& name, Scene* scene)
{
    if(m_instance == nullptr)
        m_instance = new SceneManager;
    
    m_instance->m_scenes.insert(std::make_pair(name, scene));
}

void SceneManager::changeScene(const std::string& name)
{
    if(m_instance == nullptr)
        m_instance = new SceneManager;
    
    m_instance->m_currentScene = m_instance->m_scenes.at(name);
}

Scene* SceneManager::getScene()
{
    if(m_instance == nullptr)
        m_instance = new SceneManager;
    
    return m_instance->m_currentScene;
}

SceneManager::SceneManager()
    : m_currentScene{nullptr}
{
    
}