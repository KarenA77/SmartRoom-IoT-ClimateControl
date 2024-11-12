# SmartRoom-IoT-ClimateControl
<h6> Primer Semestre 2021
     Universidad de San Carlos de Guatemala -USAC- 
     Arquitectura de Computadoras y Ensambladores  2 
</h6>
Este proyecto implementa un sistema de control climático inteligente para habitaciones, utilizando tecnología IoT. La solución integra sensores de temperatura, luz, calidad del aire y proximidad conectados a un Arduino con Wi-Fi. Los datos se envían a la nube mediante MQTT y se almacenan en una base de datos, permitiendo control y monitoreo en tiempo real a través de aplicaciones web y móviles. El sistema permite ajustar la iluminación, purificar el aire, controlar la temperatura, y gestionar la seguridad de acceso, con un enfoque en la privacidad y seguridad de los datos.

Requerimientos Mínimos
* Hardware
    * Arduino UNO
    * Sensores DHT11, MQ-135, HC-SR04
    * Módulo de fotorresistencia
    * Servomotor
    * Ventilador de 12V
    * Pantalla LCD con interfaz I2C o con potenciómetro
    
* Software: Arduino IDE, MQTT
