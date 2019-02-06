# Noise Animation

## compile:
    make.bat

## usage:
* 1. generate bmp files [%d.bmp] from [video.suffix] into folder [p]
```bash
example : ffmpeg ffmpeg -i video.suffix -s 640x480 -r 60 p/%d.bmp
```
* 2. revise mask2bin.cpp (// first bmp file) and (// 0001.bmp to 4383.bmp) in correspondence with bmp files that you generated before
        example : p/1.bmp to 15.bmp --> for (int i = 2; i <= 15; ++i)
* 3. use make.bat to compile 2 applications
        attention : opengl sdk should be installed first !!!
        noise.exe noise.bin bmp2bin.exe are already generated as examples
* 4. put folder [p] and [bmp2bin.exe] at the same folder and run [bmp2bin.exe] so that bin file be generated
     then rename out.bin to noise.bin
* 5. put noise.exe noise.bin at the same folder and enjoy

## control:
* UP&DWON     : scroll speed, value=1 recommanded
* LEFT&RIGHT  : frame rate, you may have better experience if set value a little more than screen refresh rate.
* SPACE       : pause and resume video
* ESC         : exit application

## more:
    contact me via thinknaive@hotmail.com if you have any issues and I may get in touch with you ocassionally due to my laziness. XD
