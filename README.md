# HyperRender
## Features
* 模糊算法

    |     Algorithm      | Supported |
    | :----------------: | :-------: |
    | `Dual Kawase Blur` |     ✅     |





## Shader原理

### 浮雕

<img src="https://picture-1256967270.cos.ap-guangzhou.myqcloud.com/images/dabae3a78212cea951b2b463921a7e3d.png" alt="Alt" style="zoom:67%;" />

**太阳方位角（Azimuth）：**以目标物的正北方向为起算方向，即0度，其取值范围在0-360度，计算旋转方式为以目标物为轴心，以目标物的北方向为起始点，按顺时针方向旋转一周，方位角逐步增大至360°；因此太阳方位角一般是以目标物的北方向为起始方向，以太阳光的入射方向为终止方向，按顺时针方向所测量的角度

**太阳高度角（Elevation）：**太阳光的入射方向和地平面之间的夹角

**原理**

1. RGB转成亮度

2. 利用光照方向向量公式

    ```math
    L = (X,Y,Z)=(cos(azimuth)cos(elevation),
         sin(azimuth)cos(elevation),
         sin(elevation))
    ```

3. 利用卷积核将亮度转换成法线方向向量

    ```
    Nx =
    [ +1   0  -1 ]
    [ +1   0  -1 ]
    [ +1   0  -1 ]
    
    Ny =
    [ +1  +1  +1 ]
    [  0   0   0 ]
    [ -1  -1  -1 ]
    ```

4. 利用点积公式，dot(法线，光线)，所得的结果为负数就是背阴面，为正就是正面

### 漩涡

本质就是对uv进行变换，然后进行采样，因为是漩涡圆形，对直角坐标的uv进行旋转变换比较不直观，故先转成极坐标，然后改变角度后再转回直角坐标即可

![img](https://picture-1256967270.cos.ap-guangzhou.myqcloud.com/images/v2-a0d640eb0a53110f4d8d593c57f919ef_1440w.jpg)

其中，极坐标转化成直角坐标：

$$x=rcos\theta$$

$$y=rsin\theta$$

直角坐标转化为极坐标：

$$r=\sqrt{x^2+y^2}$$

$$\theta=arctan(\frac{y}{x})$$
