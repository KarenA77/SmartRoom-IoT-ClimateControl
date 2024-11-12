// importar estas librerías al IDE de Arduino
#include <LiquidCrystal_I2C.h> // Uso de LDC con el módulo I2C
#include <Wire.h>              // I2C 
#include <DHT.h>	             // Sensor de temperatura y Humedad
#include <MQ135.h>             // Librería para el sensor de gas
#include <Servo.h>             // Uso del Servomotor

// Objeto del LCD     Analog Fila Columna    
LiquidCrystal_I2C  lcd(0x27,  16,   2);

// definiendo el tipo de dato según su medición
int SENSOR = 6;			// DHT11 a pin digital 6
float TEMPERATURA;  
int HUMEDAD;
DHT dht(SENSOR, DHT11);		// creacion del objeto para el DHT11

// Variable de control del Ventiladdor
int fan = 11;

// Variables de control del Servomotor
Servo myservo;  // Crear un objeto para controlar el servomotor
int pos = 0;    // Variable que almacena la posición del servomotor

// En #define se puede cambiar el tipo de sensor
// ya que existen varios de la serie MQ-XXX
#define PIN_MQ135 A1
MQ135 mq135_sensor(PIN_MQ135); //Creación del objeto MQ135

// Pines del sensor ultrasónico HC-SR04
int TRIG = 10;
int ECHO = 9;
int DURACION, DISTANCIA;

String variable = "";


// Variables de control de ciclos
unsigned long time_now = 0;
int period = 2000;


void setup() {
  Serial.begin(9600);

  //LCD with I2C
    lcd.backlight(); // Activación luz trasera
    lcd.init();  // Inicio del LCD

  // Sensor de Humedad y Temperatura DHT11
    dht.begin();

  // PinMode del Ventilador de 12V
    pinMode(fan, OUTPUT);

  // Conecta el servo en el pin 5 al objeto servo
    myservo.attach(5);

  // Ciclos
    time_now = millis();

  // HC-SR04 PinMode
    pinMode(TRIG, OUTPUT); 
    pinMode(ECHO, INPUT);

  // Se inicializa el pin digital LED_BUILTIN = 13 como salida.
    pinMode(LED_BUILTIN, OUTPUT);

}


