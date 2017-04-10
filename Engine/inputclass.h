////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

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

////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////
class InputClass
{
public:
	InputClass();
	~InputClass();

	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	bool m_keys[KEY_COUNT];
};

#endif