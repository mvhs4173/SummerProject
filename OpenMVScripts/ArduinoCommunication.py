import sensor, time, pyb
class ArduinoComm:
    def __init__(self):
        self.bus = pyb.I2C(2, pyb.I2C.SLAVE, addr=0x12)
        self.bus.deinit()
        self.bus = pyb.I2C(2, pyb.I2C.SLAVE, addr=0x12)
    def getData(self):
        try:
            data = self.bus.recv(3, timeout=50)
            return data;
        except OSError as err:
            pass;
        return None;
    def sendData(self, data):
        data = str(data)
        try:
            self.bus.send(data, timeout=50);#Send the data
        except OSError as err:
            pass;
