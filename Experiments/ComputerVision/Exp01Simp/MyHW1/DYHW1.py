import cv2
import numpy as np
from PIL import Image, ImageDraw, ImageFont
################################################
# 参数区(一般视频、帧参数)
DY_WIDTH = 1024  # 标准宽度
DY_HEIGHT = 618  # 标准高度
DY_BGCOLOR = 224  # 标准背景色
DY_FONTCOL = (255, 127, 63)  # 标准字体色
FPS = 30  # 标准视频帧率
TTW = int(1000/FPS)  # 标准帧时间间隔
# 参数区(输入输出参数)
in_jpg = "22221324.jpg"  # 个人照片
out_video = "22221324.mp4"  # 输出视频流容器
# 参数区(片头片尾文字参数)
info1 = "大家好我是段裕，学号22221324"  # 片头字幕
info2 = "感谢观看！@七彩的河制作"  # 片尾字幕
fontSize = 30  # 字体大小
# 雪人动画绘制参数(可调节)
DY_TOP_PAD = 50  # 上边距
DY_BOT_PAD = 28  # 下边距
DY_HEAD_SIZE = (100, 100)  # 头部大小: (横半轴长, 纵半轴长)
DY_HAT_HEIGHT = 60  # 帽子高度
DY_BODY_SIZE = (200, DY_HEIGHT-DY_TOP_PAD-DY_BOT_PAD-DY_HAT_HEIGHT-2*DY_HEAD_SIZE[1])  # 身体大小
DY_HEAD_CENTER = (int(DY_WIDTH/2), DY_TOP_PAD+DY_HAT_HEIGHT+DY_HEAD_SIZE[1])  # 头部中心
DY_BODY_CENTER = (int(DY_WIDTH/2), DY_HEIGHT-DY_BOT_PAD)  # 身体中心
HAT_TOP_P = (int(DY_WIDTH/2), DY_TOP_PAD) # 帽子顶部坐标
delta = np.sqrt(3)/2
HAT_SIDE1 = (HAT_TOP_P[0]-DY_HEAD_SIZE[1], DY_HEAD_CENTER[1]-int(DY_HEAD_SIZE[1]*delta))  # 帽沿坐标
HAT_SIDE2 = (HAT_TOP_P[0]+DY_HEAD_SIZE[1], DY_HEAD_CENTER[1]-int(DY_HEAD_SIZE[1]*delta))  # 帽沿坐标
DY_HAT_COL = (63, 63, 255)  # 帽子颜色
LocRate = 0.5  # 控制眼睛离中心的位置, 越小离中心越近，大于0小于1
eyeSize = (0.2, 0.24)  # 控制眼睛大小, 值越大眼睛越大，大于0小于1, 以下是眼睛大小与两个眼睛的中心位置
EYE_SIZE = (int(DY_HEAD_SIZE[0]*eyeSize[0]), int(DY_HEAD_SIZE[1]*eyeSize[1]))
DY_EYE1 = (DY_HEAD_CENTER[0]-int(DY_HEAD_SIZE[0]*LocRate), DY_HEAD_CENTER[1]-int(DY_HEAD_SIZE[1]*LocRate))
DY_EYE2 = (DY_HEAD_CENTER[0]+int(DY_HEAD_SIZE[0]*LocRate), DY_HEAD_CENTER[1]-int(DY_HEAD_SIZE[1]*LocRate))
mouthSize = (0.7, 0.5)  # 控制嘴巴扁平程度, 值越大越扁平，大于0小于1
mAg = 20  # 控制微笑的角度或者理解为嘴巴大小，值越大嘴巴越小，大于0小于1
DY_MOUTH_SIZE = (int(DY_HEAD_SIZE[0]*mouthSize[0]), int(DY_HEAD_SIZE[0]*mouthSize[1]))  # 嘴巴大小
################################################
# 以下为方法区
def init_img(h, w, c):
    # 返回一个BGR值均为c的高h宽w的图像矩阵
    return np.ones((h, w, 3), dtype=np.uint8)*c

