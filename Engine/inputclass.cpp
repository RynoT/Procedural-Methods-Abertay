////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "inputclass.h"

InputClass::InputClass(): m_MouseX(0), m_MouseY(0), m_Left(false), m_Right(false)
{
	for (int i = 0; i < KEY_COUNT; i++) //set all keys to false on initialization
	{
		this->m_Keys[i] = false;
	}
}

void InputClass::Update()
{
	this->m_Pressed.clear();
}
