import express, { json } from 'express'
import morgan from 'morgan'
import cors from "cors";
import routerSub from './routes/subscriber.routes.js';
import routerPub from './routes/publisher.routes.js';
const app = express()

app.all('*', function (req, res, next) {
    res.header('Access-Control-Allow-Origin', '*');
    res.header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS, PUT, PATCH, DELETE');
    res.header('Access-Control-Allow-Headers', 'Authorization, Access-Control-Allow-Methods, Access-Control-Allow-Headers, Access-Control-Allow-Origin, Origin, Content-Type, X-Auth-Token, Accept');
    next();
});
app.use(cors());
app.use(morgan('dev'));
app.use(cors());
app.use(express.json());
// app.use(json({ limit: '100mb' }));
app.use(routerSub,routerPub)

export default app;