# Single Color RGB565 Blob Tracking Example
#
# This example shows off single color RGB565 tracking using the OpenMV Cam.

import sensor, image, time, omv, math
import ArduinoCommunication as ac

#threshold_index = 1 # 0 for red, 1 for green, 2 for blue

# Color Tracking Thresholds (L Min, L Max, A Min, A Max, B Min, B Max)
# The below thresholds track in general red/green/blue things. You may wish to tune them...
thresholds = [(30, 100, -30, 5, 40, 70), # yellow --This one recognizes red way better than yellow
              (30, 100, 15, 50, 10, 50), # orange
              (30, 70, -20, 35, -70, -23), #dark blue
              (30, 100, 41, 65, -9, 55)] # red

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
clock = time.clock()

#Init i2c bus
com = ac.ArduinoComm()

#Makes sure the number is always 3 bytes for transmission
def formatNumberForTransmission(coord):
    coord = str(coord)
    digits = len(coord)
    if digits < 3:
        newNumber = '0' * (3-digits)
        newNumber = newNumber + coord
        return newNumber;
    return coord


tag_families = 0
tag_families |= image.TAG16H5 # comment out to disable this family
tag_families |= image.TAG25H7 # comment out to disable this family
tag_families |= image.TAG25H9 # comment out to disable this family
tag_families |= image.TAG36H10 # comment out to disable this family
tag_families |= image.TAG36H11 # comment out to disable this family (default family)
tag_families |= image.ARTOOLKIT # comment out to disable this family

#Convert april tag enum value to a string
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


sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.VGA) # we run out of memory if the resolution is much bigger...
# AprilTags works on a maximum of < 64K pixels.
if omv.board_type() == "H7": sensor.set_windowing((240, 240))
elif omv.board_type() == "M7": sensor.set_windowing((200, 200))
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False)  # must turn this off to prevent image washout...
sensor.set_auto_whitebal(False)  # must turn this off to prevent image washout...


# Only blobs that with more pixels than "pixel_threshold" and more area than "area_threshold" are
# returned by "find_blobs" below. Change "pixels_threshold" and "area_threshold" if you change the
# camera resolution. "merge=True" merges all overlapping blobs in the image.

#, thresholds[2], thresholds[3]
iteration = 0
blobThreshold = thresholds[3]
recognitionMode = "AprilTags"#Recognize color by default
lastAprilTag = None

aprilTagHeightCm = 6#Centimeters
pWidth = 125#Pixels
distCm = 30#Centimeters

focalLength = (pWidth * distCm)/aprilTagHeightCm

#Get the distance between the april tag and the camera
def getAprilTagDistance(aprilTagPixelHeight):
    return (aprilTagHeightCm * focalLength)/aprilTagPixelHeight;

def getAprilTagRotation(pixelWidth, pixelHeight, distanceToTag):
    #Info about triangle we using to get rotation angl
    aspectRatio = pixelWidth/pixelHeight
    apparentWidth = aprilTagHeightCm * aspectRatio

    angleTheta = math.atan2(apparentWidth, distanceToTag)

    sideD = distanceToTag
    angleD = (sideD/aprilTagHeightCm) * math.sin(angleTheta)
    angleD = min(angleD, 1)
    angleD = math.asin(angleD)

    rotation = 180 - math.degrees(angleD) - math.degrees(angleTheta)

    return 180 - rotation;

#Returns the x and y coordinate as a tuple
def getPositionRelativeToAprilTag(distanceFromTag, angleFromTag):
    hypotenuse = distanceFromTag

    sideX = math.cos(angleFromTag) * hypotenuse
    sideY = math.sin(angleFromTag) * hypotenuse

    return sideX, sideY;




while(True):
    clock.tick()
    img = sensor.snapshot()
    img.rotation_corr(z_rotation = 180)
    currentBlob = None
    currentAprilTag = None


    if recognitionMode == "Color":

        for blob in img.find_blobs([blobThreshold], pixels_threshold=500, area_threshold=200, merge=True):
            img.draw_rectangle(blob.rect())
            img.draw_cross(blob.cx(), blob.cy())

            #Determine which blob is the largest and use it for information to send to arduino
            if currentBlob == None:
                currentBlob = blob
            else:
                blobSize = blob.pixels()

                lastBlobSize = currentBlob.pixels()

                if blobSize > lastBlobSize:
                    currentBlob = blob
    elif recognitionMode == "AprilTags":
        for tag in img.find_apriltags(families=tag_families): # defaults to TAG36H11 without "families".
            img.draw_rectangle(tag.rect(), color = 127)
            img.draw_cross(tag.cx(), tag.cy(), color = 127)
            print_args = (family_name(tag), tag.id(), (180 * tag.rotation()) / math.pi)
            #print("Tag Family %s, Tag ID %d, rotation %f (degrees)" % print_args)

            currentAprilTag = tag
            height = tag.h()
            width = tag.w()
            distance = getAprilTagDistance(height)
            rotation = getAprilTagRotation(width, height, distance)
            xCoord, yCoord = getPositionRelativeToAprilTag(distance, rotation)

            print("Distance: " + str(distance))
            print("Rotation: " + str(rotation))
            print("Robot Coordinates: " + str(xCoord) + ", " + str(yCoord))
            #print(str(distance) + " cm")
            #print("Rotation: " + str(rotation) + " degrees")
            #if tag.id() == 7:
             #   print("Turn Left")
            #if tag.id() == 2:
            #    print("Onward")
            #if tag.id() == 4:
            #    print("Halt")

        #img.binary([thresholds[3]])




    ######COMMUNICATION#########
    if iteration == 2:
        cmd = com.getData()#Get the last command from the arduino
        #If arduino is checking for status of camera
        if cmd == b'_ok':
            com.sendData("ok")
        elif cmd == b'cbc':#Arduino requests Color Blobl Coordinate
            if currentBlob:
                x = formatNumberForTransmission(currentBlob.cx())
                y = formatNumberForTransmission(currentBlob.cy())
                xy = x+y
                com.sendData(xy)
            else:
                com.sendData("-1_-1_")
        elif cmd == b'rrb': #Recognize red blobs
            blobThreshold = thresholds[3]
            com.sendData("set")
        elif cmd == b'rbb': #Recognize Blue Blobs
            blobThreshold = thresholds[2]
            com.sendData("set")
        elif cmd == b'apr': #Recognize april tags
            recognitionMode = "AprilTags"
            sensor.reset()
            sensor.set_pixformat(sensor.GRAYSCALE)
            sensor.set_framesize(sensor.VGA) # we run out of memory if the resolution is much bigger...
            # AprilTags works on a maximum of < 64K pixels.
            if omv.board_type() == "H7": sensor.set_windowing((240, 240))
            elif omv.board_type() == "M7": sensor.set_windowing((200, 200))
            sensor.skip_frames(time = 2000)
            sensor.set_auto_gain(False)  # must turn this off to prevent image washout...
            sensor.set_auto_whitebal(False)  # must turn this off to prevent image washout...
            com.sendData("april");
        elif cmd == b'cat': #Get the data of the Current April Tag
            if currentAprilTag != None:
                com.sendData(formatNumberForTransmission(currentAprilTag.id()))
            else:
                com.sendData("-1_");
        elif cmd == b'apc': #Get the current april tag X and Y coordinate
            if currentAprilTag != None:
                x = currentAprilTag.cx()
                y = currentAprilTag.cy()
                xy = formatNumberForTransmission(x)+formatNumberForTransmission(y)
                print(xy)
                com.sendData(xy)
            else:
                com.sendData("-1_-1_")

        iteration = 0

    iteration += 1
