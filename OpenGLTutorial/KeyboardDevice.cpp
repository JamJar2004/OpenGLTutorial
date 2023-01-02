#include "KeyboardDevice.hpp"

void KeyboardDevice::Update(const SDL_Event& e)
{
	switch(e.type)
	{
		case SDL_KEYDOWN:
			m_keys[e.key.keysym.scancode] = true;
			break;
		case SDL_KEYUP:
			m_keys[e.key.keysym.scancode] = false;
			break;
	}
}
