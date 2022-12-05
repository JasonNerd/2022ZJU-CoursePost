"""
in_project:     MyHW1
file_name:      featureM.py
create_by:      mrrai
create_time:    2022/12/4 18:59
description:    
"""
import cv2
import numpy
from matplotlib import pyplot as plt


def match(img1, img2):
    sift = cv2.SIFT_create()
    # find the key points and descriptors with SIFT
    kp1, des1 = sift.detectAndCompute(img1, None)
    kp2, des2 = sift.detectAndCompute(img2, None)
    # BFMatcher with default params
    bf = cv2.BFMatcher()
    matches = bf.knnMatch(des1, des2, k=2)
    # Apply ratio test
    good = []
    for m, n in matches:
        if m.distance < 0.75 * n.distance:
            good.append([m])
    # cv2.drawMatchesKnn expects list of lists as matches.
    return cv2.drawMatchesKnn(img1, kp1, img2, kp2, good, None, flags=2)


if __name__ == '__main__':
    img_dir = "../imgs/"
    hill1 = cv2.imread(img_dir + "1Hill.jpg")  # 300x400
    hill2 = cv2.imread(img_dir + "2Hill.jpg")  # 300x400
    h12m = match(hill1, hill2)
    cv2.imshow('match', h12m)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
