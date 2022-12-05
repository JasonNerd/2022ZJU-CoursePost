"""
in_project:     MyHW2
file_name:      stitch.py
create_by:      mrrai
create_time:    2022/12/4 18:34
description:    
"""
import cv2
import numpy as np
import os

src_root = "../imgs/"
res_root = "../res/"

def show(img):
    cv2.imshow("img", img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

def stitch_image(expID, img1Name, img2Name, resize):
    # 1. 读取图像
    img1 = cv2.imread(src_root + expID + "/" + img1Name)
    img2 = cv2.imread(src_root + expID + "/" + img2Name)
    img1 = cv2.resize(img1, resize)
    img2 = cv2.resize(img2, resize)
    # 2. 创建特征转换器，检测两幅图像的特征点
    sift = cv2.xfeatures2d.SIFT_create()
    k1, d1 = sift.detectAndCompute(img1, None)
    k2, d2 = sift.detectAndCompute(img2, None)
    # 3. 将特征点描绘出来并拼接
    img1kp = cv2.drawKeypoints(img1, k1, None)
    img2kp = cv2.drawKeypoints(img2, k2, None)
    step1 = np.hstack((img1kp, img2kp))
    # 4. 将第一步特征点描绘图像结果进行保存
    if not os.path.isdir(res_root + expID):
        os.makedirs(res_root+expID)
    cv2.imwrite(res_root+expID+"/"+"step1_"+img1Name+img2Name+"_featurePoints.jpg", step1)
    show(step1)
    ###############################################
    # 5. 创建特征匹配器并进行匹配
    bf = cv2.BFMatcher()
    matches = bf.knnMatch(d1, d2, k=2)
    # 6. 筛选有效匹配的特征点，绘制匹配图，并保存和显示
    verify_matches = []
    for m1, m2 in matches:
        if m1.distance < 0.8 * m2.distance:
            verify_matches.append(m1)
    step2 = cv2.drawMatchesKnn(img1, k1, img2, k2, [[m] for m in verify_matches], None, flags=2)
    cv2.imwrite(res_root + expID + "/" + "step2_" + img1Name + img2Name + "_featureMatches.jpg", step2)
    show(step2)
    # 7. 求解单应性矩阵，先提取img1和img2的特征描述子坐标点，接着转为合适的np.float32数组形式，最后予以求解
    print(len(verify_matches))
    img1_pts = [k1[m.queryIdx].pt for m in verify_matches]
    img2_pts = [k2[m.trainIdx].pt for m in verify_matches]
    img1_pts = np.array(img1_pts, dtype=np.float32).reshape(-1, 1, 2)
    img2_pts = np.array(img2_pts, dtype=np.float32).reshape(-1, 1, 2)
    homo, mask = cv2.findHomography(img1_pts, img2_pts, cv2.RANSAC, 5.0)
    ###############################################
    # 最后是创建一个大图，依据投影变换将两个图像缝到一起
    # 8. 先得到原始图的四个角点的坐标
    h1, w1 = img1.shape[:2]
    h2, w2 = img2.shape[:2]
    img1_angle = np.float32([[0, 0], [0, h1], [h1, w1], [w1, 0]]).reshape(-1, 1, 2)
    img2_angle = np.float32([[0, 0], [0, h2], [h2, w2], [w2, 0]]).reshape(-1, 1, 2)
    # 2. 根据单应性矩阵进行透视变换
    img1_transform = cv2.perspectiveTransform(img1_angle, homo)
    result_dims = np.concatenate((img2_angle, img1_transform), axis=0)
    # 3. 透视变换实现平移, 输出图像大小 (x_max - x_min, y_max - y_min)
    [x_min, y_min] = np.int32(result_dims.min(axis=0).ravel() - 0.5)  # 四舍五入
    [x_max, y_max] = np.int32(result_dims.max(axis=0).ravel() + 0.5)  # 四舍五入
    transform_array = np.array([[1, 0, -x_min],
                                [0, 1, -y_min],
                                [0, 0, 1]])
    result_img = cv2.warpPerspective(img1, transform_array.dot(homo), (x_max - x_min, y_max - y_min))
    result_img[-y_min:-y_min + h2, -x_min:-x_min + w2] = img2  # 将img2添加进平移后的图像
    cv2.imwrite(res_root + expID + "/" + "step3_" + img1Name + img2Name + "_Stitched.jpg", result_img)
    show(result_img)


if __name__ == '__main__':
    exp = "006"
    # stitch_image(exp, "SnowStreet1.JPG", "SnowStreet2.JPG", (1420, 1066))
    # stitch_image(exp, "Libruary1.JPG", "Libruary2.JPG", (850, 650))
    # stitch_image(exp, "laptop1.JPG", "laptop2.JPG", (800, 800))
    # stitch_image(exp, "snowman1.JPG", "snowman2.JPG", (720, 990))
    # stitch_image(exp, "pengye.png", "zhu.png", (420, 420))
    stitch_image(exp, "map.jpg", "map2.jpg", (500, 500))

