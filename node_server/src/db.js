import Sequelize from 'sequelize';
import dotenv from 'dotenv'
import sql from "mssql";
import { createConnection } from "mysql2";

dotenv.config();
const connection = createConnection({
    host: process.env.DBHOSTNAME,
    user: process.env.DB_USERNAME || 'root',
    password: process.env.DB_PASSWORD,
    database: process.env.NAMEDATABASE,
    port: process.env.DB_PORT
});

connection.connect(err => {
    if (err) {
        console.error('Error conectando la base de datos:', err.stack);
        return;
    }
    console.log('Base de datos Conectada con ID ' + connection.threadId);
});
export default connection
// const dbSettings = {
//     user: process.env.USERNAME,
//     password: process.env.PASSWORD,
//     server: process.env.DBHOSTNAME,
//     database: process.env.NAMEDATABASE,
//     port: 3306,
//     options: {
//         encrypt: true,
//         trustServerCertificate: true
//     }
// }

// export async function getConnection() {
//     try {
//         const pool = await sql.connect(dbSettings);
//         console.log('pool :>> ', pool);
//         return pool;
//     } catch (error) {
//         console.log(error)
//     }
// }

// export { sql };