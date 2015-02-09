var NB_POINT = 50
var Runner = function (_name, _lat, _lng, _date, _color) {
  this.points = new Array({lat: _lat, lng: _lng});
  this.name = _name;
  this.currentLat = _lat;
  this.currentLng = _lng;
  this.time = _date;
  this.date = _date;
  this.marker = undefined;
  this.line = undefined;
  this.infowindow = undefined;
  this.color = _color;

  this.init();
};

Runner.prototype.init = function() {
  this.marker = new google.maps.Marker({
    position: this.getPosition(),
    map: run.map,
    title: "Coureur " + this.name,
    runner: this
  });
  this.infowindow = new google.maps.InfoWindow({content: this.toString()});

  google.maps.event.addListener(this.marker, 'click', function() {
    if (run.openedWindow != undefined) {
      run.openedWindow.close();
    }
    run.openedWindow = this.get('runner').infowindow;
    this.get('runner').infowindow.open(this.get('map'), this);
  });

  this.line = new google.maps.Polyline({
    path: this.points,
    geodesic: true,
    strokeColor: this.color,
    map: run.map,
    strokeOpacity: 1.0,
    strokeWeight: 2
  });
};

Runner.prototype.addPoint = function(lat, lng, time) {
  if (this.points.length > NB_POINT) {
    this.points.shift();
  }
  this.points.push({lat: lat, lng: lng});
  this.currentLat = lat;
  this.currentLng = lng;
  console.log(time)
  this.time = new Date(time).getTime();
  this.date = time;
  this.updatePosition();
};

Runner.prototype.updatePosition = function () {
  this.marker.setPosition(this.getPosition());
  this.line.setPath(this.points);
  this.infowindow.setContent(this.toString())
};

Runner.prototype.getPosition = function() {
  return {lat:this.currentLat, lng: this.currentLng};
};

Runner.prototype.toString = function() {
  pad = "00"
  return "<h3>Coureur " + this.name + "</h3>" +
        "<div><b>Latitude : </b>" + this.currentLat + "</div>" +
        "<div><b>Longitude : </b>" + this.currentLng + "</div>" +
        "<div><b>Dernier point : </b>" + this.date + "</div>";
};
