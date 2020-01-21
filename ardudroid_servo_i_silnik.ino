/*
 PROJECT: ArduDroid
 PROGRAMMER: Hazim Bitar (techbitar at gmail dot com)
 DATE: Oct 31, 2013
 FILE: ardudroid.ino
 LICENSE: Public domain
*/

#define START_CMD_CHAR '*'
#define END_CMD_CHAR '#'
#define DIV_CMD_CHAR '|'
#define CMD_DIGITALWRITE 10
#define CMD_ANALOGWRITE 11
#define CMD_TEXT 12
#define CMD_READ_ARDUDROID 13
#define MAX_COMMAND 20  // max command number code. used for error checking.
#define MIN_COMMAND 10  // minimum command number code. used for error checking. 
#define IN_STRING_LENGHT 40
#define MAX_ANALOGWRITE 255
#define PIN_HIGH 3
#define PIN_LOW 2
#define TRIGGER_PIN  2  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     13  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define STOP_DIST 20

#include <Servo.h>
#include <NewPing.h>
#include <AFMotor.h>
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
Servo myservo;
int pos = 0;
int posstart = 90;

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

String inText;

void motorlewyprzod()
{
  motor1.run(FORWARD);
  motor2.run(FORWARD);
}
void motorlewytyl()
{
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
}

void motorprawyprzod()
{
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}
void motorprawytyl()
{
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

int lewypredkosc(int predkosc)
{
  motor1.setSpeed(predkosc);
  motor2.setSpeed(predkosc);
}
int prawypredkosc(int predkosc)
{
  motor3.setSpeed(predkosc);
  motor4.setSpeed(predkosc);
}

void motorlewystop ()
{
  motor1.run(RELEASE);
  motor2.run(RELEASE);
}

void motorprawystop ()
{
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}
int test;
int freetogo() {

  //delay(1000);
  //int pomiar_odl = sonar.ping_cm();
  if ((sonar.ping_cm() <= STOP_DIST) && (sonar.ping_cm() != 0)) {
    Serial.println(sonar.ping_cm());
    Serial.println(" przeszkoda");
    return 0;
    // test = 1;
  }
  else
    return 1;
  //test = 0;
}

void setup() {
  Serial.begin(19200);
  Serial.println("ArduDroid 0.12 Alpha by TechBitar (2013)");
  Serial.flush();
  myservo.attach(9);

  //lewypredkosc(100);
  //motorlewyprzod();
 // prawypredkosc(100);
  //motorprawyprzod();
  myservo.write(posstart);
  delay(200);



}
int sv;
bool do_prog;
void loop() {
  //delay(100);
  // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  if ( Serial.available() )      // if data is available to read
  {
    sv = Serial.read();    // read it and store it in 'val'
    switch (sv) {
      case 'p':
        motorlewystop ();
        motorprawystop ();
        do_prog = false;
        break;
      case 'o':
        lewypredkosc(100);
        motorlewyprzod();
        prawypredkosc(100);
        motorprawyprzod();
        myservo.write(posstart);
        do_prog = true;
        break;
      case 'w':
        lewypredkosc(100);
        motorlewyprzod();
        prawypredkosc(100);
        motorprawyprzod();
        break;
      case's':
        lewypredkosc(100);
        motorlewytyl();
        prawypredkosc(100);
        motorprawytyl();
        break;
      case'a':
        prawypredkosc(100);
        motorprawyprzod();
        lewypredkosc(100);
        motorlewytyl();
        delay(1500);
        motorlewystop ();
        motorprawystop ();
        break;
        case'd':
        prawypredkosc(100);
        motorprawytyl();
        lewypredkosc(100);
        motorlewyprzod();
        delay(1500);
        motorlewystop ();
        motorprawystop ();
        break;


    }

  }
  if (do_prog) {
    if (freetogo()) {
      Serial.println("jade prosto");
      // motorlewystop ();
      // motorprawystop ();
      // delay(2000);
    }

    else {
      motorlewystop ();
      motorprawystop ();
      myservo.write(0);
      delay(500);

      if (freetogo()) { //czy mozna jechac w prawo
        Serial.println ("mozna w prawo");
        lewypredkosc(100);
        motorlewyprzod();
        prawypredkosc(100);
        motorprawytyl();

        delay(1500); // czas obrotu

        myservo.write(90);
        lewypredkosc(100);
        motorlewyprzod();
        prawypredkosc(100);
        motorprawyprzod();
        Serial.println ("skrecilem w prawo i jade");


      }
      else {
        myservo.write(180);
        delay(750);
        if (freetogo()) { //czy mozna jechac w lewo
          Serial.println ("mozna w lewo");
          prawypredkosc(100);
          motorprawyprzod();
          lewypredkosc(100);
          motorlewytyl();

          delay(1500); // czas obrotu

          myservo.write(90);
          lewypredkosc(100);
          motorlewyprzod();
          prawypredkosc(100);
          motorprawyprzod();
          Serial.println ("skrecilem w lewo i jade");
        }
        else {
          myservo.write(90);
          delay(750);
          motorlewystop ();
          motorprawystop ();

        }


      }



    }
    //    Serial.print("Ping: ");
    //Serial.print(sonar.ping_cm()); // Send ping, get distance in cm and print result (0 = outside set distance range)
    //  Serial.println("cm");
    //myservo.write(90);
    // delay(1500);
  }
}







