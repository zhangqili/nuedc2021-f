import sensor, image, time, math
from pyb import UART,LED
LED(3).on()
uart = UART(3, 115200, timeout_char=1000)
#u_start=bytearray([0xb3,0xb3])
#u_over=bytearray([0x0d,0x0a])
GRAYSCALE_THRESHOLD = [(63, 22, 9, 72, 3, 66)]#巡线的阈值


#摄像头设置
sensor.reset()
sensor.set_contrast(1)
sensor.set_pixformat(sensor.RGB565)
#sensor.set_pixformat(sensor.GRAYSCALE) # use grayscale.
#设置图像色彩格式，有RGB565色彩图和GRAYSCALE灰度图两种
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(30)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
clock = time.clock()

uart = UART(3,115200)   #定义串口3变量
uart.init(115200, bits=8, parity=None, stop=1) # init with given parameters

#识别区域
rois =     [(0, 0, 30, 120),        #  左  x y w h
            (130,0,30,120), ]       #  右  x y w h

ROIS = [ # [ROI, weight]
        (30, 100, 100, 20, 0.8), # 你需要为你的应用程序调整权重
        (30, 50, 100, 20, 0.3), # 取决于你的机器人是如何设置的。
        (30, 000, 100, 20, 0.1)
       ]


cross_star=[0,0]
cross_end=[0,0]
cross_finish=[0,0]
cross_finish1=[0,0]
stop_flag = False
angle = 0
n_1=[0,0]  #两边有色块的帧数
n_2=[0,0]  #两边无色块的帧数

def find_cross():
    global cross_finish
    global cross_star
    global cross_end
    #cross_end=[0,0]
    #cross_star=[0,0]
    global n
    #cross_finish=[0,0]
    left_right=[0,0]
    for i in range(2):
        blobs = img.find_blobs(GRAYSCALE_THRESHOLD, roi=rois[i][0:4], x_stride=25,merge=True,area_threshold=100,margin=3)
        if blobs:
            n_2[i]=0
            for blob in blobs:
                img.draw_rectangle(blob.rect())
            if (not cross_star[i]):
                n_1[i] += 1
            if (n_1[i]==30):
                cross_star[i]=1
                n_1[i]=0                          #两侧超过30帧有，则判断什么时候没有
        else:
            n_1[i]=0
            if cross_star[i]:
                n_2[i] += 1
            if (n_2[i]==10):
                cross_end[i]=1
                n_2[i]=0                          #超过10帧没有，则表示通过十字
        if(cross_star[i] and cross_end[i]):
            cross_star[i]=0
            cross_end[i]=0
            cross_finish[i] += 1



def car_run():
    global stop_flag
    global angle
    stop_flag=False
    angle=0
    blobs_center=[0,0,0]  #存放左边色块的中心cx值用于计算偏移角
    blobs_flag=[1,1,1]
    centroid_sum = 0
    weight_sum = 0
    for r in range(3):  #三个区域分别寻找色块
        max_blob = None
        max_area = 0
        blobs = img.find_blobs(GRAYSCALE_THRESHOLD, roi=ROIS[r][0:4], merge=True,area_threshold=100,margin=3)
        if blobs:
            for blob in blobs:
                if blob.area() > max_area:  #寻找色块最大色块
                    max_blob = blob
                    max_area = blob.area()
            img.draw_rectangle(max_blob.rect())
            blobs_center[r]=max_blob.cx()
            centroid_sum += blobs_center[r]* ROIS[r][4] #乘权值
            weight_sum += ROIS[r][4]
        else:
            blobs_flag[r]=0
    if (blobs_flag[0] or blobs_flag[1] or blobs_flag[2]):
        angle=math.degrees(math.atan(((centroid_sum / weight_sum)-80)/60))
    else:
        stop_flag = True

while(True):
    cross_flag = False
    stop_buf = 0
    times=0
    clock.tick()
    #img = sensor.snapshot().lens_corr(strength = 1.8, zoom = 1.0)#不断拍照，进行鱼眼校正
    img = sensor.snapshot()
    img.draw_rectangle(rois[0])
    img.draw_rectangle(rois[1])
    car_run()
    find_cross()
    if(cross_finish1[0]!=cross_finish[0] or cross_finish1[1]!=cross_finish1[1]):
        stop_buf = 1
        cross_flag = True
    else:
        stop_buf = 0
    if(stop_flag):
        stop_buf = 2
    cross_finish1[0]=cross_finish[0]
    cross_finish1[1]=cross_finish1[1]
    print('stop_flag = '+str(stop_flag))
    print('angle = '+str(int(angle)))
    print('cross_star = '+str(cross_star))
    print('cross_end = '+str(cross_end))
    print('cross_finish = '+str(cross_finish))
    print('n_1 = '+str(n_1))
    print('n_2 = '+str(n_2))
    print('stop_buf = '+str(stop_buf))
    output_str="\x00%c\x01%c\x05" % (int(angle), stop_buf)
    #uart.write(u_start)
    uart.write(output_str)
    #time.sleep_ms(10)
