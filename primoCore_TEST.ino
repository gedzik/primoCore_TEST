/*
Moj kod testowy do BLE primo core
*/
#define PULSE 1000 //zalezy od rodzaju miernika pradu
#define POWER 7 // pin zasilajacy wzmacniacz

#include <CoreSensors.h>
#include <BLESerial.h>
//#include <PPI.h> //jakas dziwna biblio od przerwan
#include "ArduinoLowPower.h"
#include <BLEPeripheral.h>
#include <nrf_rtc.h>

double ppwh = ((double)PULSE)/1000;
//volatile byte hmm = LOW;
unsigned long time1=0;
unsigned long time2=0;
double kwhh=0;
int intervaltime=0;
//volatile int zz,yy = 0;
BLESerial bleSerial = BLESerial();
char dat;
void setup() {
	
	//PPI.setTimerInterval(2000);
	//PPI.setShortcut(TIMER, PIN_TOGGLE);
	//PPI.setOutputPin(7);
	

	pinMode(0, INPUT_PULLUP); // Pin od ktorego chce przerwanie
	pinMode(POWER, OUTPUT);
	//pinMode(BLUE_LED, OUTPUT);
	pinMode(LED_BUILTIN, OUTPUT);
	//pinMode(GREEN_LED, OUTPUT);

	//attachInterrupt(digitalPinToInterrupt(0), zbierzdane, CHANGE); //klasyczne przerwanie tu moze byc tez RISING
	//LowPower.attachInterruptWakeup(0, getpulsecount, CHANGE); //przerwanie snu

	
	
	bleSerial.setLocalName("UART");
	bleSerial.begin();
	bleSerial.println("Starting transmission");

	digitalWrite(LED_BUILTIN, HIGH);
	delay(500);
	digitalWrite(LED_BUILTIN, LOW);
	delay(500);
	
	digitalWrite(POWER, LOW);

	//uzywam real time clock, odpalam go w trybie 1 pik = 1 ms i uzywam RTC2 jako ze RTC1 jest zajety przez przerwanie a RTC0 to jakis systemowy
	nrf_rtc_prescaler_set(NRF_RTC2, 32);
	nrf_rtc_task_trigger(NRF_RTC2, NRF_RTC_TASK_START);

	LowPower.attachInterruptWakeup(0, getpulsecount, RISING);
	LowPower.attachInterruptWakeup(RTC_ALARM_WAKEUP, timeralarm, CHANGE);
}


void loop() {


	digitalWrite(POWER, LOW);
	//digitalWrite(GREEN_LED, HIGH);
	LowPower.sleep(10000);
	for (int i = 0; i < 2; i++) {
		LowPower.sleep();
		getpulsecount();
	}
	bleSerial.println(intervaltime);
	
	
	//put the board in low power mode to save power


	
}
/*
void gethightime() {
	if (bleSerial.available())
	{
		dat = bleSerial.read();
		if (dat == 'S')
			digitalWrite(7, HIGH);
		if (dat == 'K') {
			digitalWrite(7, LOW);
		}
	}
		hmm = digitalRead(0);
		intervaltime = time1 - time2;
		time1 = millis();
		if (hmm == HIGH) {
			time2 = time1;
			if (intervaltime != 0)
			{
				bleSerial.println(intervaltime);
				//digitalWrite(LED_BUILTIN, HIGH);
			}
		}
		//digitalWrite(LED_BUILTIN, LOW);
		//digitalWrite(GREEN_LED, hmm);
	
}
*/



/*Przerwanie od pinu ktore sie nie uruchamia ale to nie szkodzi bo wystarczy ze wybudza ze snu*/
void getpulsecount()
{
	time1 = nrf_rtc_counter_get(NRF_RTC2);
	intervaltime = time1 - time2;
	time2 = time1;
	//digitalWrite(GREEN_LED, LOW);
}
/*Przerwanie od timera*/
void timeralarm()
{
	//bleSerial.println(intervaltime);
	digitalWrite(POWER, HIGH);
}
