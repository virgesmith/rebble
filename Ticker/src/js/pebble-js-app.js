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

function getIndex(ccy) {
  // Construct URL
  //var url = 'http://finance.yahoo.com/webservice/v1/symbols/^'+name+'/quote?format=json&view=detail';
  var url = 'https://blockchain.info/tobtc?currency='+ccy+'&value=1';

  // Send request
  xhrRequest(url, 'GET', 
    function(responseText) {
    
      //console.log("index response:" + responseText);
      // responseText contains a JSON object with weather info
      //var json = JSON.parse(responseText);
      var price = 100.0 / responseText;

      //console.log(price.toFixed(0));

      // Assemble dictionary using our keys
      var dictionary = {
        "KEY_PRICE": price,		
      };

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
    console.log("PebbleKit JS ready!");
		var dict = { "KEY_CONNECTION_STATUS" : 1 };
    Pebble.sendAppMessage(dict, 
                          function(e) { },                     
                          function(e) { });                     
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    getIndex(e.payload["KEY_CCY"]);
  }                     
);
