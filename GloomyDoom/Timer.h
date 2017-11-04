#pragma once
namespace mjh
{

	class Timer
	{

		const float MICRO_TO_MILLI =	1000.0f;
		const float NANO_TO_MICRO =		1000000.0f;
		
	private:

		double lastStampMicro;

		double deltaMicro;
		double deltaMilli;

	public:

		const int SECONDS_IN_MINUTE = 60;

		Timer();

		// getters
		double getTimestampMicro();
		double getTimestampMilli();
		double getLastTimestampMicro();
		double getLastTimestampMilli();
		double getDeltaMicro();
		double getDeltaMilli();

	};
}
