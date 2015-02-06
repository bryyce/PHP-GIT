var Runner = function (_name, _color) {
  this.points = new Array();
  this.name = _name;
  this.currentLat = 0;
  this.currentLng = 0;
  this.time = 0;
  this.marker = undefined;
  this.line = undefined;
  this.color = _color;
};
var NB_POINT = 50
Runner.prototype.addPoint = function(lat, lng, time) {
  if (this.points.length > NB_POINT) {
    this.points.shift();
  }
  this.points.push({lat: lat, lng: lng});
  this.currentLat = lat;
  this.currentLng = lng;
  this.time = time;
}


Runner.prototype.toString = function() {
  pad = "00"
  return "<h3>Coureur " + this.name + "</h3>" +
        "<div><b>Latitude : </b>" + this.currentLat + "</div>" +
        "<div><b>Longitude : </b>" + this.currentLng + "</div>" +
        "<div><b>Dernier point : </b>" + (pad + this.time.getDay()).slice(-pad.length)  + "/" + (pad + this.time.getMonth()).slice(-pad.length)  + "/" + this.time.getFullYear()  + " "  + this.time.toLocaleTimeString() + "</div>";
}