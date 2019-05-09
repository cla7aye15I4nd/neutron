目前前端还要实现的功能：

查找购买
查看我的车票
管理界面
完成与后端的交互
对车站名称自动补全*


# 9102年火车票大作业概览

## 人员构成

| 姓名   | 角色 |
| ------ | ---- |
| 于峥   | 组长 |
| 马浩博 | 核心 |
| 唐泽   | 核心 |
| 刘成锴 | 核心 |

## 思路

### 前端(于峥,马浩博)

大概使用1~2天的时间完成基本的功能界面

一些~~可能~~必然会用到的前端知识

bootstrap(css) : <https://v3.bootcss.com/>

javascript : <http://www.runoob.com/js/js-tutorial.html>

jquery : <http://www.runoob.com/jquery/jquery-tutorial.html>

html : <http://www.runoob.com/html/html-tutorial.html>

这么多教程并不是劝退项，因为前端并不打算对代码风格作任何要求，而且我们的网页并不需要太多复杂的功能。

所以总体上对于这些教程的正确使用方法是即学即用。先大致浏览了解这些东西的作用，做到打开一个网页能够读懂50%以上的代码即可。

### 后端

现在的想法是使用tornado或flask进行开发。

<http://docs.jinkan.org/docs/flask/>

<https://tornado-zh.readthedocs.io/zh/latest/>

我现在还是决定使用tornado,这是一个有趣的尝试。

以下为能想到的可以做的任务(带*的为可选任务):

#### 用户部分(于峥)

- [ ] 用户登陆
- [ ] 登陆验证码
- [ ] 邮箱验证
- [ ] 第三方验证
#### 功能部分(马浩博)
- [ ] 关键字排序查询火车票
- [ ] 地图系统(*)
- [ ] 支付宝支付(*)
- [ ] 聊天留言系统(*)

### 数据库(全体)

其实我很想写成下面这个东西的

<http://huili.github.io/>

但是可能难度比较大。

所以我们先完成以下这个任务。
- [ ] B树(刘成锴,唐泽)
- [ ] 页缓存管理(于峥,刘成锴,唐泽)
  说一下文件读写全部使用c的函数，不要用c++的哦！
### 特殊算法优化(全体)
这个到时候再具体分析吧
### How to use git?

<https://github.com/livoras/blog/issues/7>
