"""
in_project:     HW3EigenX
file_name:      eigenx.py
create_by:      mrrai
create_time:    2022/12/18 22:20
description:    图像重构，主要利用Eigenvalue(特征值分解)或称为PCA相关算法
图片集读取自FashionMnist数据集中dress分类下的400张图片，均为28x28大小的图片
"""
import matplotlib.pyplot as plt
import numpy as np
import os
from sklearn.decomposition import PCA
from sklearn.model_selection import train_test_split
# 读取root文件夹下所有的图片，每个图片都是一个28x28的矩阵，转为向量，返回图片vector列表
def img_read(root):
    img_ = []
    for filename in os.listdir(root):
        im_arr = plt.imread(root + "/" + filename, 0)   # 将图片以灰度模式读取
        img_.append(im_arr.flatten())
    return np.array(img_)

# pca模型fitting
def pca_fit(features, i):
    pca = PCA(n_components=i)  # 定义PCA模型保留i个主成分
    pca.fit(features)  # 参数fit
    primary = pca.transform(features)  # 得到降维后的数据
    return primary, pca.components_


if __name__ == '__main__':
    # 读取图片向量：400×(28*28) = 400 × 784
    img_vec_ = img_read("../dress")
    img_size = (28, 28)
    # 图片重构：平均图+特征向量×降维后的数据=重构图像
    avg = np.mean(img_vec_, axis=0).reshape(img_size)
    # 接下来依次递增主成分数量n_components, 步长n_step, 实验次数times
    times, n_components, n_step = 9, 10, 10
    x_p1, x_p2 = train_test_split(img_vec_, test_size=0.2)
    fig, axes = plt.subplots(3, int(times/3), figsize=(20, 10), subplot_kw={"xticks": [], "yticks": []})
    for i, ax in enumerate(axes.flat):
        fitted, v = pca_fit(x_p1, n_components)
        reconstructed = avg + np.dot(fitted[0], v).reshape(img_size)
        ax.imshow(reconstructed, cmap="gray")
        n_components = n_components + 10
    plt.show()

