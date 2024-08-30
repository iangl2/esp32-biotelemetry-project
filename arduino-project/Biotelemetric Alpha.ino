

#include <BluetoothSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MLX90614.h>
#define WIRE Wire


//Ritmo cardiaco
float factor = 0.75;		// coeficiente para filtro pasa bajos
float maximo = 0.0;		// para almacenar valor maximo 
int minimoEntreLatidos = 300;	// 300 mseg. de tiempo minimo entre latidos
float valorAnterior = 500;	// para almacenar valor previo
int latidos = 0;		// contador de cantidad de latidos

float tp, th,count=1,bpm;
BluetoothSerial SerialBT;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32,&WIRE);


static const unsigned char PROGMEM logoBmp[] = 
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x07, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x01, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x1e, 0x0f, 0x80, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x38, 0x3c, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x70, 0xf0, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0xe3, 0xc1, 0xff, 0xff, 0x80, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x61, 0x87, 0x0f, 0xfc, 0x07, 0xf0, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x41, 0x86, 0x3f, 0xc0, 0x00, 0x7e, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x41, 0x04, 0x7f, 0x80, 0x00, 0x0f, 0xf8, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
	0xfe, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 
	0x01, 0x80, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x01, 0x80, 
	0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x03, 0x00, 0x30, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x02, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x7e, 0x00, 0x00, 0x00, 0x02, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 
	0x00, 0x00, 0x03, 0x1e, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x03, 
	0x1e, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x03, 0x1e, 0x30, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x00, 0x03, 0x1e, 0x30, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0xfc, 0x00, 0x00, 0x02, 0x1e, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x7f, 0x00, 0x00, 0x0c, 0x3f, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 
	0x08, 0x7f, 0x8c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x0c, 0x3f, 0x0c, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x06, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf8, 0x30, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


void setup() { 
   display.begin(SSD1306_SWITCHCAPVCC, 0x3C);//inicio lcd
  mlx.begin(); //Inicia sensor de temperatura
 Serial.begin(9600);//velocidad de comunicación
  SerialBT.begin("ESP32");//Inicia bluetooth y le pone un nombre
   display.clearDisplay();//limpio display
  display.drawBitmap(10,0,logoBmp,103,32,WHITE);
  display.display();
  delay(2000);
  }
void loop() {
  if(count>10){
    display.clearDisplay();//limpio display
//Hago lecturas
 th=mlx.readAmbientTempC();
  tp=mlx.readObjectTempC();
  //impresión en display
  display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("BioTelemetric Alpha");;
  display.print("Paciente: ");display.println(tp);
  display.print("Habitacion: ");display.println(th);
  display.print("Pulso: "); display.print(bpm);
  display.display(); 
  //impresión en bluetooth
 SerialBT.print("P");SerialBT.println(tp);
   SerialBT.print("H");SerialBT.println(th);
    SerialBT.print("B");SerialBT.println(bpm);
   count=1;
  }
    static unsigned long tiempoLPM = millis();	
  static unsigned long entreLatidos = millis(); 
  int valorLeido = analogRead(34);	
  float valorFiltrado = factor * valorAnterior + (1 - factor) * valorLeido;	
  float cambio = valorFiltrado - valorAnterior;		
  valorAnterior = valorFiltrado;		
  if ((cambio >= maximo) && (millis() > entreLatidos + minimoEntreLatidos)) {		
    maximo = cambio;		
    entreLatidos = millis();	
    latidos++;				
  }
  maximo = maximo * 0.97;	
  if (millis() >= tiempoLPM + 15000) {	
    bpm=latidos*4;	
    latidos = 0;
    tiempoLPM = millis();		
  }
  count=count+1;
  Serial.println(analogRead(34));
delay(49);
}


