#pragma once

#ifndef INPUT_
#define INPUT_

namespace mjh
{
	static int lineStart = 0; 
	static int lineEnd = 16; // 60 starts floor center stair shits
	// 200 - 300 for weird secret walls by center acid circular pool
	static bool useDebugLines = false;
	static int rand1 = rand() % 128;
	static int rand2 = rand() % 128;

	class Input
	{
		private:
			
			bool keyDownBuffer[512];
			bool pressedBuffer[512];
			int currentMouseX;
			int currentMouseY;
			int mouseDeltaX;
			int mouseDeltaY;
			bool first;

		public:

			Input();
			void keyDown(int keyNumber);
			void keyUp(int keyNumber);
			void toggleKey(int keyNumber);
			bool isKeyDown(int keyNumber);
			void updateMouse(int currX, int currY);
			void getMouseMovement(int &dx, int &dy);
			void getMouseCurrentPosition(int &x, int &y);
			void clear();
			bool keyWasPressed(int keyNumber);
	};
}

#endif