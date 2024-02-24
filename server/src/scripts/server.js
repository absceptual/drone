/*
import express from 'express';
import { createServer } from 'node:http';
import { Server } from 'socket.io';

const app = express();
const server = createServer(app);
const io = new Server(server);

io.on('connection', (socket) => {
    console.log("new user connected!");
})

server.listen(8080, () => {
  console.log('server running at http://localhost:8080');
});
*/

import WebSocket, { WebSocketServer } from 'ws';

// Create a WebSocket server by specifying the port number
const wss = new WebSocketServer({ port: 8080 });

// Listen for connection events
wss.on('connection', function connection(ws) {
  console.log('A new client connected.');

  // Listen for messages from clients
  ws.on('message', function incoming(message) {
    console.log('received: %s', message);

    // Echo the received message back to the client
    ws.send(`Server received: ${message}`);
  });

  // Send a welcome message to the newly connected client
  ws.send('Welcome to the WebSocket server!');
});

console.log('WebSocket server started on ws://localhost:8080');