# GreedySnake
自动寻路贪吃蛇百分百吃满屏幕  
环境：vs2019（c++），EasyX图形库(安装地址https://easyx.cn/)  
为了阅读方便全部代码都放在snake.cpp  
在https://gitee.com/kexibsn/snakeai 基础上做了大幅度的修改，实现了百分百吃满屏幕  
效果试玩在snake.exe  
首先地图是矩形，没有障碍物  
核心代码解释：  
算法一：  
详见函数getNextCmd  
如果真蛇能找到食物（蛇头有到蛇尾路径）  
----派虚拟蛇去吃掉食物（bfs）  
----如果虚拟蛇吃完还能找到尾巴  
--------派真蛇真正去吃（只走一步）  
----如果虚拟蛇吃完找不到尾巴则“远离尾巴”  
如果真蛇找不到食物则也“远离尾巴”  
如果以上找不到下一步，则随机走安全的一步，要保证走完这一步后还能找到蛇尾  

上面的“远离尾巴”有两种策略，第一种是走完这步还能到蛇尾且离蛇尾最远的，这种情况剩下2个格时可能会陷入死循环  
所以第二种策略是从走完还能到蛇尾的格子中随机选一个并且格子里的空位不能重复出现  
默认采用策略一，当连续走了10x地图长x地图宽这么多步还没吃到食物时就用另一策略  
以上算法能跑满大部分情况，偶尔会有死循环  
确保蛇头到蛇尾有路径是因为这样蛇头追着蛇尾走能保证不死，而远离尾巴是为了使蛇头蛇尾间留出足够多的空格方便吃食物同时尽可能减少零散的格子  
但是这种算法不能严格证明能吃满，比较玄学  

算法二：  
哈密顿回路定义：由指定的起点前往指定的终点，途中经过所有其他节点且只经过一次  
我们只要找出一条这样的路径，然后无脑走这条路径就行。。  
若地图为偶x偶或者奇x偶，比如6x8或者7x8等等，我们可以用回溯法找到一条路径：从最左上格点每次递归严格按照右下左上的先后顺序遍历即可，详见函数HanMilTonCore2  
若地图为奇x奇，则哈密顿回路经过的格点数只能是奇x奇-1，也就是说我们要找两条哈密顿回路，这两条路都是差一个格子跑满地图，假设路径1差的是(1,3)，路径2差的是(2,2)，那么只需要当食物出现在这两个格点时才交换路径即可，详见函数HanMilTonCore1  
这种算法能百分百跑满

演示
算法一：https://live.csdn.net/v/280641
算法二：
