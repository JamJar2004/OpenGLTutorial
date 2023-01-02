#pragma once

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <unordered_map>

class Window;

enum MouseButton
{
	LEFT   = SDL_BUTTON_LEFT,
	MIDDLE = SDL_BUTTON_LEFT,
	RIGHT  = SDL_BUTTON_LEFT,
};

class MouseDevice
{
private:
	Window* m_window;

	glm::vec2 m_position;

	std::unordered_map<MouseButton, bool> m_buttons;

	
public:
	MouseDevice(Window* window) : m_window(window), m_position(glm::vec2(0)) {}

	bool IsButtonDown(MouseButton button) { return m_buttons[button]; }

	glm::vec2 GetPosition() const { return m_position; }

	void SetPosition(const glm::vec2& position);

	void Update(const SDL_Event& e);

	friend class Window;
};