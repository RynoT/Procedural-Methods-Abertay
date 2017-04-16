#pragma once

#include <vector>

#define KEY_COUNT 256

#define VK_Q 0x51
#define VK_E 0x45

#define VK_B 0x42
#define VK_C 0x43
#define VK_V 0x56
#define VK_X 0x58
#define VK_Z 0x5a

#define VK_W 0x57
#define VK_A 0x41 
#define VK_S 0x53
#define VK_D 0x44

#define VK_SPACE 0x20

enum class MouseButton
{
	LEFT, RIGHT
};

class InputClass
{
public:
	InputClass();

	void Update();

	inline const int& GetMouseX() const { return this->m_MouseX; }
	inline const int& GetMouseY() const { return this->m_MouseY; }

	inline bool IsKeyPressed(unsigned int key) const { return std::find(this->m_Pressed.begin(), this->m_Pressed.end(), key) != this->m_Pressed.end(); }
	inline const bool& IsKeyDown(unsigned int key) const { return this->m_Keys[key]; }
	inline const bool& IsMouseDown(const MouseButton& button) const { return button == MouseButton::RIGHT ? this->m_Right : this->m_Left; }

	inline void SetKey(unsigned int key, bool value) { this->m_Keys[key] = value; if (!value) this->m_Pressed.push_back(key); }
	inline void SetMouseButton(bool left, bool down) { if (left) this->m_Left = down; else this->m_Right = down; }
	inline void SetMousePosition(int x, int y) { this->m_MouseX = x; this->m_MouseY = y; }

private:
	bool m_Keys[KEY_COUNT];
	std::vector<unsigned int> m_Pressed;

	int m_MouseX, m_MouseY;
	bool m_Left, m_Right;
};