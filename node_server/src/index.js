
import app from './app.js';
import dotenv from 'dotenv'
import db from './db.js'
dotenv.config();

async function main() {
  await app.listen(process.env.SERVER_PORT, async () => {
    console.log('Servidor iniciado en ' + process.env.WEB_SERVER_HOST + ":" + process.env.SERVER_PORT);
    // db.query('SELECT * FROM Proyecto2.actual ', (err, rows) => {
    //   if (err) throw err;
    //   console.log(rows);
    // });
  })
}
//Run the server and main app
main();