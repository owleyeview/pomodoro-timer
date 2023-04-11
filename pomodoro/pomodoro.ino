/*
Rashaan Lightpool
3/16/2023
SD247
Raspberry Pi Pico Pomodoro Timer
pomodoro.ino
*/

/*
This Raspberry Pi project uses an LED bar to represent the percentage of time
remaining in the current time period.  A button is used to start the timer and
can also be used to stop a time period early or reset the timer.  When the
timer is started, all the LEDs light and slowly go out over a 25 minute study
period.  When the study period is over a series of tones play on a passive
buzzer to alert the user that the time has run out.  After the tones play, the
LEDs all light again and go out over a 5 minute break period.  At the end of the
break, tones play again and the timer waits for another button press to start
the study period timer again.
*/

// Define pins for button and buzzer
#define PIN_BUTTON 15
#define PIN_BUZZER 14

// Define pins for the LED bar
const byte ledPins[] = {16, 17, 18, 19, 20, 21, 22, 26, 27, 28};

int ledCount;

// Define constants for the study and break timers
const int STUDY_TIME = 25 * 60; // 25 minutes in seconds
const int BREAK_TIME = 5 * 60;  // 5 minutes in seconds

// Define constants for the button
const int BUTTON_DEBOUNCE_DELAY = 30; // 30 ms

// State variables
bool isRunning = false;

void setup()
{
    // Set the button pin to input mode
    pinMode(PIN_BUTTON, INPUT);
    // Set the buzzer pin to output mode
    pinMode(PIN_BUZZER, OUTPUT);
    ledCount = sizeof(ledPins);
    // Set the LED bar pins to output mode
    for (int i = 0; i < ledCount; i++)
    {
        pinMode(ledPins[i], OUTPUT);
    }
}

void loop()
{
    delay(1000); // Wait one second for button release
    if (!isRunning)
    {
        turnOffLEDs();
    }
    // Check if the button is pressed
    if (digitalRead(PIN_BUTTON) == LOW)
    {
        // Button is pressed, wait for the debounce delay
        delay(BUTTON_DEBOUNCE_DELAY);
        // Check if the button is still pressed
        if (digitalRead(PIN_BUTTON) == LOW)
        {
            // Button is still pressed, start the timer
            while (digitalRead(PIN_BUTTON) == LOW)
                ; // Wait for the button to be released
            isRunning = true;
            pomodoroTimer();
        }
    }
}

void pomodoroTimer()
{
    // Start the study timer
    studyTimer();
    // Start the break timer
    breakTimer();
}

void studyTimer()
{
    // Turn on all LEDs at the start of the timer
    turnOnLEDs();
    int nextLED = ledCount - 1;
    // Start the timer for STUDY_TIME seconds
    for (int i = 0; i < STUDY_TIME; i++)
    {
        // Check if the button is pressed
        if (digitalRead(PIN_BUTTON) == LOW)
        {
            // Button is pressed, wait for the debounce delay
            delay(BUTTON_DEBOUNCE_DELAY);
            // Check if the button is still pressed
            if (digitalRead(PIN_BUTTON) == LOW)
            {
                // Button is still pressed, stop the timer
                isRunning = false;
                // Wait for the button to be released before returning
                while (digitalRead(PIN_BUTTON) == LOW)
                    ;

                return;
            }
        }
        if ((i + 1) % (STUDY_TIME / ledCount) == 0)
        {
            // Turn off one LED at a time
            digitalWrite(ledPins[nextLED], LOW);
            nextLED--;
        }
        // Wait for 1 second
        delay(1000);
    }
    freedomAlert();
}

void breakTimer()
{
    // Turn on all LEDs at the start of the timer
    turnOnLEDs();
    int nextLED = ledCount - 1;
    // Start the timer for BREAK_TIME seconds
    for (int i = 0; i < BREAK_TIME; i++)
    {
        // Check if the button is pressed
        if (digitalRead(PIN_BUTTON) == LOW)
        {
            // Button is pressed, wait for the debounce delay
            delay(BUTTON_DEBOUNCE_DELAY);
            // Check if the button is still pressed
            if (digitalRead(PIN_BUTTON) == LOW)
            {
                // Button is still pressed, stop the timer
                isRunning = false;
                // Wait for the button to be released before returning
                while (digitalRead(PIN_BUTTON) == LOW)
                    ;
                return;
            }
        }
        if ((i + 1) % (BREAK_TIME / ledCount) == 0)
        {
            // Turn off one LED at a time
            digitalWrite(ledPins[nextLED], LOW);
            nextLED--;
        }
        // Wait for 1 second
        delay(1000);
    }
    imperialMarchAlert();
    // comeBackAlert();
    isRunning = false;
}

// turn on all LEDs at the start of each countdown
void turnOnLEDs()
{
    for (int i = 0; i < ledCount; i++)
    {
        digitalWrite(ledPins[i], HIGH);
    }
}

void turnOffLEDs()
{
    for (int i = 0; i < ledCount; i++)
    {
        digitalWrite(ledPins[i], LOW);
    }
}

void playTones(int frequency, int duration)
{
    // Play a tone with the given frequency and duration
    tone(PIN_BUZZER, frequency, duration);
    // Wait for the tone to finish playing
    delay(duration + 25);
    noTone(PIN_BUZZER);
}

void freedomAlert()
{
    // Play three descending tones with a duration of 150ms each
    int freedomFrequencies[] = {262, 330, 392, 523};
    int freedomDurations[] = {500, 500, 500, 1000};
    int alertLength = 4;
    for (int i = 0; i < 4; i++)
    {
        playTones(freedomFrequencies[i], freedomDurations[i]);
    }
}

void imperialMarchAlert()
{
    int marchFrequencies[] = {392, 392, 392, 311, 466, 392, 311, 466, 392};
    int marchDurations[] = {500, 500, 500, 300, 200, 500, 300, 200, 1000};
    int marchLength = 9;
    for (int i = 0; i < marchLength; i++)
    {
        playTones(marchFrequencies[i], marchDurations[i]);
    }
}