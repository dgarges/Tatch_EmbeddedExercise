
// Tatch Fun Embedded Excersize 
// Author: David C Garges
// Date: July 8th, 2018
// Summary: The following draft of the Stopwatch class has been created to take advantage of the Ticker and Timer classes.
//			The available Timer API has a rollover of only 30min due to its return value type which is a short.
//			The Stopwatch class will allow a time memory of up to 50 days a unsigned long data type return.
//			This is accomplished by interupting the Timer before rollover and accumulating the result before reset.
//			The Stopwatch class also attempts to protect against multi-thread access so that it can be utilized consistently.
//			For this challenge, I assumed that the start of run time is equivalent to the creation of a Stopwatch object. 

#include <iostream>
using namespace std;

// Available API:

class Timer
{
	void start();				// start the timer
	void stop();				// stop the timer
	void reset(); 				// reset the timer

	unsigned short read_s();	// number of seconds since start
	unsigned short read_ms();	// number of milliseconds since start
	int read_us();				// number of microseconds since start	
}

class Ticker
{
	// attach a function, specifying interval in seconds
	attach(Callback< void()> func, float t);	

	// attach a member function, specifying interval in seconds		
	attach( *obj, M method, float t);

	// attach a function, specifying interval in microseconds
	attach_us(Callback< void()> func, us_timestamp_t t);

	// attach a member function, specifying interval in microseconds
	attach_us(T *obj, M method, us_timestamp_t t);

	// detach
	void detach();
}

/////////////////////////////////////////////////////////

class Stopwatch
{
	private:
		const float 29_MINUTES = 29*60*1000; 
		unsigned long current_millis = 0;
		unsigned int current_micros = 0;
		Timer timer;
		Ticker ticker;
		int running = 0;

		void update_current_time();
		{
			if (running) 
			{
				return;
			}
			running = true;


			// errorTimer is a secondary timer that allows safe sequential read of millis/micros without losing time
			Timer errorTimer;
			errorTimer.start();
			timer.stop();
			
			current_micros += timer.read_us();
			unsigned long temp = (long) timer.read_ms(); //convert short to long
			current_millis += temp;
			
			timer.reset();
			errorTimer.stop();
			timer.start();
			current_micros += errorTimer.read_us(); 
			temp = (long) errorTimer.read_ms();			//convert short to long
			current_millis += temp; 
		}

	public: 
		Stopwatch() 
		{
			timer.start();
			ticker.attach(this, &update_current_time, 29_MINUTES);
		}

		~Stopwatch()
		{
			ticker.detach();
		}

		unsigned long millis() 
		{
			update_current_time();
			return current_millis;
		}

		unsigned int micros() 
		{
			update_current_time();
			return current_micros;
		}
};


int main()
{
	Stopwatch stopwatch;
	//wait some time

	cout << "It has been " << stopwatch.millis() << "ms and " << stopwatch.micros() << "us since beginning of runtime." << endl;
}



