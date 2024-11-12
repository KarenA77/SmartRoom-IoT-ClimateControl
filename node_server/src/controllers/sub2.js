import mqtt from 'mqtt'
import mysql from 'mysql2'
let get = true;
const Topic = "pub";
const sub = mqtt.connect("mqtt://localhost:9000");
    // console.log('sub :>> ', sub);
    sub.on("connect", () => {
        sub.subscribe(Topic);
    });

    sub.on("message", (topic, message) => {
        console.log("topic: ", topic, "  msj: ", message.toString());
    });

    const inserData = async (data) => {
        console.log('data :>> ', data);
    }