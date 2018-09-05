# AprilTags Max Res Example
#
# This example shows the power of the OpenMV Cam to detect April Tags
# on the OpenMV Cam M7. The M4 versions cannot detect April Tags.

import sensor, image, time, math, omv, pyb, ustruct

sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.VGA) # we run out of memory if the resolution is much bigger...
# AprilTags works on a maximum of < 64K pixels.
if omv.board_type() == "H7": sensor.set_windowing((240, 240))
elif omv.board_type() == "M7": sensor.set_windowing((200, 200))
else: raise Exception("You need a more powerful OpenMV Cam to run this script")
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False)  # must turn this off to prevent image washout...
sensor.set_auto_whitebal(False)  # must turn this off to prevent image washout...
clock = time.clock()

text = '...\n'
data = ustruct.pack("<%ds" % len(text), text)
bus = pyb.I2C(2, pyb.I2C.SLAVE, addr=0x12)
bus.deinit()
bus = pyb.I2C(2, pyb.I2C.SLAVE, addr=0x12)
print("Waiting for Arduino...")

# Note! Unlike find_qrcodes the find_apriltags method does not need lens correction on the image to work.

# The apriltag code supports up to 6 tag families which can be processed at the same time.
# Returned tag objects will have their tag family and id within the tag family.

tag_families = 0
tag_families |= image.TAG16H5 # comment out to disable this family
tag_families |= image.TAG25H7 # comment out to disable this family
tag_families |= image.TAG25H9 # comment out to disable this family
tag_families |= image.TAG36H10 # comment out to disable this family
tag_families |= image.TAG36H11 # comment out to disable this family (default family)
tag_families |= image.ARTOOLKIT # comment out to disable this family

# What's the difference between tag families? Well, for example, the TAG16H5 family is effectively
# a 4x4 square tag. So, this means it can be seen at a longer distance than a TAG36H11 tag which
# is a 6x6 square tag. However, the lower H value (H5 versus H11) means that the false positve
# rate for the 4x4 tag is much, much, much, higher than the 6x6 tag. So, unless you have a
# reason to use the other tags families just use TAG36H11 which is the default family.

def family_name(tag):
    if(tag.family() == image.TAG16H5):
        return "TAG16H5"
    if(tag.family() == image.TAG25H7):
        return "TAG25H7"
    if(tag.family() == image.TAG25H9):
        return "TAG25H9"
    if(tag.family() == image.TAG36H10):
        return "TAG36H10"
    if(tag.family() == image.TAG36H11):
        return "TAG36H11"
    if(tag.family() == image.ARTOOLKIT):
        return "ARTOOLKIT"

while(True):
    clock.tick()
    img = sensor.snapshot()
    img.rotation_corr(z_rotation = 180)
    for tag in img.find_apriltags(families=tag_families): # defaults to TAG36H11 without "families".
        img.draw_rectangle(tag.rect(), color = 127)
        img.draw_cross(tag.cx(), tag.cy(), color = 127)
        print_args = (family_name(tag), tag.id(), (180 * tag.rotation()) / math.pi)
        #print("Tag Family %s, Tag ID %d, rotation %f (degrees)" % print_args)
        if tag.id() == 7:
            print("Turn Left")
            data = 'left\n'
        if tag.id() == 2:
            print("Onward")
            data = 'forward\n'r
        if tag.id() == 4:
            print("Halt")
            data = 'stop\n'
        #print(print_args)
    try:
        bus.send(ustruct.pack("<h", len(data)), timeout=10000)
        try:
            bus.send(data, timeout=10000)
            #print("Sent Data!")
            #data = "...\n"
        except OSError as err:
            pass
    except OSError as err:
        pass