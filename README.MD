### Make FC(NES) Emulator Step by Step
Re: 从零开始的红白机模拟

### 项目说明
同时拥有VS2017和CodeLite的项目工程以保证在不同编译器上能够成功编译.


### 列表

0. [第0步](./step0/): 加载NES ROM文件
   - 读取文件是优先的. 
   - 但是打好基础也是非常重要的, 接口是一个好东西
1. [第1步](./step1/): 实现基本的CPU读写功能
   - 需要了解CPU地址空间的布局哦
2. [第2步](./step2/): 6502汇编
   - 实现反汇编是调试不可缺少的一环
3. [第3步](./step3/): 实现CPU指令
   - 这一步比起前面就比较大了
   - 实现所有的CPU指令
4. [第4步](./step4/): 背景渲染
5. [第5步](./step5/): 基本输出
6. [第6步](./step6/): 精灵渲染
7. [第7步](./step7/): 实现中精度同步
8. [第8步](./step8/): 实现基本音频播放
9. [第9步](./step9/): 实现Mappper1234