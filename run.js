var NB_RUNNER = 50;
var MAP_OPTIONS;

var Run = function () {
  this.startDate = new Date();
  this.map = new google.maps.Map(document.getElementById('map-canvas'), MAP_OPTIONS);
  this.runners = new Array();
  this.openedWindow = undefined;
};

Run.prototype.launch = function() {
  this.map.setTilt(45)
  for (i = 0; i < NB_RUNNER; i++) {
    this.runners[i] = new Runner("test " + i, 49.2534, 4.033, new Date(), '#'+Math.floor(Math.random()*16777215).toString(16));
  }
  var that = this;
  setInterval(function () { that.moveMarker() }, 10)
};

Run.prototype.moveMarker = function () {
  for (i = 0; i < NB_RUNNER; i++) {
    delta_lat = ((Math.random() - 0.5)/10000)
    delta_lng = ((Math.random() - 0.5)/10000)
    this.runners[i].addPoint(this.runners[i].currentLat + delta_lat, this.runners[i].currentLng + delta_lng, new Date());
  }
  // if (!map.getBounds().contains(marker.getPosition()))
  //  map.setCenter(marker.getPosition());
}

var run;
function initialize() {
  MAP_OPTIONS = {
    zoom: 18,
    mapTypeId: google.maps.MapTypeId.HYBRID,
    center: {lat:49.2534, lng: 4.033}
  };
  (run = new Run()).launch();
}

function loadScript() {
  var script = document.createElement('script');
  script.type = 'text/javascript';
  script.src = 'https://maps.googleapis.com/maps/api/js?key=AIzaSyCLugUQetPtAJ69p48ZmNLh-eOPTWTmElo&v=3.exp&' +
      'callback=initialize';
  document.body.appendChild(script);
}