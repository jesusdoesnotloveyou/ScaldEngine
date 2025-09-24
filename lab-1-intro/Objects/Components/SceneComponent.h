#pragma once

#include "ScaldComponent.h"
#include "Transform/TransformComponent.h"
#include "Graphics/DXHelper.h"

class SceneComponent : public ScaldComponent
{
	using Super = ScaldComponent;
public:
	SceneComponent();
	virtual ~SceneComponent() override;
	virtual void Update(const ScaldTimer& st) override;

public:

	template<typename T>
	std::vector<std::shared_ptr<T>> GetComponents() const
	{
		static_assert(std::is_base_of<ScaldComponent, T>::value, "Component must be derived from component class!");
		std::vector<std::shared_ptr<T>> foundComponents;

		for (const auto& comp : m_components)
		{
			if (std::shared_ptr<T> castedComp = std::dynamic_pointer_cast<T>(comp))
			{
				foundComponents.push_back(castedComp);
			}
		}
		return foundComponents;
	}

	// restriction for having one component of each class
	template<typename T>
	std::shared_ptr<T> GetComponent() const
	{
		static_assert(std::is_base_of<ScaldComponent, T>::value, "T must be a component class!");
		for (const auto& comp : m_components)
		{
			if (std::shared_ptr<T> castedComp = std::dynamic_pointer_cast<T>(comp))
			{
				return castedComp;
			}
		}
		return nullptr;
	}

	template<typename T, typename... Args>
	void AddComponent(Args&&... args)
	{
		static_assert(std::is_base_of<ScaldComponent, T>::value, "T must be a component class!");
	
		auto comp = std::make_shared<T>(/*owner*/shared_from_this(), std::forward<Args>(args)...);
		m_components.emplace_back(std::move(comp));
	}

	FORCEINLINE XMVECTOR GetPosition() const
	{
		return mTransformComponent->GetPositionVector();
	}

	FORCEINLINE XMVECTOR GetRotation() const
	{
		return mTransformComponent->GetRotationVector();
	}

	FORCEINLINE XMVECTOR GetOrientation() const
	{
		return mTransformComponent->GetOrientation();
	}

	FORCEINLINE XMVECTOR GetScale() const
	{
		return mTransformComponent->GetScaleVector();
	}

	virtual void SetPosition(const XMVECTOR& pos);
	virtual void SetPosition(float x, float y, float z);
	virtual void AdjustPosition(const XMVECTOR& pos);
	virtual void AdjustPosition(float x, float y, float z);

	virtual void SetOrientation(const XMVECTOR& newRotation);

	virtual void SetRotation(const XMVECTOR& rot);
	virtual void SetRotation(float x, float y, float z);
	virtual void AdjustRotation(const XMVECTOR& rot);
	virtual void AdjustRotation(float x, float y, float z);

	void SetScale(const XMVECTOR& scale);
	void SetScale(float x, float y, float z);
	void AdjustScale(const XMVECTOR& scale);
	void AdjustScale(float x, float y, float z);

	FORCEINLINE XMVECTOR GetForwardVector()const
	{
		return mTransformComponent->GetForwardVector();
	}

	FORCEINLINE XMVECTOR GetRightVector()const
	{
		return mTransformComponent->GetRightVector();
	}

	FORCEINLINE XMVECTOR GetUpVector()const
	{
		return mTransformComponent->GetUpVector();
	}

	void SetForwardVector(const XMVECTOR& relativeForwardVector);
	void SetRightVector(const XMVECTOR& relativeRightVector);
	void SetUpVector(const XMVECTOR& relativeUpVector);


	void AttachToParent(SceneComponent* Parent);
	FORCEINLINE SceneComponent* GetParent() { return mParent; }

	FORCEINLINE SceneComponent* GetRootObject()
	{
		if (mParent)
		{
			return mParent->GetRootObject();
		}
		return this;
	}
	

	FORCEINLINE TransformComponent* GetTransform()const { return mTransformComponent; }

private:
	std::vector<std::shared_ptr<ScaldComponent>> m_components;

	SceneComponent* mParent = nullptr;
	TransformComponent* mTransformComponent = nullptr;
	std::vector<SceneComponent*> mChildren{};
};