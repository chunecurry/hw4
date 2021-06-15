import pyb, sensor, image, time, math

enable_lens_corr = False # turn on for straighter lines...
sensor.reset()
sensor.set_pixformat(sensor.RGB565) # grayscale is faster
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 2000)
clock = time.clock()

f_x = (2.8 / 3.984) * 160 # find_apriltags defaults to this if not set
f_y = (2.8 / 2.952) * 120 # find_apriltags defaults to this if not set
c_x = 160 * 0.5 # find_apriltags defaults to this if not set (the image.w * 0.5)
c_y = 120 * 0.5 # find_apriltags defaults to this if not set (the image.h * 0.5)

def degrees(radians):
   return (180 * radians) / math.pi

uart = pyb.UART(3,9600,timeout_char=1000)
uart.init(9600,bits=8,parity = None, stop=1, timeout_char=1000)

while(True):
   clock.tick()
   img = sensor.snapshot()
   if enable_lens_corr: img.lens_corr(1.8) # for 2.8mm lens...

   # `merge_distance` controls the merging of nearby lines. At 0 (the default), no
   # merging is done. At 1, any line 1 pixel away from another is merged... and so
   # on as you increase this value. You may wish to merge lines as line segment
   # detection produces a lot of line segment results.

# `max_theta_diff` controls the maximum amount of rotation difference between
   # any two lines about to be merged. The default setting allows for 15 degrees.
   for l in img.find_line_segments(merge_distance = 10, max_theta_diff = 5):
      if(l.length() > 10 and l.y1() < 50 and l.y2() < 50 and abs(l.y1()-l.y2()) > 9):
          if(l.y1() < 5 and l.x1() < 105 and l.x1() > 65 or l.y2() < 5 and l.x2() < 105 and l.x2() > 65):
              img.draw_line(l.line(), color = (255, 0, 0))
              print(1)
              #print(l.line())
              uart.write(("1").encode())
      #uart.write(("0").encode())
   print("end\n")
   #uart.write(("FPS %f\r\n" % clock.fps()).encode())
