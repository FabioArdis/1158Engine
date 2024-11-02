#include "../include/ScriptBase.h"

class PlayerController : public ScriptBase
{
public:
    void OnUpdate(float deltaTime) override
    {
        auto transform = GetComponent<TransformComponent>();
        if (transform->GetPosition().z >= 10.0f)
        {
            isIncreasing = false;

        }            
        else if (transform->GetPosition().z <= -10.0f)
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
            {"isIncreasing", PropertyType::Bool},
        };
    }

    void* GetPropertyPtr(const std::string& name) override
    {
        if (name == "Speed") return &m_Speed;
        if (name == "Health") return &m_Health;
        if (name == "Stuff") return &m_stuff;
        if (name == "isIncreasing") return &isIncreasing;

        return nullptr;
    }

private:
    float m_Speed = 0.1f;
    float m_Health = 100.0f;
    float m_stuff = 100.0f;

    bool isIncreasing = true;
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