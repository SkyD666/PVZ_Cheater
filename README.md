# Plants Vs Zombies Cheater

## 主要原理

1. 直接修改内存变量数值
2. 修改游戏内存代码，如**add** [eax+00005560], ecx修改为**sub** [eax+00005560], ecx或**nop**实现**由减数值**变为**加数值或**数值**不变**
3. 在游戏空白地址出写入代码，使程序跳转执行新代码
4. 通过Windows API **FindWindow** 、**GetWindowThreadProcessId** 、**OpenProcess** 等获取游戏进程信息
5. 通过Windows API **ReadProcessMemory**读取游戏内存，找到多级偏移指针
6. 通过Windows API **WriteProcessMemory**写入数值或代码到游戏进程。用十六进制表示汇编代码，如80 7B 50 00代表**cmp** byte ptr [ebx+50], 00

