#include "stdafx.h"

bool keyDownBuffer[256];

namespace mjh
{
	Input::Input()
	{
		currentMouseX = 0;
		currentMouseY = 0;
		first = true;
		for (int x = 0; x < 512; x++)
			keyDownBuffer[x] = false;
	}

	void Input::keyDown(int x)
	{
		keyDownBuffer[x] = true;
	}

	void Input::keyUp(int x)
	{
		if (keyDownBuffer[x])
			pressedBuffer[x] = true;
		keyDownBuffer[x] = false;
	}

	void Input::toggleKey(int x)
	{
		if (keyDownBuffer[x])
			keyDownBuffer[x] = false;
		else
			keyDownBuffer[x] = true;
	}

	bool Input::isKeyDown(int keyNumber)
	{
		return keyDownBuffer[keyNumber];
	}

	void Input::updateMouse(int x, int y)
	{
		if (!first)
		{
			mouseDeltaX = x - currentMouseX;
			mouseDeltaY = y - currentMouseY;
		}
		else
		{
			mouseDeltaX = mouseDeltaY = 0;
			first = false;
		}
		currentMouseX = x;
		currentMouseY = y;
	}

	void Input::getMouseMovement(int &dx, int &dy)
	{
		dx = mouseDeltaX;
		dy = mouseDeltaY;
	}

	void Input::getMouseCurrentPosition(int &x, int &y)
	{
		x = currentMouseX;
		y = currentMouseY;
	}

	void Input::clear()
	{
		updateMouse(currentMouseX, currentMouseY);
		for (int i = 0; i < 512; i++)
			pressedBuffer[i] = false;
	}

	bool Input::keyWasPressed(int keyNumber)
	{
		return pressedBuffer[keyNumber];
	}
}