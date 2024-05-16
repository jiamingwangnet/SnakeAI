#pragma once

namespace GEngine
{
	class FPSManager
	{
	public:
		FPSManager(float targetFPS);

		void SetTargetFPS(float target) { this->target = target; };

		void BeginLimiter();
		float EndLimiter(); // return FPS;

		float GetFrameTime() const { return frameTime; };

	private:
		void CalculateFPS();

	private:
		float fps;
		float frameTime;
		float target;
		unsigned int startTicks;
	};
}