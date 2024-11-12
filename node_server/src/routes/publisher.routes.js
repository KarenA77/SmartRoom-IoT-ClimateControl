import { Router } from "express";
import { historico1, historico2, historico3, leerSensores, publishA, publishB } from "../controllers/pub.js";
const routerPub = Router();

routerPub.get('/pub', publishB);
routerPub.get('/pubA', publishA);
routerPub.post('/actual', leerSensores);
routerPub.post('/historico3', historico3);
routerPub.post('/historico2', historico2);
routerPub.post('/historico1', historico1);
export default routerPub;