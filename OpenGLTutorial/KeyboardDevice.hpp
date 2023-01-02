#pragma once

class Window;

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <unordered_map>

class KeyboardDevice
{
private:
	std::unordered_map<SDL_Scancode, bool> m_keys;
public:
	KeyboardDevice() {}

	bool IsKeyDown(SDL_Scancode keyCode) { return m_keys[keyCode]; }

	void Update(const SDL_Event& e);

	friend class Window;
};