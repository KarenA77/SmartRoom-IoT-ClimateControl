// import mqtt from 'mqtt'
// import mysql from 'mysql2'
// let get = true;

// export async function subscriberMQTT(req, res) {
//     const Topic = "pub";
//     const sub = mqtt.connect("mqtt://localhost:9000");

//     // Suscribirte a un tema MQTT
//     sub.subscribe('pub');

//     // Manejar los mensajes que llegan en el tema suscrito
//     sub.on('message', (topic, message) => {
//         console.log(`Mensaje recibido en el tema ${topic}: ${message.toString()}`);
//         // Aquí puedes realizar cualquier acción que desees con el mensaje recibido
//     });

//     sub.on('connect', () => {
//         console.log('Conexión exitosa al broker MQTT');
//     });

//     sub.on('error', (err) => {
//         console.error('Error de conexión al broker MQTT:', err);
//     });
// }


import mqtt from 'mqtt';
import { SerialPort, ReadlineParser } from 'serialport';

const Topic = "pub";
const arduinoPortPath = "COM5"; // Reemplaza con el puerto COM de tu Arduino
const arduinoBaudRate = 9600;

export async function subscriberMQTT(req, res) {
    // Simula un tópico MQTT para probar
    const topic = "ARQUI2_G13_PA";
    // Simula un mensaje MQTT para probar
    const message = "Mensaje de prueba";

    // Si el tópico coincide con "ARQUI2_G13_PA", envía el mensaje al Arduino
    //endToArduino(topic);

    // Función para enviar datos al Arduino (simulado)
    //function sendToArduino(data) {
    const port = new SerialPort({
        path: arduinoPortPath,
        baudRate: arduinoBaudRate,
    });

    port.on("open", () => {
        console.log("Conexión serial abierta en " + arduinoPortPath + " " + arduinoBaudRate);
        port.write(topic);
        console.log(topic);
        port.close();
        return res.send("Realizado con exito")
    });

    port.on("error", (err) => {
        console.error("Error en la conexión serial:", err);
        return res.send("Error en la conexión serial:", err)
    });
    //}
}