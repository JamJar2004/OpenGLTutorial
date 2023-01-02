#include "MouseDevice.hpp"
#include "Window.hpp"

void MouseDevice::Update(const SDL_Event& e)
{
	float x;
	float y;

	switch(e.type)
	{
		case SDL_MOUSEMOTION:
			x = (e.motion.x / (float)m_window->Width ) * 2 - 1;
			y = (e.motion.y / (float)m_window->Height) * 2 - 1;
			m_position = glm::vec2(x, -y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_buttons[MouseButton(e.button.button)] = true;
			break;
		case SDL_MOUSEBUTTONUP:
			m_buttons[MouseButton(e.button.button)] = false;
			break;
	}
}

void MouseDevice::SetPosition(const glm::vec2& position)
{
	int x = int((position.x * 0.5 + 0.5) * m_window->Width);
	int y = int((position.y * 0.5 + 0.5) * m_window->Height);
	SDL_WarpMouseInWindow(m_window->m_window, x, y);
}
