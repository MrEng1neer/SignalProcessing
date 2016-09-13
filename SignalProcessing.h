/************************
* Under development
* Be carefull
* MrEng1neer@ 09.2016
* Version 0.1
*************************/
#include <Arduino.h>

class SignalProcessing
{
public:
	SignalProcessing(float cuttOffFrequency_LPF, float cuttOffFrequency_HPF)
	{
		//HIGH PASS FILTER init
		lastDataIn_HPF = 0.;
		lastTime_HPF = 0.;
		LastDataFiltered_HPF = 0.;
		tau_HPF = 159154.94309/cuttOffFrequency_HPF;
		//LOW PASS FILTER init
		lastTime_LPF  = 0.;
		lastDataIn_LPF = 0.;
		tau_LPF = 159154.94309/cuttOffFrequency_LPF;
	}//END
	
	SignalProcessing(void)
	{
		//HIGH PASS FILTER init
	    lastDataIn_HPF = 0.;
	    lastTime_HPF = 0.;
		LastDataFiltered_HPF = 0.;
		//LOW PASS FILTER init
	    lastTime_LPF  = 0.;
		lastDataIn_LPF = 0.;
	}//END

  inline void LowPassFilter(float &data, float cuttOffFrequency)
	{
		unsigned long time = micros(); //Stores actual time
		//float tau = 159154.94309/cuttOffFrequency; //in microseconds 1/(2*Pi*Fc)		
		float dt = float(time - lastTime_LPF);
		//ALPHA = dt/(tau + dt);
		float ALPHA = dt/((159154.94309/cuttOffFrequency) + dt);
		data = ALPHA * data + lastDataIn_LPF * (1. - ALPHA);
		lastDataIn_LPF = data;
		lastTime_LPF = time;
	};//END inline void LowPassFilter(float *data, float cuttOffFrequency)
	
	
	//Must be initialized by constructor
	inline void LowPassFilter(float &data)
	{
		unsigned long time = micros(); //Stores actual time
		//float tau = 159154.94309/cuttOffFrequency; //in microseconds 1/(2*Pi*Fc)		
		float dt = float(time - lastTime_LPF);
		//ALPHA = dt/(tau + dt);
		float ALPHA = dt/(tau_LPF + dt);
		data = ALPHA * data + lastDataIn_LPF * (1. - ALPHA);
		lastDataIn_LPF = data;
		lastTime_LPF = time;
	};//END inline void LowPassFilter(float *data, float cuttOffFrequency)
	
	
  inline void HighPassFilter(float &data, float cutOffFrequency)
	{
		float tau = 159154.94309/(cutOffFrequency); //em microsegundos 1/(2*Pi*Fc)
		unsigned long time = micros();
		
		float dt = time - lastTime_HPF;
		float ALPHA = tau/(tau + dt);
		data = ALPHA * (LastDataFiltered_HPF + data - lastDataIn_HPF);
		lastDataIn_HPF = data;
		LastDataFiltered_HPF = data;
		lastTime_HPF = time;
	};//FIM inline void LowPassFilter(float *data, float cutOffFrequency)
  
	//Must be initialized by constructor
	inline void HighPassFilter(float &data)
	{
		//float tau = 159154.94309/(cutOffFrequency); //em microsegundos 1/(2*Pi*Fc)
		unsigned long time = micros();
		
		float dt = time - lastTime_HPF;
		float ALPHA = tau_HPF/(tau_HPF + dt);
		data = ALPHA * (LastDataFiltered_HPF + data - lastDataIn_HPF);
		lastDataIn_HPF = data;
		LastDataFiltered_HPF = data;
		lastTime_HPF = time;
	};//FIM inline void LowPassFilter(float *data, float cuttOffFrequency)
	
	//comming soon
	inline void BandPassFilter(float &data, float cutOffFrequency){}
	
  
	//generates a sine -like signal , which may be composed of several sine waves.
	//Receive vectors as pointers amplitude , phase and frequency
	//static method , best for use at any time , but is not required to be static
	inline static float GenerateSineSignal(unsigned int numberOfwaves, float *amplitude, float *fase, float *frequency)
	{
	  	float outSignal = 0.;
		unsigned long time = micros();
		
		for(register unsigned int i = 0; i<numberOfwaves; i++)
		{
			outSignal = outSignal + amplitude[i] * sin(TWO_PI * frequency[i] * float(time/1e6) + fase[i]);
		};//FIM for
		//Serial.println(1000*float(time/1e6)); //Check output (Optional for DEBUG)
		return outSignal;
	};//END
	
	//Generate a Random Number in this range and divides to return floating point value
	//static method , best for use at any time , but is not required to be static
	static float noise(int minValue = -200, int maxValue = 200, float divisor = 100)
	{
		float noiseFloat;
		noiseFloat = float((random()%(1 + 2*maxValue)) + minValue)/divisor;
		return noiseFloat;
	};//END float noise(int minValue = -200, int maxValue = 200, float divisor = 100)
	
	  //Comming soon?
	  //setCutOffFrequency()
  
private:
	//HIGH PASS FILTER
	float lastDataIn_HPF;
	unsigned long lastTime_HPF;
	float LastDataFiltered_HPF;
	float tau_HPF;
	//LOW PASS FILTER
	float tau_LPF;
	unsigned long lastTime_LPF;       
	float lastDataIn_LPF;
	
};//End class SignalProcessing
