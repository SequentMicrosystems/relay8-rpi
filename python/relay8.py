import smbus

#bus = smbus.SMBus(1)    # 0 = /dev/i2c-0 (port I2C0), 1 = /dev/i2c-1 (port I2C1)

DEVICE_ADDRESS = 0x20     #7 bit address (will be left shifted to add the read write bit)

RELAY8_INPORT_REG_ADD = 0x00
RELAY8_OUTPORT_REG_ADD = 0x01
RELAY8_POLINV_REG_ADD = 0x02
RELAY8_CFG_REG_ADD = 0x03

def check(bus, add):
	cfg = bus.read_byte_data(add, RELAY8_CFG_REG_ADD)
	if(cfg != 0):
		bus.write_byte_data(add, RELAY8_CFG_REG_ADD, 0)
		bus.write_byte_data(add, RELAY8_OUTPORT_REG_ADD, 0)
	return bus.read_byte_data(add, RELAY8_INPORT_REG_ADD)


def set(stack, relay, value):
	bus = smbus.SMBus(1)
	if stack < 0 or stack > 7:
		raise ValueError('Invalid stack level')
		return
	if relay < 1:
		raise ValueError('Invalid relay number')
		return
	if relay > 8:
		raise ValueError('Invalid relay number')
		return
	oldVal = check(bus, DEVICE_ADDRESS + stack)	
	if value == 0:
		oldVal = oldVal & (~(1 << (relay - 1)))
		bus.write_byte_data(DEVICE_ADDRESS + stack, RELAY8_OUTPORT_REG_ADD, oldVal)
	else:
		oldVal = oldVal | (1 << (relay - 1))
		bus.write_byte_data(DEVICE_ADDRESS + stack, RELAY8_OUTPORT_REG_ADD, oldVal)

		
def set_all(stack, value):
	bus = smbus.SMBus(1)
	if stack < 0 or stack > 7:
		raise ValueError('Invalid stack level')
		return
	if value > 255 :
		raise ValueError('Invalid relay value')
		return
	if value < 0:
		raise ValueError('Invalid relay value')
		return
	oldVal = check(bus, DEVICE_ADDRESS + stack)	
	bus.write_byte_data(DEVICE_ADDRESS + stack, RELAY8_OUTPORT_REG_ADD, value)


	
def get(stack, relay):
	bus = smbus.SMBus(1)
	if stack < 0 or stack > 7:
		raise ValueError('Invalid stack level')
		return
	if relay < 1:
		raise ValueError('Invalid relay number')
		return
	if relay > 8:
		raise ValueError('Invalid relay number')
		return
	val = check(bus, DEVICE_ADDRESS + stack)
	val = val & (1 << (relay - 1))
	if val == 0:
		return 0
	else:
		return 1
		
		
	
def get_all(stack):
	bus = smbus.SMBus(1)
	if stack < 0 or stack > 7:
		raise ValueError('Invalid stack level')
		return
	return check(bus, DEVICE_ADDRESS + stack)