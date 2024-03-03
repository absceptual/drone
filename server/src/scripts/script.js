import { WebSocketServer } from 'ws';
import { readFileSync } from 'fs';
import { createWebSocketStream } from 'ws';

const START = 0x90;

// Create a WebSocket server by specifying the port number
const wss = new WebSocketServer({ port: 8080 });

// Open a filestream and redirect it to our websocket
function read(path, duplex) {
  const stream = fs.createReadStream(path);
  stream.pipe(duplex);
}

// Listen for connection events
wss.on('connection', function connection(ws) {
  // This will be the stream we will use to listen to requests on

  console.log("[+] new client connected to server");
  ws.on('error', console.error);

  const duplex = createWebSocketStream(ws);

  let dataBuffer = [];
  duplex.on('data', function(data) {

    dataBuffer.push(data);
    if (Buffer.compare(data, Buffer.from("hello server!", "utf-8")) == 0) {
      // console.log(data, Buffer.from("hello server!", "utf-8"));

      duplex.write(Buffer.from("hello client!", 'utf-8'));
    }
    else {
      let result = Buffer.alloc(1);
      result.writeUint8(0x90);

      // console.log(data, result);
      if (Buffer.compare(data, result) == 0) {
        duplex.write(Buffer.from("hmm, you sent me a start packet.. here's one back.", 'utf-8'));
        duplex.write(result);
      }
      else
        duplex.write("[?] don't know what the fuck you gave me man...");
    }
  
    console.log(`[+] client says: ${data}`);
  });

});

console.log('WebSocket server started on ws://localhost:8080');
