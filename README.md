Minotor is a MIDI-controlled LED matrix software.

License:
Minotor is released under GPL but embedded RtMidi¹ and QExtSerialPort².

[1] http://www.music.mcgill.ca/~gary/rtmidi/

[2] https://github.com/qextserialport/qextserialport

# Build and install

## Requirements

### With system-wide rtmdi

~~~
sudo apt install qtbase5-dev librtmidi-dev libqt5extserialport-dev
~~~

### With embedded rtmidi, without JACK support

~~~
sudo apt install qtbase5-dev libasound2-dev libqt5extserialport-dev
~~~

## Compilation

```
qmake
make
```

