#pragma once

#include <chrono>

class FTimer
{
public:
	FTimer()
	{
		Reset();
	}

	void Reset()
	{
		Start = std::chrono::high_resolution_clock::now();
	}

	float Elapsed() const
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(
			std::chrono::high_resolution_clock::now() - Start
		).count() * 0.001f * 0.001f * 0.001f;
	}

	float ElapsedMs() const
	{
		return Elapsed() * 1000.0f;
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> Start;
};