def show(img):
    cv2.imshow("img", img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

def calFontPos(text):
    fontSTX = int((DY_WIDTH - fontSize*len(text))/2)
    fontSTY = int(DY_HEIGHT - 2*fontSize)
    return fontSTX, fontSTY

def standardize(img, w, h, c, hp):
    """
    :param img: 输入图像矩阵
    :param w: 标准宽度
    :param h: 标准高度
    :param c: 背景填充色
    :param hp: 下边距(主要是为字幕留空)
    :return: 经标准化后的图像矩阵
    """
    iw = img.shape[1]
    ih = img.shape[0]
    std = np.ones((h, w, 3), dtype=np.uint8)*c
    # 放缩
    if iw > w:  # 宽度超限
        ih = int(w*ih/iw)
        iw = w
        img = cv2.resize(img, (iw, ih))  # 注意为宽, 高
    if ih > h-hp:  # 高度超限
        iw = int((h-hp)*iw/ih)
        ih = h - hp
        img = cv2.resize(img, (iw, ih))
    # 填充
    sx = int((w-iw)/2)
    sy = int((h-hp-ih)/2)
    std[sy: sy+ih, sx: sx+iw, 0: 3] = img
    return std

def addCh(img, text, w, h, size, c):
    # 在img的下方中间处写上大小为size颜色为c的text
    img = Image.fromarray(img)
    draw = ImageDraw.Draw(img)
    font = ImageFont.truetype("stsong.ttf", size=size, encoding='utf-8')
    draw.text((w, h), text, c, font=font)
    img = np.array(img)
    return img

def getVideoHD():
    fourcc = cv2.VideoWriter_fourcc(*"mp4v")
    vhd = cv2.VideoWriter(out_video, fourcc, FPS, (DY_WIDTH, DY_HEIGHT))
    return vhd

def controller(name, frame, t):
    # 显示一帧, 或者暂停, 控制视频播放或者暂停
    cv2.imshow(name, frame)
    key = cv2.waitKey(t)
    if key == 32:  # 表示空格, 应该暂停
        key = cv2.waitKey(0)  # 暂停帧, 等待键盘输入
        if key == 32:  # 表示再次按下空格, 视频播放, 也即进入下一帧的处理
            return

def sceneTransfer(start, end, t, app_name, video_handler):
    # 从帧start到帧end的转换, 用时t秒, 场景在app_name中展现, 记录在video_handler中
    for i in np.linspace(0, 1, t * FPS):
        frame = cv2.addWeighted(start, 1 - i, end, i, 0)  # 生成一帧
        controller(app_name, frame, TTW)  # 显示一帧
        video_handler.write(frame)  # 写入一帧
    return frame

def handWriter(bg, text, t, app_name, video_handler):
    # 将text逐字符的放入帧bg底部, 显示到app_name上, 时长为t, 同时存入到video_handler中
    # 返回最终具有完整字符的frame
    sw, sh = calFontPos(text)
    t_each = int(1000 * t / len(text))  # 单字符ms数
    n_each = round(t_each / FPS)  # 单字符帧数
    tmp = ''
    for i in range(len(text)):
        tmp += text[i]
        frame = addCh(bg, tmp, sw, sh, fontSize, DY_FONTCOL)
        controller(app_name, frame, t_each)
        for j in range(n_each):
            video_handler.write(frame)
    return frame

def slowDraw(bg, center, size, ans, ane, c, w, t, app_name, video_handler):
    # 在bg上以center为中心画一个大小为size的椭圆, 起始角度ans, 终止角度ane, 绘制颜色c, 绘制线宽w
    # 持续时间t, 显示在app_name, 存储到video_handler上
    d_ang = (ane - ans) / (t * FPS)  # 每一帧应该 增加的角度
    f_tt = t * FPS  # 一共的帧数
    for i in range(f_tt):
        frame = cv2.ellipse(bg, center, size, 0, ans, ans+d_ang*i, c, w)
        controller(app_name, frame, TTW)  # 显示一帧
        video_handler.write(frame)  # 写入一帧
    return frame


################################################
if __name__ == '__main__':
    # 1. 片头, 照片配一段文字，要求先显示照片，效果为淡入, 然后逐个的显示文字，最后慢慢淡出
    canvas = init_img(DY_HEIGHT, DY_WIDTH, DY_BGCOLOR)
    post = standardize(cv2.imread(in_jpg), DY_WIDTH, DY_HEIGHT, DY_BGCOLOR, 3*fontSize)
    app = "22221324"
    vhd = getVideoHD()
    cw_post = sceneTransfer(canvas, post, 3, app, vhd)  # 照片淡入 3 s
    cw_p_text = handWriter(cw_post, info1, 9, app, vhd)  # 逐个显示文字 9 s
    canvas = sceneTransfer(cw_p_text, canvas, 3, app, vhd)  # 整体淡出 3 s
    # 2. 画一个类似于雪人的卡通形象, 它有眼睛和嘴巴，身体和头部，还有一个帽子
    # 难点在于各个部分比例的协调与坐标计算, 这些参数统一放进了全局参
    # 注意部分参数可调节，并非全部固定死，动画顺序可以自定，例如
    # 1. 缓缓画一个头部 2. 缓缓画一个身子 3. 浮出一个帽子 4. 缓缓画一双眼睛 5. 缓缓画上嘴巴
    cw_head = slowDraw(canvas, DY_HEAD_CENTER, DY_HEAD_SIZE, 0, 360, DY_FONTCOL, 2, 5, app, vhd)
    cw_body = slowDraw(cw_head, DY_BODY_CENTER, DY_BODY_SIZE, 180, 360, DY_FONTCOL, 3, 5, app, vhd)
    cw_hat = cw_body.copy()
    cv2.line(cw_hat, HAT_TOP_P, HAT_SIDE1, DY_HAT_COL, 4)
    cv2.line(cw_hat, HAT_TOP_P, HAT_SIDE2, DY_HAT_COL, 4)
    cw_hat = sceneTransfer(cw_body, cw_hat, 3, app, vhd)
    cw_eye = slowDraw(cw_hat, DY_EYE1, EYE_SIZE, 0, 360, DY_HAT_COL, -1, 2, app, vhd)
    cw_eye = slowDraw(cw_eye, DY_EYE2, EYE_SIZE, 0, 360, DY_HAT_COL, -1, 2, app, vhd)
    cw_mouth = slowDraw(cw_eye, DY_HEAD_CENTER, DY_MOUTH_SIZE, mAg, 180-mAg, DY_FONTCOL, 2, 3, app, vhd)
    # 淡出
    canvas = sceneTransfer(cw_mouth, init_img(DY_HEIGHT, DY_WIDTH, DY_BGCOLOR), 3, app, vhd)  # 整体淡出 3 s
    # 添加字幕
    end_with_text = handWriter(canvas, info2, 5, app, vhd)  # 逐个显示文字 5 s
    # 视频结束
    vhd.release()
    cv2.waitKey(0)
    cv2.destroyAllWindows()
