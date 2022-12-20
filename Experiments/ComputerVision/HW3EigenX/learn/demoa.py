"""
in_project:     HW3EigenX
file_name:      demoa.py
create_by:      mrrai
create_time:    2022/12/18 20:59
description:    
"""
import torch
import torchvision
from matplotlib import pyplot as plt
from torchvision import transforms
from torch.utils import data
import cv2
fashion_root = "C:/Users/mrrai/Desktop/PostGraduate/MachineLearning/ML-JN-WuEnDa/LiMuPytorch/code/data"
res_data_dir = "../dress/"
def get_fashion_mnist_labels(labels):  #@save
    """返回Fashion-MNIST数据集的文本标签"""
    text_labels = ['t-shirt', 'trouser', 'pullover', 'dress', 'coat',
                   'sandal', 'shirt', 'sneaker', 'bag', 'ankle boot']
    return [text_labels[int(i)] for i in labels]

def show_images(imgs, num_rows, num_cols, titles=None, scale=1.5):  #@save
    """绘制图像列表"""
    figsize = (num_cols * scale, num_rows * scale)
    _, axes = plt.subplots(num_rows, num_cols, figsize=figsize)
    axes = axes.flatten()
    for i, (ax, img) in enumerate(zip(axes, imgs)):
        if torch.is_tensor(img):
            # 图片张量
            ax.imshow(img.numpy())
        else:
            # PIL图片
            ax.imshow(img)
        ax.axes.get_xaxis().set_visible(False)
        ax.axes.get_yaxis().set_visible(False)
        if titles:
            ax.set_title(titles[i])
    return axes

def get_dataloader_workers():  #@save
    """使用4个进程来读取数据"""
    return 4

def loadFashionMnist(batch_size, resize=None):
    trans = [transforms.ToTensor()]
    if resize:
        trans.insert(0, transforms.Resize(resize))
    trans = transforms.Compose(trans)
    mnist_train = torchvision.datasets.FashionMNIST(root=fashion_root, train=True, transform=trans)
    mnist_test = torchvision.datasets.FashionMNIST(root=fashion_root, train=False, transform=trans)
    return (data.DataLoader(mnist_train, batch_size, shuffle=True,
                            num_workers=get_dataloader_workers()),
            data.DataLoader(mnist_test, batch_size, shuffle=False,
                            num_workers=get_dataloader_workers()))


if __name__ == '__main__':
    batch_size = 100
    img_size = 28
    dataset_len = 400
    j = 0
    train_iter, test_iter = loadFashionMnist(batch_size=batch_size)
    for X, y in train_iter:
        X = X.reshape(batch_size, img_size, img_size)
        y = y.reshape(-1)
        for i in range(batch_size):
            if y[i] == 3:
                fln = str(j).zfill(4)+".jpg"
                cv2.imwrite(res_data_dir+fln, 255*X[i].numpy())
                j = j+1
                if j >= dataset_len:
                    break
        if j >= dataset_len:
            break

