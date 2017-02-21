Minotor is a MIDI-controlled LED matrix software.

License:
Minotor is released under GPL but embedded RtMidi¹ and QExtSerialPort².

[1] http://www.music.mcgill.ca/~gary/rtmidi/

[2] https://github.com/qextserialport/qextserialport

## Install under Ubuntu (>=16.04)

Under Ubuntu, you can use PPA to install through Ubuntu package manager

~~~
sudo apt-add-repository ppa:neomilium/minotor
sudo apt update
sudo apt install minotor
~~~

# Build and install

## Requirements

### With system-wide rtmdi

~~~
sudo apt install qt5-defaults librtmidi-dev libqt5extserialport-dev
~~~

### With embedded rtmidi, without JACK support

~~~
sudo apt install qt5-defaults libasound2-dev libqt5extserialport-dev
~~~

## Compilation

```
qmake
make
```

