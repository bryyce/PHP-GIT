var NB_POINT = 1000;
var STEP = 200;
var Runner = function (_name, _lat, _lng, _date, _color) {
  this.smoothPoints = new Array({lat: _lat, lng: _lng});
  this.points = new Array({lat: _lat, lng: _lng});
  this.idx = 0;
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
  this.idx++;
  this.time = new Date(time).getTime();
  this.date = time;
  var that = this;
  setTimeout(function () {that.pushPoints(lat, lng)}, that.idx * 98 )
};
Runner.prototype.pushPoints = function(lat, lng, time) {
  var that = this;
  pos = this.getPosition();

  diff_lat = (lat - pos.lat) / STEP;
  diff_lng = (lng - pos.lng) / STEP;
  for(var i = 0; i < STEP; i++) {
    (pos.lat = pos.lat + diff_lat);
    (pos.lng = pos.lng + diff_lng);
    setTimeout(function(__lat, __lng) { that.realPush(__lat, __lng)}, i * 0.5, pos.lat, pos.lng);
  }
  setTimeout(function(__lat, __lng) { that.realPush(__lat, __lng, true)}, STEP * 0.5, lat, lng);
}
Runner.prototype.realPush = function (lat, lng, all) {
  if (this.smoothPoints.length == NB_POINT)
    this.smoothPoints.shift();
  this.smoothPoints.push({"lat": lat, "lng": lng});
  if (all == true) {
    if (this.points.length == NB_POINT)
      this.points.shift();
    this.points.push({"lat": lat, "lng": lng});
    this.smoothPoints = new Array();
  }
  this.updatePosition({"lat": lat, "lng": lng})
}

Runner.prototype.updatePosition = function (pos) {
  this.marker.setPosition(pos);
  /*new google.maps.Marker({
    position: (pos == undefined ? this.getPosition(): pos),
    map: run.map,
    title: "Coureur " + this.name,
    runner: this
  });*/
  this.line.setPath(this.points.concat(this.smoothPoints));
  this.infowindow.setContent(this.toString())
  //if (!run.map.getBounds().contains(this.marker.getPosition()))
    //run.map.setCenter(this.marker.getPosition());
};

Runner.prototype.getPosition = function() {
  return {lat:this.smoothPoints[this.smoothPoints.length - 1].lat, lng: this.smoothPoints[this.smoothPoints.length - 1].lng};
};

Runner.prototype.toString = function() {
  pad = "00"
  return "<h3>Coureur " + this.name + "</h3>" +
        "<div><b>Latitude : </b>" + this.getPosition().lat + "</div>" +
        "<div><b>Longitude : </b>" + this.getPosition().lon + "</div>" +
        "<div><b>Dernier point : </b>" + this.date + "</div>";
};
