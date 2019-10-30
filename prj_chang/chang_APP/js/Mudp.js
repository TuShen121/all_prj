document.addEventListener( "plusready",  function()
{
    var _BARCODE = 'Mudp',
		B = window.plus.bridge;
    var Mudp = 
    {
    	createUdp : function (Argus1,successCallback, errorCallback ) 
		{
			var success = typeof successCallback !== 'function' ? null : function(args) 
			{
				successCallback(args);
			},
			fail = typeof errorCallback !== 'function' ? null : function(code) 
			{
				errorCallback(code);
			};
			callbackID = B.callbackId(success, fail);

			return B.exec(_BARCODE, "createUdp", [callbackID, Argus1]);
		},
		receiveUdp : function (successCallback, errorCallback ) 
		{
			var success = typeof successCallback !== 'function' ? null : function(args) 
			{
				successCallback(args);
			},
			fail = typeof errorCallback !== 'function' ? null : function(code) 
			{
				errorCallback(code);
			};
			callbackID = B.callbackId(success, fail);
		
			return B.exec(_BARCODE, "receiveUdp", [callbackID]);
		},
		closeUdp : function (successCallback, errorCallback ) 
		{
			var success = typeof successCallback !== 'function' ? null : function(args) 
			{
				successCallback(args);
			},
			fail = typeof errorCallback !== 'function' ? null : function(code) 
			{
				errorCallback(code);
			};
			callbackID = B.callbackId(success, fail);
		
			return B.exec(_BARCODE, "closeUdp", [callbackID]);
		},
		sendUdp : function (data,ip,port,successCallback, errorCallback ) 
		{
			var success = typeof successCallback !== 'function' ? null : function(args) 
			{
				successCallback(args);
			},
			fail = typeof errorCallback !== 'function' ? null : function(code) 
			{
				errorCallback(code);
			};
			callbackID = B.callbackId(success, fail);
		
			return B.exec(_BARCODE, "sendUdp", [callbackID,data,ip,port]);
		},
    };
    window.plus.Mudp = Mudp;
}, true );