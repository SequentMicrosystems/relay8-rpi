
[![relay8-rpi](res/sequent.jpg)](https://www.sequentmicrosystems.com/megaio.html)

# relay8.py

This is the python library to control [Raspberry Pi 8 Relay Board](https://www.sequentmicrosystems.com)

Before use you have to install python smbus library

## Functions
### set(stack, relay, value)
Set one relay state.

stack - stack level of the relay8 card (selectable from address jumpers [0..7])

relay - relay number (id) [1..8]

value - relay state 1: turn ON, 0: turn OFF[0..1]

### set_all(stack, value)
Set all relays state.

stack - stack level of the relay8 card (selectable from address jumpers [0..7])

value - 8 bit value of all relays (ex: 255: turn on all relays, 0: turn off all relays, 1:turn on relay #1 and off the rest)

### get_all(stack)
Return the state of all relays.

stack - stack level of the relay8 card (selectable from address jumpers [0..7])

return - [0..255]

### get(stack, relay)
Return the state of one relay.

stack - stack level of the relay8 card (selectable from address jumpers [0..7])

relay - relay number (id) [1..8]

return - [0..1]

## Usage example
Browse to relay8-rpi python directory 

```bash
~$ cd relay8-rpi/python/
```

Start Python

```bash
~/relay8-rpi/python$ python
Python 2.7.9 (default, Sep 17 2016, 20:26:04)
[GCC 4.9.2] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>>
```
Import library
```bash
>>> import relay8
```
Write one relay state ( turn on relay #3)

```bash
>>> relay8.set(0, 3, 1)
```

Read one relay state (read relay #3)

```bash
>>> relay8.get(0, 3)
1
```
Read all relays state

```bash
>>> relay8.get_all(0)
4
```

Write all relays state ( turn all ON and turn all OFF)

```bash
>>> relay8.set_all(0, 255)
>>> relay8.set_all(0, 0)
```

