let io = require('socket.io-client')

let socket = io.connect('http://localhost:3000')

socket.on('connect', () => {
  let i = 0;
  setInterval(()=> {
    console.log(i)
    i += 1
    socket.emit('data', {value: i});
  }, 100);
});



