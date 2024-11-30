#include "../include/ScriptBase.h"

class PlayerController : public ScriptBase
{
public:
    void OnUpdate(float deltaTime) override
    {
        auto transform = GetComponent<TransformComponent>();

        if (!transform)
        {
            std::cerr << "TransformComponent is not available!" << std::endl;
            return;
        }

        if (nome == "ciao") {
          transform->SetScale(glm::vec3(1.5f, 1.5f, 1.5f));
        } else {
          transform->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
        }

        if (transform->GetPosition().z >= maxRange)
        {
            isIncreasing = false;

        }            
        else if (transform->GetPosition().z <= minRange)
        {
            isIncreasing = true;
        }

        if (isIncreasing)
            transform->SetPosition(transform->GetPosition() - transform->GetForward() * m_Speed * deltaTime);
        else
            transform->SetPosition(transform->GetPosition() + transform->GetForward() * m_Speed * deltaTime);
    }

    std::vector<std::pair<std::string, PropertyType>> GetProperties() const override
    {
        return
        {
            {"Speed", PropertyType::Float},
            {"Health", PropertyType::Float},
            {"Stuff", PropertyType::Float},
            {"minRange", PropertyType::Float},
            {"maxRange", PropertyType::Float},
            {"isIncreasing", PropertyType::Bool},
            {"nome", PropertyType::String},
        };
    }

    void* GetPropertyPtr(const std::string& name) override
    {
        if (name == "Speed") return &m_Speed;
        if (name == "Health") return &m_Health;
        if (name == "Stuff") return &m_stuff;
        if (name == "isIncreasing") return &isIncreasing;
        if (name == "minRange") return &minRange;
        if (name == "maxRange") return &maxRange;
        if (name == "nome") return &nome;

        return nullptr;
    }

private:
    float m_Speed = 10.0f;
    float m_Health = 100.0f;
    float m_stuff = 100.0f;

    float maxRange = 10.0f;
    float minRange = -10.0f;

    bool isIncreasing = true;

    std::string nome = "Ciao!";
};

extern "C"
{
    API_EXPORT ScriptBase* CreateScript()
    {
        return new PlayerController();
    }

    API_EXPORT void DestroyScript(ScriptBase* script)
    {
        delete script;
    }
}