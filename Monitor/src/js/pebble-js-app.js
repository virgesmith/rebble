var xhrRequest = function (url, type, successCallback, failureCallback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    successCallback(this.responseText);
  };
	xhr.onerror = function() {
		failureCallback();
	}
  xhr.open(type, url);
  xhr.send();
};


function getLanStatus() {
  // Construct URL
  var url = "http://cazaz.ddns.info:3000/network";
	
  // Send request
  xhrRequest(url, 'GET', 
    function(responseText) {
    
		  //console.log("network response:" + responseText);
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);

			var names = "";
			var ips = "";
			var osvers = "";
			var loads = "";
			var uptimes = "";
			var mems = "";
			var disks = "";
			var temps = "";
			
			for (var key in json) {
			  //console.log(key + ":" + json[key]["ip"]);
			  names += key + ";";
			  ips += json[key]["ip"] + ";";
			  osvers += json[key]["osver"] + ";";
			  loads += json[key]["load"] + ";";
			  uptimes += json[key]["uptime"] + ";";
			  mems += json[key]["freemem"] + ";";
			  disks += json[key]["diskused"] + ";";
			  temps += json[key]["cputemp"] + ";";
			}

			// Assemble dictionary using our keys
      var dictionary = {
        "KEY_NAMES": names,
        "KEY_IPS": ips,
        "KEY_OSVERS": osvers,
        "KEY_LOADS": loads,
        "KEY_UPTIMES": uptimes,
        "KEY_MEMS": mems,
        "KEY_DISKS": disks,
        "KEY_TEMPS": temps
      };
		  //console.log("names:" + names);

      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          //console.log("Weather info sent to Pebble successfully!");
        },
        function(e) {
          //console.log("Error sending weather info to Pebble!");
        }
      );
    },
		function() {
			var dictionary = { "KEY_CONNECTION_FAILURE": 0 };
			// Send to Pebble
			Pebble.sendAppMessage(dictionary,
				function(e) {
				},
				function(e) {
				}
			);							
		}      
  );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    //console.log("PebbleKit JS ready!");
    // Get the initial weather
		getLanStatus();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    //console.log("AppMessage received!");
		//getLanStatus();
  }                     
);
