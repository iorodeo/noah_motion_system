
Vue.use(VueMaterial)

Vue.material.registerTheme('default', {
  primary: 'deep-purple',
  accent: 'pink',
  warn: 'orange',
  background: 'grey'
})

var app = new Vue({
  el: '#app',
  data: {
    socket: null,
    message: {},
  },
  methods: {
    setupSockets: function() {
      this.socket = io.connect('http://' + document.domain + ':' + location.port)
      this.socket.on('data', (message) => {
        this.message = message
      });
    },
  },
  mounted: function() { 
    this.setupSockets();
  }
});


