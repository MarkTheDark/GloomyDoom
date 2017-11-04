#include "stdafx.h"

namespace mjh
{
	RenderDevice::~RenderDevice()
	{
		release();
	}

	void RenderDevice::initialize(int bufferWidth, int bufferHeight, int windowWidth, int windowHeight)
	{
		release();

		BUFFER_WIDTH =	bufferWidth;
		BUFFER_HEIGHT = bufferHeight;
		BUFFER_LENGTH = BUFFER_WIDTH * BUFFER_HEIGHT;
		WINDOW_WIDTH =	windowWidth;
		WINDOW_HEIGHT = windowHeight;
		WINDOW_LENGTH =	WINDOW_WIDTH * WINDOW_HEIGHT;

		backBuffer =	new int[BUFFER_LENGTH];
		zBuffer =		new float[BUFFER_LENGTH];
		windowBuffer =	new int[WINDOW_LENGTH];
	}

	void RenderDevice::release()
	{
		if (backBuffer)
		{
			delete[] backBuffer;
			backBuffer = NULL;
		}

		if (zBuffer)
		{
			delete[] zBuffer;
			zBuffer = NULL;
		}

		if (windowBuffer)
		{
			delete[] windowBuffer;
			windowBuffer = NULL;
		}
	}


	// buffer operations
	void RenderDevice::clear()
	{
		ZeroMemory(backBuffer, BUFFER_LENGTH * sizeof(int));
	}

	void RenderDevice::clearZBuffer()
	{
		memset(zBuffer, 127, BUFFER_LENGTH * sizeof(int));
	}

	void RenderDevice::renderWindowBuffer()
	{
		if (BUFFER_WIDTH != WINDOW_WIDTH || BUFFER_HEIGHT != WINDOW_HEIGHT)
		{
			float dx = (float)BUFFER_WIDTH / (float)WINDOW_WIDTH;
			float dy = (float)BUFFER_HEIGHT / (float)WINDOW_HEIGHT;
			float sy = 0;
			for (int y = 0; y < WINDOW_HEIGHT; y++)
			{
				float sx = 0;
				int yOffset = y * WINDOW_WIDTH;
				for (int x = 0; x < WINDOW_WIDTH; x++)
				{
					windowBuffer[x + yOffset] = backBuffer[(int)sx + (int)sy * BUFFER_WIDTH];
					sx += dx;
				}
				sy += dy;
			}
		}
		else
		{
			memcpy(windowBuffer, backBuffer, BUFFER_LENGTH * 4);
		}
	}


	// pixel operations
	void RenderDevice::putPixel(int x, int y, int color)
	{
		backBuffer[x + y * BUFFER_WIDTH] = color;
	}

	
	// line operations
	void RenderDevice::lineClippedToEdges(int x1, int y1, int x2, int y2, int color)
	{
		const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
		if (steep)
		{
			std::swap(x1, y1);
			std::swap(x2, y2);
		}

		if (x1 > x2)
		{
			std::swap(x1, x2);
			std::swap(y1, y2);
		}

		float dx = (float)(x2 - x1);
		float dy = (float)fabs(y2 - y1);

		float error = dx / 2.0f;
		const int ystep = (y1 < y2) ? 1 : -1;
		int y = (int)y1;

		const int maxX = (int)x2;

		for (int x = (int)x1; x < maxX; x++)
		{
			if (steep)
			{
				if (x > -1 && y > -1 && y < BUFFER_WIDTH && x < BUFFER_HEIGHT)
				backBuffer[y + x * BUFFER_WIDTH] = color;
			}
			else
			{
				if (x > -1 && y > -1 && x < BUFFER_WIDTH && y < BUFFER_HEIGHT)
					backBuffer[x + y * BUFFER_WIDTH] = color;
			}

			error -= dy;
			if (error < 0)
			{
				y += ystep;
				error += dx;
			}
		}
	}


	// getters
	int RenderDevice::getBufferWidth() 
	{ 
		return BUFFER_WIDTH; 
	}

	int RenderDevice::getBufferHeight() 
	{ 
		return BUFFER_HEIGHT; 
	}
	
	int RenderDevice::getWindowWidth() 
	{ 
		return WINDOW_WIDTH; 
	}
	
	int RenderDevice::getWindowHeight() 
	{ 
		return WINDOW_HEIGHT; 
	}
	
	int *RenderDevice::getWindowBuffer() 
	{ 
		return windowBuffer; 
	}
}