document.addEventListener( "plusready",  function()
{
    var _BARCODE = 'Bluetooth',
		B = window.plus.bridge;
    var Bluetooth = 
    {
    	getBondedDevices : function (successCallback, errorCallback ) 
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

			return B.exec(_BARCODE, "getBondedDevices", [callbackID]);
		},
		receiveData : function (successCallback, errorCallback ) 
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
		
			return B.exec(_BARCODE, "receiveData", [callbackID]);
		},
		sendData : function (data,successCallback, errorCallback ) 
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
		
			return B.exec(_BARCODE, "sendData", [callbackID,data]);
		},
		connectDevice : function (address,successCallback, errorCallback ) 
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
		
			return B.exec(_BARCODE, "connectDevice", [callbackID,address]);
		},
    };
    window.plus.Bluetooth = Bluetooth;
}, true );

/*调用方法

document.getElementById("peiduiBtn").addEventListener("click",function(){
					recMsgDiv.innerText=recMsgDiv.innerText+"\n"+"456"
					plus.Bluetooth.getBondedDevices(function(result){
							recMsgDiv.innerText=recMsgDiv.innerText+"\n"+"qqq"+JSON.stringify(result)
						},function(result){
							recMsgDiv.innerText=recMsgDiv.innerText+"\n"+"bbb"+result
					});
				})
				document.getElementById("connectBtn").addEventListener("click",function(){
					macIpt.value="5464"
					plus.Bluetooth.connectDevice(
						"20:19:05:14:22:94",
					function(result){
							recMsgDiv.innerText=recMsgDiv.innerText+"\n"+"qqq"+(result)
						},function(result){
							recMsgDiv.innerText=recMsgDiv.innerText+"\n"+"bbb"+result
					});
				})
				document.getElementById("sendBtn").addEventListener("click",function(){
					sendIpt.value="asdfsfd"
					plus.Bluetooth.sendData(
						"20:19:05:14:22:94",
					function(result){
							recMsgDiv.innerText=recMsgDiv.innerText+"\n"+"send"+(result)
						},function(result){
							recMsgDiv.innerText=recMsgDiv.innerText+"\n"+"send!!"+result
					});
				})
				plus.Bluetooth.receiveData(
					function(result){
						recMsgDiv.innerText=recMsgDiv.innerText+"\n"+"RES"+(result)
					},function(result){
						recMsgDiv.innerText=recMsgDiv.innerText+"\n"+"RES!!"+result
				});


*/