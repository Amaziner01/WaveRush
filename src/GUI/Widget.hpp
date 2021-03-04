#pragma once

#include "Core/PCH.hpp"
#include "Math/Vec2.hpp"

class Widget {
protected:
	Vec2f m_Position;

public:
	explicit Widget(const Vec2f& p_Position = Vec2f());

	virtual ~Widget() = default;

	virtual void ProcessEvents(SDL_Event& p_Event);

	virtual void ProcessUpdate(float p_DeltaTime);

	virtual void ProcessRender(SDL_Renderer* p_Renderer);
};