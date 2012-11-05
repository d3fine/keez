#pragma once

// XTst module wrapper class

class XTest
{
	public:
		static void start(void (*)(int key, bool down));
		static void stop();
	private:
		static bool mRunning;
};
