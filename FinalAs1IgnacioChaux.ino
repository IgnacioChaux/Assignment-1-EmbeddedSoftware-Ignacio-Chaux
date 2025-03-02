int GreenLED = 16;//initialize inputs and outputs
int RedLED = 17;
int SetupButton = 26;
int ModeButton = 27;
int YellowLED = 23;
//define some state values
bool OnState = false;
bool AltState = false;
long Ton;

//setup the inputs and outputs 
void setup() {
    pinMode(SetupButton, INPUT);
    pinMode(ModeButton, INPUT);
    pinMode(GreenLED, OUTPUT);
    pinMode(RedLED, OUTPUT);
    pinMode(YellowLED, OUTPUT);
}
//function to check if the button is on 
bool debounceButton(int buttonPin) {
    static bool lastButtonState = HIGH;
    static unsigned long lastDebounceTime = 0;
    const unsigned long debounceDelay = 50;
    //check if the button pressed is low and previously high
    bool buttonState = digitalRead(buttonPin);
    if (buttonState == LOW && lastButtonState == HIGH) {
        if (millis() - lastDebounceTime > debounceDelay) {
            lastDebounceTime = millis();
            lastButtonState = buttonState;
            return true;
        }
    }
    lastButtonState = buttonState;
    return false;
}


void loop() {
    static bool lastSetupButtonState = HIGH;
    bool setupButtonState = digitalRead(SetupButton);
//Toggle on state when button is pressed
    if (setupButtonState == LOW && lastSetupButtonState == HIGH) {
        delay(50);
        if (digitalRead(SetupButton) == LOW) {
            OnState = !OnState;
            Serial.print("Arduino is now ");
            Serial.println(OnState ? "ON" : "OFF");
        }
    }
    lastSetupButtonState = setupButtonState;//update button 
//if arduino is off then turn off all the LEDs
    if (!OnState) {
        digitalWrite(GreenLED, LOW);
        digitalWrite(RedLED, LOW);
        digitalWrite(YellowLED, LOW);
        return;
    }
//toggle alternate mode 
    if (debounceButton(ModeButton)) {
        AltState = !AltState;
        digitalWrite(YellowLED, AltState ? HIGH : LOW);
        Serial.print("Alternate Mode is now ");
        Serial.println(AltState ? "ON" : "OFF");
    }
//initialize the calcualted values from the name CHAUX
    int arr[] = {3,8,1,6};
    int a1 = arr[0] * 100;
    int b1 = arr[1] * 100;
    int c1 = arr[2] + 4;
    int d1 = arr[3] * 500;
//Tsync
    digitalWrite(RedLED, HIGH);
    delayMicroseconds(50);
    digitalWrite(RedLED, LOW);
//Alternate state
    if (AltState) {
       digitalWrite(GreenLED, HIGH);
       delayMicroseconds(a1);
       Serial.println(a1);
       digitalWrite(GreenLED, LOW);
       delayMicroseconds(b1);
       Serial.println(b1);
       c1 = c1 - 3;
       for (int i = 2; i <= c1; i++) {
        Ton = a1 + ((i - 1) * 50);
        digitalWrite(GreenLED, HIGH);
        delayMicroseconds(Ton);
        Serial.println(Ton);
        digitalWrite(GreenLED, LOW);
        delayMicroseconds(b1);
        Serial.println(b1);
      }
    } 
    //normal state
    else {
      digitalWrite(GreenLED, HIGH);
      delayMicroseconds(a1);
      Serial.println(a1);
      digitalWrite(GreenLED, LOW);
      delayMicroseconds(b1);
      Serial.println(b1);
      for (int i = 2; i <= c1; i++) {
        Ton = a1 + ((i - 1) * 50);
        digitalWrite(GreenLED, HIGH);
        delayMicroseconds(Ton);
        Serial.println(Ton);
        digitalWrite(GreenLED, LOW);
        delayMicroseconds(b1);
        Serial.println(b1);
      }
    }
    //final delay
    digitalWrite(GreenLED, LOW);
    delayMicroseconds(d1);
    Serial.println(d1);
}