void loop() {
  //String topic = String("ARQUI2_G13_PC");
  //Serial.println(String(topic));
  String topic = String(Serial.readString());

  //---------- Medición de Temperatura Y Humedad
    // -----------        Sensor DHT11
    TEMPERATURA = dht.readTemperature();	// obtencion de valor de temperatura
    HUMEDAD = dht.readHumidity();		// obtencion de valor de humedad
    
  //------------ Medición de Calidad del Aire
    //-------------    Sensor de Gas MQ-135
    float ppm = mq135_sensor.getPPM(); // Obtener el valor de CO2 en el aire
    float ppm2 = mq135_sensor.getCorrectedPPM(TEMPERATURA, HUMEDAD);
    float correctedPPM   = map(ppm2, 0, 1024, 0, 100); //  0, 1024

  //------------- Cantidad de Luz en el ambiente
     //-------------   Módulo de Fotoresistencia
    int Value_D8 = analogRead(A0);   //Leyendo el pin analógico A0 en Arduino

    // Conversion Lux to Lumen 
      // Donde 0 representa claridad y 100 oscuridad, ascendente
    int lux  = map(Value_D8, 0, 1024, 0, 100); //  0, 1024
    int lumen = lux*3.75;

  //------------Medición de la distancia de objetos o personas-----------
    //-------------- Codigo para HC-SR04-----------------------------------
    digitalWrite(TRIG, HIGH); 		// generacion del pulso a enviar
    delay(1);				              // al pin conectado al trigger
    digitalWrite(TRIG, LOW);		  // del sensor

    DURACION = pulseIn(ECHO, HIGH);	// con funcion pulseIn se espera un pulso
    DISTANCIA = DURACION / 58.2;		// distancia medida en centimetros
    
    delay(1000);


  //----1. Monitoreo y acción de iluminación de la habitación.(Web)
    /*
    Primer Ciclo 20 seg*/ 
    if (millis() >= time_now + period){
      time_now += period;

      // Lumen = Lux x Area m2
        // NOTA 9m2 es 3 ancho 3 largo del cuarto
      lcd.setCursor(0, 0);
      lcd.print("Niveles de Luz");
        if (lumen  <= 50) {
          lcd.setCursor(0, 1);
          lcd.print("Normal = " + String(lumen));
          delay(1000); 
          lcd.clear(); // Limpia el display para ingresar nuevos datos
        } else if (lumen  > 50) {
          lcd.setCursor(0, 1);
          lcd.print("High = " + String(lumen) );
          delay(1000); 
          lcd.clear(); // Limpia el display para ingresar nuevos datos
        }

      // Primer Ciclo del LED
      if(DISTANCIA > 30){  // No hay presencia, luz encendida
          /* 
              La App debe notificar que la habitación esta iluminada
              Pero no hay nadie en ella
                * Se puede enviar a través del serial de arduino
                algun indicador
          */ 
        digitalWrite(LED_BUILTIN, LOW);  // Manda un estado bajo (Apagado)
        delay(1000);  
        Serial.println("ARQUI2_G13_NPL-CA " + String("Luz_Encendida_Apagando")); // topic para enviar la notificacion web

      }else if(DISTANCIA <= 30 ){ //Presencia Humana, hay luz
        digitalWrite(LED_BUILTIN, HIGH); // Manda un estado alto (Endendico)
        delay(1000);                      // wait for a second  
        Serial.println("ARQUI2_G13_PL-CA " + String("Luz_Apagada_Encendiendo")); // topic para enviar la notificacion web
      }

      else{
        
        //Serial.println("No se encontraron datos");
      }


      //---- 2. Análisis de limpieza, en el aire en la habitación (Web).
    /* No Optimo

    Primer Ciclo 20 seg*/ 
    
      if (correctedPPM  <= 360) {
        lcd.setCursor(0, 0);
        lcd.print("Niveles de Gas");
        lcd.setCursor(0, 1);
        lcd.print("Normal = " + String(correctedPPM));
        delay(1000); 
        lcd.clear(); // Limpia el display para ingresar nuevos datos
        
        digitalWrite(fan, LOW);
        Serial.println("ARQUI2_G13_AV-CA " + String("Calidad_Aire_Optima")); // topic para enviar la notificacion web

        /* 
          La App debe notificar que el nivel de 
          aire es correcto.
            * Se puede enviar a través del serial de arduino
            algun indicador
            * O se hace desde la Api, indicando los parámetros
              <= 360 niveles normales
              >= 361 niveles altos
        */  
      }else if (correctedPPM  > 361) {
        lcd.setCursor(0, 0);
        lcd.print("Niveles de Gas");
        lcd.setCursor(0, 1);
        lcd.print("High = " + String(correctedPPM));
        delay(1000); 
        lcd.clear(); // Limpia el display para ingresar nuevos datos

        digitalWrite(fan, HIGH);
        Serial.println("ARQUI2_G13_EV-CA " + String("Calidad_Aire_No_Optima")); // topic para enviar la notificacion web

        /* 
            La App debe notificar que el nivel de 
            aire es DEFICIENTE.
              * Se puede enviar a través del serial de arduino
              algun indicador
              * O se hace desde la Api, indicando los parámetros
                <= 450 niveles normales
                >= 451 niveles altos
          */ 
      }

    }


  //---- 3. Análisis de temperatura dentro de la habitación. (Móvil).
    // Notificacion de temperatura dentro de la habitación.
    if(topic == "ARQUI2_G13_E1V-T"){ // Velocidad 1
      
                // x, y
        lcd.setCursor(0, 0);  //setCursor define la posición del cursor en el led
        lcd.print("Velocidad 1"); 
                    // x, y
        lcd.setCursor(0, 1);
        lcd.print( "TEMP: "+String(TEMPERATURA) + " C");  // Imprime el valor de la temperatura 
        delay(1000);
        lcd.clear(); // Limpia el display para ingresar nuevos datos


        for(int i=0; i<=100;i++){
          digitalWrite(fan,i);
          //Serial.println(i);
          delay(500);
        }
        delay(5000);

    }else if(topic == "ARQUI2_G13_E2V-T"){ // Velocidad 2 

            // x, y
      lcd.setCursor(0, 0);  //setCursor define la posición del cursor en el led
      lcd.print("Velocidad 2"); 
                  // x, y
      lcd.setCursor(0, 1);
      lcd.print( "TEMP: "+String(TEMPERATURA) + " C");  // Imprime el valor de la temperatura 
      delay(1000);
      lcd.clear(); // Limpia el display para ingresar nuevos datos

      for(int i=0; i<=200;i++){
        digitalWrite(fan,i);
        //Serial.println(i);
        delay(300);
      }
      delay(5000);

    }else if(topic == "ARQUI2_G13_AV-T"){ // Apaga el ventilador

            // x, y
      lcd.setCursor(0, 0);  //setCursor define la posición del cursor en el led
      lcd.print("Ventilador Apagado"); 
      delay(1000);
      lcd.clear(); // Limpia el display para ingresar nuevos datos

      digitalWrite(fan,LOW);
      delay(2000);
    }else{
      //Serial.println("No se encontró el ventilador");

      digitalWrite(fan,LOW);
      delay(2000);
    }

  //--4. Sistema de seguridad, habilita la entrada o salida de la habitación (Móvil).
      //--------- Servomotor gira de 0 a 90

    if(topic == "ARQUI2_G13_PA"){       // Abre la puerta
      lcd.setCursor(0, 0);
      lcd.print("Acceso Concedido");
      delay(1000); 
      lcd.clear(); // Limpia el display para ingresar nuevos datos

      myservo.write(90);  // Establece la posición inicial en 90 grados
      
    }else if(topic == "ARQUI2_G13_PC"){  // Cierra la puerta
      lcd.setCursor(0, 0);
      lcd.print("Acceso Denegado");
      delay(1000); 
      lcd.clear(); // Limpia el display para ingresar nuevos datos

      myservo.write(0);  // Establece la posición en 0 grados

    }else{
      //myservo.write(0);  // Establece la posición en 0 grados

      lcd.setCursor(0, 0);
      lcd.print("No Mov");
      delay(1000); 
      lcd.clear(); // Limpia el display para ingresar nuevos datos
    }
    


  //----------Envío de Resultados a través del monitor Serial 9600

     
    //DISTANCIA EN BOOLEAN
    if(DISTANCIA > 30){  // No hay presencia, luz encendida
      variable = String("0");
    }else if(DISTANCIA <= 30){ //Presencia Humana, hay luz
      variable = String("1"); 
    }  
    
    

    //Serial.println("ARQUI2_G13_humedad "+ String(HUMEDAD));
    Serial.println("ARQUI2_G13_temperatura "+ String(TEMPERATURA)+ " " +"ARQUI2_G13_luz "+ String(lumen)+ " " + "ARQUI2_G13_gas " + String(correctedPPM) + " " + "ARQUI2_G13_distancia " + String(variable) + " ");	// escritura en monitor serial de los valores

   

}
 