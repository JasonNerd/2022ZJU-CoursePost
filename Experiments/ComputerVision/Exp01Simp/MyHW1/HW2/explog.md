# 实验记录
## [SIFT关键点提取](https://qiaoxs.blog.csdn.net/article/details/125849051)
```python
# SIFT Algorithm
def sift_algorithm(img_path):
    img = cv2.imread(img_path)
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    sift = cv2.SIFT_create()
    # sift.detect()作用：在图像中找到关键点。
    # 如果只想搜索图像的一部分，可以传递mask。
    # 每个关键点都是一个特殊的结构，它有许多属性，
    # 比如（x，y）坐标、有意义邻域的大小、指定其方向的角度、指定关键点强度的响应等。
    kp = sift.detect(gray, None)
    # 现在已经找到关键点，就可以使用sift.compute()根据关键点计算描述符。
    # 最终得到的kp是一个关键点元组，
    # des是一个形状（关键点数量）×128的numpy数组。
    kp, des = sift.compute(gray, kp)
    # 刚刚我们分两步，先找到关键点、再计算描述符。
    # 可以直接一步实现，即使用sift.detectAndCompute()方法。
    # kp, des = sift.detectAndCompute(gray, None)
    # cv2.drawKeypoints可视化关键点。
    # 可以传入flags参数，选择绘制带有大小与方向的更为丰富的关键点信息。
    # img = cv2.drawKeypoints(gray, kp, img)
    img = cv2.drawKeypoints(gray, kp, img, flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
```

## drawMatchesKnn()参数缺失
```python
img3 = cv2.drawMatchesKnn(img1,kp1,img2,kp2,good,None,flags=2)
```
## 主要参考链接
[OpenCV-Python教程:41.特征匹配](https://www.jianshu.com/p/ed57ee1056ab)

[案例-使用python实现基于opencv的图像拼接(合并)](https://huaweicloud.csdn.net/63806ae5dacf622b8df877ea.html)
