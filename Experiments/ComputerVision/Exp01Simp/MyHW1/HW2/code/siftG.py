"""
in_project:     MyHW1
file_name:      siftG.py
create_by:      mrrai
create_time:    2022/12/4 18:41
description:    
"""
import cv2
import numpy as np

def sift_alg(img_path):
    img = cv2.imread(img_path)
    sift = cv2.SIFT_create()
    kp = sift.detect(img, None)  # sift.detect()作用：在图像中找到关键点
    # 现在已经找到关键点，就可以使用sift.compute()根据关键点计算描述符。
    kp, des = sift.compute(img, kp)
    # 最终得到的kp是一个关键点元组，des是一个形状（关键点数量）×128的numpy数组
    img = cv2.drawKeypoints(img, kp, None)
    cv2.imshow("img", img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()


if __name__ == '__main__':
    sift_alg("../imgs/SnowStreet1.JPG")
