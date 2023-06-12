//Angel Farina 1F UTN

//BIBLIOTECAS
#include <IRremote.h>
#include <LiquidCrystal.h>
#include <Servo.h>

//DEFINES
#define Tecla_1 0xEF10BF00
#define Tecla_2 0xEE11BF00
#define Tecla_3 0xED12BF00
#define Tecla_4 0xEB14BF00

#define IR 11
#define LED_RED 12
#define LED_GREEN 13
#define servoPin 10

//VARIABLES
int temperaturaCriticaPrimavera = 40;
int temperaturaCriticaVerano = 45;
int temperaturaCriticaOtonio = 35;
int temperaturaCriticaInvierno = 30;

//LCD
LiquidCrystal lcd(4, 5, 6, 7, 8, 9);

//SERVO
Servo myservo;
bool servoActivado = false;
bool inicializacionCompleta = false;

//VARIABLES DE ESTACION
int estacionSeleccionada = 0;
int temperaturaCritica = 0;

//SETUP
void setup() {
  pinMode(A0, INPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  Serial.begin(9600);
  IrReceiver.begin(IR, DISABLE_LED_FEEDBACK);
  myservo.attach(servoPin);

  lcd.begin(16, 2);
  lcd.print("Encendiendo...");
  delay(1000);
  lcd.clear();
  lcd.print("En espera");
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);
  delay(1000);

  inicializacionCompleta = true;
  myservo.write(90);
}

//FUNCION SENSOR
void sensorTemp(int temp) {
  if (inicializacionCompleta && estacionSeleccionada != 0) {
    if (temp >= temperaturaCritica) {
      if (!servoActivado) {
        servoActivado = true;
        lcd.clear();
        lcd.print("Alarma activada");
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_RED, HIGH);
        myservo.write(180);
        delay(1000);
      }
    } else {
      if (servoActivado) {
        servoActivado = false;
        lcd.clear();
        lcd.print("Temperatura");
        lcd.setCursor(0, 1);
        lcd.print("Normal");
        digitalWrite(LED_RED, LOW);
        digitalWrite(LED_GREEN, HIGH);
        myservo.write(90);
        delay(1000);
      }
    }
  }

  lcd.setCursor(0, 0);
  //lcd.print("Estacion: ");
  //lcd.print(estacionSeleccionada);
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print("C");
}

//FUNCION BOTONES
void botonPresionado(unsigned long boton) {//unsigned long para almacenar valores grandes y no negativos de los codigos de boton IR
  if (boton == Tecla_1) {
    estacionSeleccionada = 1;
    temperaturaCritica = temperaturaCriticaInvierno;
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    lcd.clear();
    lcd.print("Invierno");
  } else if (boton == Tecla_2) {
    estacionSeleccionada = 2;
    temperaturaCritica = temperaturaCriticaVerano;
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    lcd.clear();
    lcd.print("Verano");
  } else if (boton == Tecla_3) {
    estacionSeleccionada = 3;
    temperaturaCritica = temperaturaCriticaOtonio;
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    lcd.clear();
    lcd.print("Otonio");
  } else if (boton == Tecla_4) {
    estacionSeleccionada = 4;
    temperaturaCritica = temperaturaCriticaPrimavera;
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    lcd.clear();
    lcd.print("Primavera");
  }
}

//LOOP
void loop() {
  int temp = analogRead(A0);
  int tempMapeada = map(temp, 20, 358, -40, 125);

  if (inicializacionCompleta && IrReceiver.decode()) { //verifico si la inicialización esta completa y si se presiono un boton
    unsigned long boton = IrReceiver.decodedIRData.decodedRawData;
    Serial.println(boton, HEX);

    botonPresionado(boton);

    IrReceiver.resume();
  }

  sensorTemp(tempMapeada);
}