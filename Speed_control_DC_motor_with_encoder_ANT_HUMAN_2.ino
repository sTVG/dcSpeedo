// period of pulse accumulation and serial output, milliseconds
#define MainPeriod 100
long previousMillis1 = 0; // will store last time of the cycle end
volatile unsigned long duration=0; // accumulates pulse width
volatile unsigned int pulsecount=0;
volatile unsigned long previousMicros=0;



const int spinSpeedPin=3; //Blue wire for speed control using PWM
const int spinDirectionPin=4; // Yellow wire for Direction control
//const int speedoPin=2; //Green wire for encoder - 4 pulses per revolution of motor 114 pulses per rev of gearbox. using interrupt pin 0 = GPIO2
const int brakePin=5; //White wire for brake or E-stop
const int buttonPin=8; //Start
const int ledPin=13; //LED output pin
//emergency stop - hard wired to ground

int RPM;
int spinSpeed;
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
int counter;
long milli2;

#define spinupTime 1000
#define spinTime 2000
#define spindownTime 3000
long previousMillis2 = 0; // will store last time of the cycle end
long previousMillis3 = 0; // will store last time of the cycle end




void setup()
{
  
   pinMode(brakePin, OUTPUT);
 pinMode(spinSpeedPin, OUTPUT);
 pinMode(spinDirectionPin, OUTPUT);
 pinMode(buttonPin, INPUT);
 pinMode(ledPin, OUTPUT);
  Serial.begin(19200); 
  attachInterrupt(0, myinthandler, RISING);
digitalWrite(spinSpeedPin, HIGH);
digitalWrite(buttonPin, LOW);
digitalWrite(ledPin, LOW);

}

void loop()
{
  digitalWrite(brakePin,HIGH); // LOW = Brake, High = no brake
digitalWrite(spinDirectionPin, LOW); // LOW = CW, HIGH = CCW



  

 unsigned long currentMillis = millis();
  if (currentMillis - previousMillis1 >= MainPeriod) 
  {
    previousMillis1 = currentMillis;   
    // need to bufferize to avoid glitches
    unsigned long _duration = duration;
    unsigned long _pulsecount = pulsecount; //how many pulses per MainPeriod (100 millis)
    float secs = MainPeriod*0.001;
    float  revs = secs*2 ; // measured pulses per gearbox revolution to calculate RPM every 100 millis
    unsigned long rpm = _pulsecount/revs;
    duration = 0; // clear counters
    pulsecount = 0;
    float Freq = 1e6 / float(_duration);
    Freq *= _pulsecount; // calculate F
    // output time and frequency data to RS232
      
    Serial.print(secs);
    Serial.print(" seconds ,");
    Serial.print(currentMillis);
    Serial.print(" counter ,");
    Serial.print(_pulsecount);
    Serial.print(" ");
    Serial.print("pulses");
    Serial.print(" , ");
    Serial.print(buttonState);
    Serial.print(" button , ");
    Serial.print(buttonState);
    Serial.print(" button , ");
    Serial.print(rpm);
    Serial.println("rpm");
    
    
  RPM = rpm;
  }
  

    
       motorCycle();

 


}

void motorCycle() //this motor works with negative PWM? 255 = stop 0=full speed for some reason?
{

/*
 * push button to start
 * spin up for 15 seconds to 240 rpm
 * hold for 120 seconds 
 * spin down for 15 seconds
*/
 unsigned long currentMillis = millis();
 milli2 = currentMillis - previousMillis2 ;
  unsigned long speedee ;
  int map1 ;
  int map2 ;
  int pwm1 ;
  int pwm2 ;

if(milli2 < spindownTime){   

if (  milli2 < spinupTime) 
{ 
    map1=0;
    map2=spinupTime;
    pwm1=255;
    pwm2=0;
speedee = map(milli2,map1,map2,pwm1,pwm2);
  analogWrite(spinSpeedPin,speedee);
  digitalWrite(ledPin, HIGH);
}
else if( spinupTime <= milli2 && milli2  < spinTime)
 {

    map1=spinupTime;
    map2=spinTime;
    pwm1=0;
    pwm2=0;
 speedee = map(milli2,map1,map2,pwm1,pwm2);
  analogWrite(spinSpeedPin,speedee);
  digitalWrite(ledPin, HIGH);
 }
 
else if( spinTime <= milli2 && milli2 < spindownTime)
 {
    map1=spinTime;
    map2=spindownTime;
    pwm1=0;
    pwm2=255;
speedee = map(milli2,map1,map2,pwm1,pwm2);
  analogWrite(spinSpeedPin,speedee);
  digitalWrite(ledPin, HIGH);
  }

 if(milli2 >= spindownTime)
 {
digitalWrite(ledPin, LOW);
      
//previousMillis2 = currentMillis;
 }
 else {
 
 }
 // Serial.println(milli2);
}

}

void myinthandler() // interrupt handler
{
  unsigned long currentMicros = micros();
  duration += currentMicros - previousMicros;
  previousMicros = currentMicros;
  pulsecount++;
}



