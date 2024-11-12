import mosca from 'mosca'
const port = 9000;

const broker = new mosca.Server({
  port: port,
});

broker.on("ready", () => {
  console.log("<SERVER BROKER PORT:" + port + ">");
});

broker.on("clientConnected", (client) => {
  console.log("(MQTT cliente conectado, id:", client.id + ")");
});

broker.on("clientDisconnected", function onClientDisconnected(client) {
  console.log("(MQTT cliente desconectado, id:", client.id + ")");
});