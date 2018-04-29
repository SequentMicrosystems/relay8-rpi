[![relay8-rpi](readmeres/sequent.jpg)](https://www.sequentmicrosystems.com)

# relay8-rpi

This is the command to control [Raspberry Pi 8 Relay Board](https://www.sequentmicrosystems.com)

Before compiling you have to install [Wiring Pi](http://wiringpi.com/download-and-install/), many thanks to Gordon Henderson for the library

## Usage

```bash
~$ git clone https://github.com/alexburcea2877/relay8-rpi.git
~$ cd relay8-rpi/
~/relay8-rpi$ sudo make install
```

Now you can access all the functions of the 8 Relay board through the command "relay8"
If you clone the repository any update can be made with the following commands:

```bash
~$ cd relay8-rpi/  
~/relay8-rpi$ git pull
~/relay8-rpi$ sudo make install
```  
