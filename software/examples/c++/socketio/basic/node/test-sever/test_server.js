let express = require('express')
let http = require('http')
let socketio = require('socket.io')

let app = express()
let server = http.Server(app)
let io = socketio(server)

app.use(express.static('public'))

let port = 3000
server.listen(port)
console.log('listening on port = ' + port)

io.on('connection', (socket) => {
  console.log('client connected: ' + socket.handshake.address)
  socket.on('data', (data) => {
    console.log('data: ' + JSON.stringify(data))
    io.emit('data', data);
  })
})

app.get('/', (req, res) => {
  res.sendfile('index.html')
})
