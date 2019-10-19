package com.example.H5PlusPlugin;

import io.dcloud.common.DHInterface.IWebview;
import io.dcloud.common.DHInterface.StandardFeature;
import io.dcloud.common.util.JSUtil;
import java.io.InputStream;
import java.io.OutputStream;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.os.Bundle;
import java.io.IOException;
import java.util.Set;
import java.util.UUID;


/**
 * 5+ SDK 扩展插件示例
 * 5+ 扩扎插件在使用时需要以下两个地方进行配置
 * 		1  WebApp的mainfest.json文件的permissions节点下添加JS标识
 * 		2  assets/data/properties.xml文件添加JS标识和原生类的对应关系
 * 本插件对应的JS文件在 assets/apps/H5Plugin/js/test.js
 * 本插件对应的使用的HTML assest/apps/H5plugin/index.html
 * 
 * 更详细说明请参考文档http://ask.dcloud.net.cn/article/66
 * **/
public class Bluetooth extends StandardFeature
{

    private IWebview       receiveDataWebview;
    private String         receiveDataCallBackID;

    private BluetoothAdapter mBluetoothAdapter= BluetoothAdapter.getDefaultAdapter();
    private BluetoothSocket socket;//连接成功后返回的socket服务，类似TCP的socket
    private String MY_UUID = "00001101-0000-1000-8000-00805F9B34FB";
    private InputStream is;//这个放在类的私有变量里
    private OutputStream os;

    public void onStart(Context pContext, Bundle pSavedInstanceState, String[] pRuntimeArgs) {
        //setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);//强制竖屏
        /**
         * 如果需要在应用启动时进行初始化，可以继承这个方法，并在properties.xml文件的service节点添加扩展插件的注册即可触发onStart方法
         * */
    }

    public void getBondedDevices(IWebview pWebview, JSONArray array)
    {
        String CallBackID   = array.optString(0);
        JSONArray resultAry = new JSONArray();
        if(!mBluetoothAdapter.isEnabled()){
            //蓝牙没有打开
            mBluetoothAdapter.enable();//打开蓝牙
            JSUtil.execCallback(pWebview, CallBackID, "获取失败", JSUtil.ERROR, false);
            return ;
        }
        Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();
        if(pairedDevices.size() > 0){
            for(BluetoothDevice device:pairedDevices){
                try {
                    JSONObject resultObj =new JSONObject();
                    resultObj.put("name",device.getName());
                    resultObj.put("address",device.getAddress());
                    resultAry.put(resultObj);
                } catch (JSONException e) {
                    JSUtil.execCallback(pWebview, CallBackID, "获取失败", JSUtil.ERROR, false);
                    e.printStackTrace();
                }
            }
        }
        //如果没有配对的设备返回空对象
        JSUtil.execCallback(pWebview, CallBackID, resultAry, JSUtil.OK, false);
    }
    public void connectDevice(IWebview pWebview, JSONArray array)
    {

        String CallBackID = array.optString(0);
        String address = array.optString(1);

        BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);
        try {
            socket = device.createRfcommSocketToServiceRecord(UUID.fromString(MY_UUID));
            socket.connect();
            is = socket.getInputStream();//这句放在你连接成功之后，不然报错
            JSUtil.execCallback(pWebview, CallBackID, "连接成功", JSUtil.OK, false);
        } catch (IOException e) {
            JSUtil.execCallback(pWebview, CallBackID, "连接失败", JSUtil.ERROR, false);
            e.printStackTrace();
        }
    }
    public void sendData(IWebview pWebview, JSONArray array)
    {

        String CallBackID = array.optString(0);
        String data = array.optString(1);
        if(socket!=null && socket.isConnected())
        {
            try {
                os = socket.getOutputStream();
                os.write(data.getBytes());
                os.write(0x0d);
                os.write(0x0a);
                JSUtil.execCallback(pWebview, CallBackID, "发送成功", JSUtil.OK, false);
            } catch (IOException e) {
                JSUtil.execCallback(pWebview, CallBackID, "发送失败,没有输出流", JSUtil.ERROR, false);
                e.printStackTrace();
            }
        }
        else {
            JSUtil.execCallback(pWebview, CallBackID, "发送失败，没有连接", JSUtil.ERROR, false);
        }
    }
    public void receiveData(IWebview pWebview, JSONArray array)
    {

        String CallBackID = array.optString(0);
        receiveDataWebview = pWebview;
        receiveDataCallBackID = CallBackID;

        new Thread(){
            public void run(){
                int num = 0;
                byte[] buffer = new byte[1024];
                byte[] buffer_new = new byte[1024];
                int i = 0;
                int n = 0;
                while(true)
                {
                    try {
                        if(socket!=null && socket.isConnected()) {
                            num = is.read(buffer);//获取本次读取长度
                            for(i=0;i<num;i++){
                                if((buffer[i] == 0x0d)&&(buffer[i+1]==0x0a)){
                                    //收到0x0d 0x0a才算结束，你也可以自定义结束符号
                                    String str = new String(buffer_new, 0, n);
//                                    System.out.println("=================:"+str+"---"+n);//打印收到的数据和数据的长度
                                    JSUtil.execCallback(receiveDataWebview, receiveDataCallBackID, str, JSUtil.OK, true);
                                    i++;//一定要加上，因为本次读取0x0d,下一次不用读取0x0a，让他跳过
                                    n=0;
                                }else{
                                    buffer_new[n] = buffer[i];
                                    n++;
                                }
                            }
                        }
                    }catch(IOException e){
                        //JSUtil.execCallback(receiveDataWebview, receiveDataCallBackID, "接收出错", JSUtil.ERROR, true);
                        e.printStackTrace();
                    }
                }
            }
        }.start();
    }
}