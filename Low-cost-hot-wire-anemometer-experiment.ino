//PIN SETTINGS----------------------------------------------------------------------------------------
#define fan 10          //output to control the fan speed
#define scale 11        //output scale to control the voltage
#define current A1      //measured current
//END OF PIN SETTINGS---------------------------------------------------------------------------------

//INTERNAL VARIABLES----------------------------------------------------------------------------------
int f;                  //output to control the fan speed
int x;                  //output scale to control the voltage
float V;                //calculated voltage
float I;                //measured current
float R;                //calculated resistance 
float target;           //target resistance
float Kp;               //P coefficient
float Ki;               //I coefficient
float Kd;               //D coefficient
float error_previous;   //previous error
float error;            //current error
float error_int;        //integrated error
float error_diff;       //differentiated error
float time_current;     //time for current measurement
float time_previous;    //time for previous measurement
//INTERNAL VARIABLES----------------------------------------------------------------------------------

void setup()
{
  Serial.begin(9600);                               //initializes the serial connection at 9600 bits per second (default for Arduino)
  pinMode(fan, OUTPUT);                             //set voltage scale as output
  pinMode(scale, OUTPUT);                           //set voltage scale as output
  pinMode(current, INPUT);                          //set current measurement as input
  
  //User settings----------------------------------------------------------------------------------
  f = 10;                                           //set the fan setting in percent
  x = 100;                                          //set initial scale to 100 (out of 255)
  //User settings----------------------------------------------------------------------------------
  
  f = map(f, 0, 100, 0, 255);                       //map fan setting from percent to usable value (0 to 255)
  analogWrite(fan, f);                              //output the fan setting to control the fan speed
  Kp = 2;                                           //set P coefficient (source: http://www2.metso.com/MetsoTutor.aspx)
  Ki = 0.2;                                         //set I coefficient (source: http://www2.metso.com/MetsoTutor.aspx)
  Kd = 0.1;                                         //set D coefficient (source: http://www2.metso.com/MetsoTutor.aspx)
  error_int = 0;                                    //set initial integrated error to 0
  analogWrite(scale, x);                            //output the scale to control the voltage (limited to 0-255)
  V = 13.8 * x / 255;                               //calculate the voltage
  delay(5000);                                      //delay 5 seconds for system to stabilize (calibration)    
  I = analogRead(current);                          //read current value (from 0A to 5A)                  
  target = V / I;                                   //calculate the initial resistance
  time_current = millis();                          //set the time for the current measurement
  error = 0;                                        //set the initial error to 0
}

void loop()
{  
  analogWrite(scale, x)                             //output the scale to control the voltage (limited to 0-255)
  V = 13.8 * x / 255;                               //calculate the voltage
  delay(1000);                                      //delay 1 second    
  I = analogRead(current);                          //read current value (from 0A to 5A) 
  R = V / I;                                        //calculate the resistance
  
  //PID control----------------------------------------------------------------------------------
  error_previous = error;                           //set the previous error
  error = target - R;                               //calculate the current error
  error_change = error - error_previous             //calculate the change in error
  time_previous = time_current;                     //set the time for the current measurement
  time_current = millis();                          //set the time for the current measurement
  time_change = time_current - time_previous        //calculate the change in time
  error_int = error_int + error;                    //calculate the integrated error
  error_diff = error / time_change;                 //calculate the differentiated error
  x = Kp * error + Ki * error_int + Kd * error_diff //vary the scale using PID control
  //PID control----------------------------------------------------------------------------------
  
  Serial.print("Voltage: ");                        //print voltage value in serial monitor
  Serial.print(V);                                  
  Serial.print("     Current: ");                   //print current value in serial monitor
  Serial.print(I);                                  
  Serial.print("     Resistance: ");                //print resistance value in serial monitor
  Serial.println(R);
}
