#pragma once
#include <chrono>


#define TIMERERROR (-1)

// Este Timer usa Chrono para saber cuanto tiempo pasa desde que empieza hasta que se para. Al llamar al constructor se empieza el timer.
// IMPORTANTE: Cuando estas debugeando, aunque uses un breakpoint, esta funcion igual va a tomar todo el tiempo que paso en el medio.

class Timer
{
public:
	Timer();
	~Timer();
	// Empieza el timer
	void start();
	// Para el timer
	void stop();
	// Hace la resta de ambos tiempos para calcular (en ms) cuanto tiempo paso entre que se activo y desactivo
	float getTime(); // In ms
private:
	std::chrono::high_resolution_clock::time_point  startValue;
	bool getStartValue;
	std::chrono::high_resolution_clock::time_point  stopValue;
	bool getStopValue;
	std::chrono:: duration<float> timePassed;
};

