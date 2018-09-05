# QRCode Example
#
# This example shows the power of the OpenMV Cam to detect QR Codes
# using lens correction (see the qrcodes_with_lens_corr.py script for higher performance).

import sensor, image, time, pyb, ustruct

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False) # must turn this off to prevent image washout...
clock = time.clock()

text = "Hello World!\n"
data = ustruct.pack("<%ds" % len(text), text)
bus = pyb.I2C(2, pyb.I2C.SLAVE, addr=0x12)
bus.deinit()
bus = pyb.I2C(2, pyb.I2C.SLAVE, addr=0x12)
print("Waiting for Arduino...")

while(True):
    clock.tick()
    img = sensor.snapshot()
    img.lens_corr(1.8) # strength of 1.8 is good for the 2.8mm lens.
    for code in img.find_qrcodes():
        img.draw_rectangle(code.rect(), color = (255, 0, 0))
        #print(code)
        lcode = code.payload()
        print(code.payload())
        if 'forward' in lcode.lower():
            data = "forward\n"
        if 'left' in lcode.lower():
            data = "left\n"
        if 'right' in lcode.lower():
            data = "right\n"
        if 'reverse' in lcode.lower():
            data = "reverse\n"
    print(clock.fps())
    try:
        bus.send(ustruct.pack("<h", len(data)), timeout=10000)
        try:
            bus.send(data, timeout=10000)
            print("Sent Data!")
            data = 'waiting\n'
        except OSError as err:
            pass
    except OSError as err:
        pass
