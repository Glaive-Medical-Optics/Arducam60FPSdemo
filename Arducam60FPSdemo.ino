/* 
 *    This demo program shows how to capture frames from the HM01B0 monochrome camera at 60 FPS
 *    from an Arduino Giga R1 board.
 *    Author: Glaive Software Group
 *    Revised: 6/22/2024.
 *    
 *    To run this sketch, you will need to specify that you are using the Arduino Giga R1 board
 *    in the IDE's board manager (under Arduino Mbed OS Giga boards).
 *    You also need to install the arducam_dvp library in the library manager.
 *    This sketch was tested with version 1.0.0 of the arducam_dvp library.
 *    
 *    The arducam_dvp library is configured to run the camera at 30 frames/sec (30 FPS).
 *    If you are happy with that frame rate, then change the FRAME_RATE below from 60 to 30.
 *    If you want to capture frames ar 60 FPS, you will need to make two changes to the arducam_dvp library.
 *    These source files are typically located on your computer
 *    in the folder ~/Documents/Arduino/libraries/arducam_dvp/src.
 *    
 *    [1] In the source file arducam_dvp.cpp, in the ARDUINO_GIGA section,
 *    change
 *        #define DCMI_TIM_FREQUENCY          (6000000)
 *    to
 *        #define DCMI_TIM_FREQUENCY          (12000000)
 *    This causes the clock to run at 12 Mhz instead of 6 MHz.
 *    
 *    [2] in the header file himax.h,
 *    change
 *        uint32_t getClockFrequency() { return 6000000; };  
 *    to
 *        uint32_t getClockFrequency() { return 12000000; };  
 *        
 *    This sketch will capture 10 frames from the camera at QVGA resolution (320 x 240 pixels)
 *    and print out the frame rate it achieved in the Serial Monitor.
 *    
 *    This program is Free Software and has ABSOLUTELY NO WARRANTY.
 */

#include "arducam_dvp.h"
#include "Himax_HM01B0/himax.h"

#define   FRAME_RATE  60

uint32_t timeIt(void (*func)())
{
    uint32_t start = micros();      // Record the start time
    func();                         // Call the function passed as an argument
    uint32_t finish = micros();     // Record the end time

    uint32_t duration = finish - start; // Calculate the duration in microseconds

    Serial.print("Frame grab time: ");
    Serial.print(duration);
    Serial.print(" microseconds");

    return duration;
}

void blinkLED(uint32_t count = 0xFFFFFFFF,uint32_t t_delay = 500)
{
    pinMode(LED_BUILTIN, OUTPUT);
    while (count--)
    {
        digitalWrite(LED_BUILTIN, LOW);  // turn the LED on (HIGH is the voltage level)
        delay(t_delay);                  // wait for a bit
        digitalWrite(LED_BUILTIN, HIGH); // turn the LED off by making the voltage LOW
        delay(t_delay);                  // wait for a bit
    }
}

FrameBuffer fb;

HM01B0 himax;

Camera cam(himax);

void grabFrame()
{
    // Grab a frame and do nothing with it
    if (cam.grabFrame(fb, 3000) != 0)
    {
        Serial.print("grabFrame failed.\n");
        blinkLED(20,100);
    }
 }

void setup()
{
    int i;
    uint32_t durationInMicroseconds;
    double frameRate;

    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(9600);   // register values will be printed in the serial monitor
    while (!Serial)
    {
        ; // Wait for the serial port to connect. Needed for native USB port only
    }
    Serial.print("\nStarting...\n");

    // Init the camera resolution to QVGA resolution at 60 FPS
    if (!cam.begin(CAMERA_R320x240, CAMERA_GRAYSCALE, FRAME_RATE))
    {
      Serial.print("cam.begin failed\n");
      blinkLED();
    }

    uint32_t clockRate = himax.getClockFrequency();
    Serial.print("\nThe clock frequency is ");
    Serial.print(clockRate/1000000);
    Serial.print(" MHz.\n");

    Serial.print("\nTimings:\n\n");

    for(i=1; i<=10; i++)
    {
      durationInMicroseconds = timeIt(grabFrame);
      frameRate = 1000000.0 / durationInMicroseconds;
      Serial.print(",     baud rate = ");
      Serial.print(frameRate);
      Serial.print(" frames/sec\n");
    }    
}

void loop()
{
    
}
