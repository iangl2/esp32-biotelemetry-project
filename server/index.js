const express = require("express");
const app =express();
const http = require("http");
const cors= require("cors");  
const { Server } = require("socket.io");
const { disconnect } = require("process");


app.use(cors());
const server = http.createServer(app);

const io = new Server(server, {
    cors: {
        origin: "http://172.29.32.186:3000",  //IMPORTANTE: AQUÍ CAMBIO EL IP
        methods:["GET", "POST"],
    },
});

io.on("connection", (socket)=>{
    console.log(socket.id);
    socket.on("disconnect", () => {
        console.log("User Disconnected", socket.id);
    });
});



const { DelimiterParser } = require("@serialport/parser-delimiter")
const SerialPort = require("serialport").SerialPort

const port = new SerialPort(
    {path:"COM18", baudRate: 9600} //AQUÍ CAMBIO EL PUERTO
)

const parser = port.pipe(new  DelimiterParser({ delimiter: '\n' }))
parser.on('open', function (){console.log('conexión abierta');});

parser.on('data', function (data){
    var enc= new TextDecoder();
    var arr =new Uint8Array(data);
    ready=enc.decode(arr);
    console.log(ready);
    io.emit('serialdata', ready);
});
port.on('error', function (err){
console.log(err);
});


server.listen(3001,() =>{
    console.log("SERVER RUNNING");
});