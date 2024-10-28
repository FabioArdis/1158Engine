#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "Component.h"
#include "TransformComponent.h"
#include "LightComponent.h"

/**
 * @class GameObject
 * @brief Represents a game object that can have various components attached.
 * 
 * The GameObject class serves as a container for different components,
 * allowing for modular functionality in the game engine. It manages
 * components, updates their state, and handles rendering.
 */
class GameObject
{
public:

	/**
     * @brief Constructs a GameObject with a specified name.
     * 
     * Initializes a TransformComponent and adds it to the game object.
     * Every GameObject must have at least a TransformComponent.
	 * 
     * @param name The name of the game object.
     */
	GameObject(std::string name) : transformComponent(new TransformComponent(this)), m_name(name)
	{
		m_components.push_back(transformComponent);
	}

	/**
     * @brief Destroys the GameObject and cleans up components.
     * 
     * Deletes all components associated with the game object to free resources.
     */
	~GameObject()
	{
		for (Component* component : m_components)
		{
			delete component;
		}
	}

	/**
     * @brief Adds a component to the game object.
     * 
     * @param component Pointer to the component to be added.
     * 
     * This method prevents adding multiple TransformComponents.
     */
	void AddComponent(Component* component)
	{
		if (dynamic_cast<TransformComponent*>(component))
			return;

		m_components.push_back(component);
	}

	/**
     * @brief Template method to add a component of a specific type.
     * 
     * @tparam T The type of the component to add.
     * @tparam Args Variadic template parameters for constructor arguments.
     * 
     * @param args Constructor arguments for the component.
     * @return Pointer to the newly created component, or nullptr if the component is TransformComponent.
     */
	template<typename T, typename... Args>
	T* AddComponent(Args&&... args)
	{
		if (std::is_same<T, TransformComponent>::value)
		{
			return nullptr;
		}

		T* component = new T(this, std::forward<Args>(args)...);

		m_components.push_back(component);
		
		return component;
	}

	/**
     * @brief Retrieves a component of a specified type.
     * 
     * @tparam T The type of the component to retrieve.
     * @return Pointer to the component of type T, or nullptr if not found.
     */
	template<typename T>
	T* GetComponent()
	{
		for (Component* component : m_components)
		{
			T* castedComponent = dynamic_cast<T*>(component);
			if (castedComponent)
			{
				return castedComponent;
			}
		}

		return nullptr;
	}

	/**
     * @brief Updates the game object and its components.
     * 
     * @param deltaTime The time elapsed since the last update.
     */
	void Update(float deltaTime)
	{
		//std::cout << "\tUpdating " << m_name << "." << std::endl; Will reuse this in the Log Dock
		for (Component* component : m_components)
		{
			component->Update(deltaTime);
		}
	}

	/**
     * @brief Renders the game object and its components.
     */
	void Render()
	{
		std::cout << "\tRendering " << m_name << "." << std::endl;
		for (Component* component : m_components)
		{
			component->Render();
		}
	}

	/**
     * @brief Retrieves the name of the game object.
     * 
     * @return The name of the game object.
     */
	std::string GetName()
	{
		return m_name;
	}

	/**
     * @brief Sets the name of the game object.
     * 
     * @param name The new name for the game object.
     */
	void SetName(std::string name)
	{
		m_name = name;
	}

private:
    std::string m_name;           			/**The name of the game object. */
    TransformComponent* transformComponent; /**Pointer to the TransformComponent associated with the game object. */
    std::vector<Component*> m_components; 	/**List of components attached to the game object. */
};

