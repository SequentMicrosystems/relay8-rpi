[![relay8-rpi](readmeres/sequent.jpg)](https://www.sequentmicrosystems.com)

# relay8-rpi

This is the command line and python functions to control the Sequent Microsystems 8 Relay Card for Raspberry PI Ver. 1 - 2. If you have Ver. 3 with pluggable connectors, download from 
https://github.com/SequentMicrosystems/8relay-rpi

Before compiling you have to install [Wiring Pi](http://wiringpi.com/download-and-install/), many thanks to Gordon Henderson for the library

Don't forget to enable I2C communication:
```bash
~$ sudo raspi-config
```

## Usage

```bash
~$ git clone https://github.com/SequentMicrosystems/relay8-rpi.git
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
