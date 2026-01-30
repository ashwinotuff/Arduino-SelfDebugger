/*
 ARDUINO SELF‑DEBUGGING EMBEDDED OS FRAMEWORK
Board: Arduino Uno / Nano (ATmega328P)
Clock: 16 MHz
*/

#include <EEPROM.h>
#include <avr/wdt.h>
#include <avr/io.h>

/* 
   CONFIGURATION
 */

#define LED_PIN        13
#define SENSOR_PIN     A0
#define VOLTAGE_PIN    A1

#define SERIAL_BAUD    9600

#define MAX_CRASH_LOGS 5
#define EEPROM_START   0

#define LOOP_MAX_TIME  200
#define SENSOR_TIMEOUT 300
#define LOW_VOLTAGE_ADC 600

/* 
   ERROR CODES
 */

enum ERROR_CODE {
  ERR_NONE = 0,
  ERR_WATCHDOG = 1,
  ERR_LOW_VOLTAGE = 2,
  ERR_SENSOR_TIMEOUT = 3,
  ERR_INVALID_SENSOR = 4,
  ERR_MEMORY_FAULT = 5,
  ERR_EEPROM_FAULT = 6,
  ERR_COMM_FAULT = 7,
  ERR_LOOP_STALL = 8,
  ERR_DEVICE_FAULT = 9
};

/* 
   CRASH LOG STRUCTURE
 */

struct CrashLog {
  byte errorCode;
  byte resetReason;
  unsigned long uptime;
  int sensorValue;
  int batteryADC;
  unsigned int loopTime;
};

CrashLog crash;

/* 
   GLOBAL VARIABLES
 */

unsigned long lastLoopTime;
unsigned long lastSensorUpdate;
unsigned long bootTime;

byte resetFlags;
byte crashIndex = 0;

/* 
   RESET REASON
 */

void readResetReason() {
  resetFlags = MCUSR;
  MCUSR = 0;
}

/* 
   EEPROM LOGGER
 */

int logAddress(byte index) {
  return EEPROM_START + index * sizeof(CrashLog);
}

void saveCrashLog() {
  EEPROM.put(logAddress(crashIndex), crash);
  crashIndex++;
  if (crashIndex >= MAX_CRASH_LOGS) crashIndex = 0;
}

void loadCrashLog(byte index) {
  EEPROM.get(logAddress(index), crash);
}

/* 
   WATCHDOG
 */

void enableWatchdog() {
  wdt_enable(WDTO_2S);
}

/* 
   ERROR HANDLER
 */

void triggerError(byte code) {
  crash.errorCode   = code;
  crash.resetReason = resetFlags;
  crash.uptime      = millis() / 1000;
  crash.sensorValue = analogRead(SENSOR_PIN);
  crash.batteryADC  = analogRead(VOLTAGE_PIN);
  crash.loopTime    = millis() - lastLoopTime;

  saveCrashLog();

  delay(50);
  wdt_enable(WDTO_15MS);   // forced reset
  while (1);
}

/* 
   DIAGNOSTIC PRINTING
 */

void printResetReason(byte r) {
  if (r & _BV(WDRF)) Serial.println(F("WATCHDOG"));
  else if (r & _BV(BORF)) Serial.println(F("BROWN‑OUT"));
  else if (r & _BV(PORF)) Serial.println(F("POWER‑ON"));
  else if (r & _BV(EXTRF)) Serial.println(F("EXTERNAL"));
  else Serial.println(F("UNKNOWN"));
}

void printCrash(byte index) {
  loadCrashLog(index);

  Serial.println(F("--------------------------------"));
  Serial.print(F("LOG #")); Serial.println(index);

  Serial.print(F("Reset: "));
  printResetReason(crash.resetReason);

  Serial.print(F("Error: "));
  Serial.println(crash.errorCode);

  Serial.print(F("Uptime: "));
  Serial.print(crash.uptime);
  Serial.println(F(" sec"));

  Serial.print(F("Sensor: "));
  Serial.println(crash.sensorValue);

  Serial.print(F("Battery ADC: "));
  Serial.println(crash.batteryADC);

  Serial.print(F("Loop time: "));
  Serial.print(crash.loopTime);
  Serial.println(F(" ms"));
}

/* 
   SENSOR MONITOR
 */

void sensorTask() {
  int v = analogRead(SENSOR_PIN);

  if (v < 0 || v > 1023)
    triggerError(ERR_INVALID_SENSOR);

  lastSensorUpdate = millis();
}

/* 
   VOLTAGE MONITOR
 */

void voltageTask() {
  int v = analogRead(VOLTAGE_PIN);

  if (v < LOW_VOLTAGE_ADC)
    triggerError(ERR_LOW_VOLTAGE);
}

/* 
   TIMING MONITOR
 */

void timingTask() {
  if (millis() - lastSensorUpdate > SENSOR_TIMEOUT)
    triggerError(ERR_SENSOR_TIMEOUT);

  if (millis() - lastLoopTime > LOOP_MAX_TIME)
    triggerError(ERR_LOOP_STALL);
}

/* 
   TASK SCHEDULER (RTOS STYLE)
*/

void runScheduler() {
  sensorTask();
  voltageTask();
  timingTask();
}

/* 
   SETUP
*/

void setup() {
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(SERIAL_BAUD);
  delay(500);

  readResetReason();

  Serial.println(F("\n===== EMBEDDED DIAGNOSTIC OS ====="));

  for (byte i = 0; i < MAX_CRASH_LOGS; i++) {
    printCrash(i);
  }

  bootTime = millis();

  lastSensorUpdate = millis();
  lastLoopTime = millis();

  enableWatchdog();

  Serial.println(F("System initialized successfully."));
}

/*
   MAIN LOOP */

void loop() {
  wdt_reset();

  lastLoopTime = millis();

  runScheduler();

  // Heartbeat
  digitalWrite(LED_PIN, HIGH);
  delay(20);
  digitalWrite(LED_PIN, LOW);

  delay(50);
}

