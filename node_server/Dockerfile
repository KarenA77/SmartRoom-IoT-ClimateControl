# Usamos la imagen oficial de Node.js 18.12.1
FROM node:18.12.1

# Establecemos el directorio de trabajo en /app
WORKDIR /app

# Copiamos los archivos de tu proyecto al directorio de trabajo en el contenedor
COPY . .

# Instalamos las dependencias del proyecto
RUN npm install

# Definimos las variables de entorno para la configuración de la aplicación (ajusta estas variables según tus necesidades)
ENV DBHOSTNAME=35.225.156.158
ENV DB_USERNAME=root
ENV DB_PORT=3306
ENV DB_PASSWORD=
ENV INSTANCE=proyecto2
ENV NAMEDATABASE=Proyecto2
ENV DB_DIALECT=mssql
ENV DISABLE_DB_LOGS=true
ENV SERVER_PORT=3000
ENV WEB_SERVER_HOST=localhost
# Exponemos el puerto en el que la aplicación va a escuchar
EXPOSE 3000

# Comando para iniciar la aplicación, en este caso, npm run dev
CMD ["npm", "run", "dev"]
