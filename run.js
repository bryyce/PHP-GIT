var NB_RUNNER = 1;
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
    this.runners[i] = new Runner("test " + i, 49.234903761261, 4.0691892026346, new Date(), '#'+Math.floor(Math.random()*16777215).toString(16));
  }
  this.getPoints();
};

Run.prototype.getPoints = function () {
  /*for (i = 0; i < NB_RUNNER; i++) {
    delta_lat = ((Math.random() - 0.5)/10000)
    delta_lng = ((Math.random() - 0.5)/10000)
    this.runners[i].addPoint(this.runners[i].currentLat + delta_lat, this.runners[i].currentLng + delta_lng, new Date());
  }*/
  $.getJSON("./points/", function(points) {
    for (var i = 0; i < NB_RUNNER; i++) {
        delta_lat =0; ((Math.random() - 0.5)/1000)
        delta_lng =0; ((Math.random() - 0.5)/1000)
        var runner = run.runners[i];
        for(var j = 0; j < points.length; j++) {
          var point = points[j];
          runner.addPoint(point.lat*1 + delta_lat, point.lng*1 + delta_lng, point.date);
        }
    }
    //setTimeout(run.getPoints, 100);
  });
 /* */
}
function sleep(ms) {
    if( ms != false ) {
      setTimeout()
    }
}
var run;
function initialize() {
  MAP_OPTIONS = {
    zoom: 18,
    mapTypeId: google.maps.MapTypeId.HYBRID,
    center: {lat: 49.234903761261, lng: 4.0691892026346}
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
