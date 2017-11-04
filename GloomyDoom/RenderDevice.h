#pragma once

#ifndef RENDERDEVICE_
#define RENDERDEVICE_

using namespace std;

namespace mjh {

	class RenderDevice {

	private:

		int	BUFFER_WIDTH =		320;
		int BUFFER_HEIGHT =		200;
		int BUFFER_LENGTH =		BUFFER_WIDTH * BUFFER_HEIGHT;
		int WINDOW_WIDTH =		320;
		int WINDOW_HEIGHT =		200;
		int WINDOW_LENGTH =		WINDOW_WIDTH * WINDOW_HEIGHT;

		int	*backBuffer =		NULL;
		int *windowBuffer =		NULL;
		float *zBuffer =		NULL;
		
	public:

		~RenderDevice();


		// housekeeping
		void initialize(int bufferWidth, int bufferHeight, int windowWidth, int windowHeight);
		void release();


		// buffer operations
		void clear();
		void clearZBuffer();
		void renderWindowBuffer();


		// pixel operations
		void putPixel(int x, int y, int color);


		// line operations
		void lineClippedToEdges(int x1, int y1, int x2, int y2, int color);


		// getters
		int *getWindowBuffer();
		int getBufferWidth();
		int getBufferHeight();
		int getWindowWidth();
		int getWindowHeight();
	};
}
#endif // RENDERDEVICE_