# video_transport
video transport by h264 format

## 发送方
通过x264对视频或者图片数据进行编码为h264格式，然后通过udp协议发送出去;</br>
需要安装x264库：
```
sudo apt-get install x264-dev
```

## 接收方
接收方通过udp协议接收发送方发送的编码后的数据，并通过ffmpeg对其进行解码，然后在qt界面上显示出来；</br>

