import { Router } from "express";
import { subscriberMQTT } from "../controllers/sub.js";
const routerSub = Router();

routerSub.get('/sub', subscriberMQTT);
export default routerSub;