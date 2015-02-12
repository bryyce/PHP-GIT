var NB_POINT = 1000;
var STEP = 50;
var TIMEOUT = 500;
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
  this.deferedPoint = undefined;
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
 //this.deferedPoint = new $.Defered();
  var that = this;
  new MarkerMoveAnimation(this.marker,
    {
      name: name,
      from: new google.maps.LatLng(this.getPosition().lat, this.getPosition().lng),
      to: new google.maps.LatLng(lat, lng),
      duration: 1400,
      effect: '',
      onStep: function(e) {
        run.map.setCenter(e.marker.getPosition());
        that.realPush(e.marker.getPosition().lat(), e.marker.getPosition().lng());
      },
      onComplete: function(e) {
        that.realPush(lat, lng, true);
      }
    });
};

Runner.prototype.realPush = function (lat, lng, all) {
  if (all == true) {
    if (this.points.length == NB_POINT)
      this.points.shift();
    this.points.push({"lat": lat, "lng": lng});
    this.smoothPoints = new Array();

  }
  if (this.smoothPoints.length == NB_POINT)
    this.smoothPoints.shift();
  this.smoothPoints.push({"lat": lat, "lng": lng});
  this.updatePosition({"lat": lat, "lng": lng})
}

Runner.prototype.updatePosition = function (pos) {
  this.marker.setPosition(pos);
  this.line.setPath(this.points.concat(this.smoothPoints));
  this.infowindow.setContent(this.toString())
  //if (!run.map.getBounds().contains(this.marker.getPosition()))
    run.map.setCenter(this.marker.getPosition());
};

Runner.prototype.getPosition = function() {
  return {lat:this.smoothPoints[this.smoothPoints.length - 1].lat, lng: this.smoothPoints[this.smoothPoints.length - 1].lng};
};

Runner.prototype.toString = function() {
  pad = "00"
  return "<h3>Coureur " + this.name + "</h3>" +
        "<div><b>Latitude : </b>" + this.getPosition().lat + "</div>" +
        "<div><b>Longitude : </b>" + this.getPosition().lng + "</div>" +
        "<div><b>Dernier point : </b>" + this.date + "</div>";
};
/*
var counter = 0;
interval = window.setInterval(function() {
  counter++;
  // just pretend you were doing a real calculation of
  // new position along the complex path
  var pos = new google.maps.LatLng(35, -110 + counter / 100);
  marker.setPosition(pos);
  if (counter >= 1000) {
    window.clearInterval(interval);
  }
}, 10);*/
function MarkerMoveAnimation(marker, options)
{
  //public vars
  this.name = options.name || 'default'; //optional: you can specify a name for this animation object to keep them apart
  this.marker = marker;

  //private vars
  var delay = 33; //how often shall the interval for a animation step be called? (in milliseconds) 33 ~ 30fps, 16 ~ 60fps
  var options = options || { };
  options.from = options.from || null; //The position from where the animation should start (accepts only a google.maps.LatLng object)
  options.to = options.to || null; //The position till where the animation should end (accepts only a google.maps.LatLng object)
  options.duration = options.duration || 1000; //Duration of the animation in milliseconds
  options.effect = options.effect || 'linear'; //Effect name (can be linear, easein, easeout)
  var beginTime; //Date object representing the start time of the animation
  var timePassed; //Difference between beginTime and the current time
  var progress; //Float between 0 and 1 representing the progress of the animation (0 = begin, 1 = end)
  var delta; //Float between 0 and 1 representing a factor by which to multiply to create certain animation effects like easein/out
  var positionDiff; //Object with lat and lng differences of options.from and options.to
  var interval; //holds the setInterval handle when an animation is running


  //public methods
  this.stop = function()
  {
    console.log("animation is stopped.");
    clearInterval(interval);

    if(options.onComplete && typeof options.onComplete === "function")
      options.onComplete(this);

  }

  this.setFrom = function(point) { options.from = point; this._updatePositionDiff(); }

  this.setTo = function(point) { options.to = point; this._updatePositionDiff(); }

  this.setDuration = function(milliseconds) { options.duration = milliseconds; }

  this.setEffect = function(string) { options.effect = string; }

  this.getFrom = function() { return options.from; }

  this.getTo = function() { return options.to; }

  this.getDuration = function() { return options.duration; }

  this.getEffect = function() { return options.effect; }

  this.start = function()
  {
    if(this._check())
    {
      beginTime = new Date;
      this._updatePositionDiff();

      //is there a callback just before we start the animation?
      if(options.onBeforeStart && typeof options.onBeforeStart === "function")
        options.onBeforeStart(this);

      //do the actual animation
      interval = setInterval(function(self) { self._animate(); }, delay, this);
    }
  }


  //private methods

  this._animate = function()
  {
    timePassed = new Date - beginTime;
    progress = this._progress(timePassed);
    delta = this._delta(progress);

    if(progress == 1)
    {
      this.stop();
    }
    else
      this._step(delta);
  }

  this._progress = function(timePassed)
  {
    var p = timePassed / options.duration;

    return (p > 1) ? 1 : p;
  }

  this._delta = function(progress)
  {
    switch(options.effect)
    {
      case 'linear':
      default:
      return progress;
      break;

      case 'easein':
      return Math.pow(progress, 3);
      break;

      case 'easeout':
      return 1 - (Math.pow((1 - progress), 3));
      break;

    }
  }

  this._step = function(delta)
  {
    var newlat = options.from.lat() + (positionDiff.lat * delta);
    var newlng = options.from.lng() + (positionDiff.lng * delta);

    this.marker.setPosition(new google.maps.LatLng(newlat, newlng));

    if(options.onStep && typeof options.onStep === "function")
      options.onStep(this);
  }


  this._check = function()
  {
    console.log('check');
    try
    {
      if(!this.marker || !this.marker.getPosition)
        throw "NoValidMarkerObject";
      if(!options.from || !options.from.lat || !options.to || !options.to.lat)
        throw "NoValidFromToLatLngObjects";

      return true;
    }
    catch(error)
    {
      switch(error)
      {
        case "NoValidMarkerObject":
        break;

        case "NoValidFromToLatLngObjects":
        break;

        default:
        break;
      }

      this.stop();
    }
  }

  this._updatePositionDiff = function() { positionDiff = { lat: (options.to.lat() - options.from.lat()) , lng: (options.to.lng() - options.from.lng()) }; }

  //Start the animation!

  this.start();
}