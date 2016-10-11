[Python标准库]os——可移植访问操作系统特定特性
    作用：可移植访问操作系统特定特性。
    Python 版本：1.4 及以后版本
    os 模块为平台特定的模块（如 posix、nt 和 mac）提供了一个包装器。所有平台上函数的 API 都是相同的，所以使用 os 模块可以提供一定的可移植性。不过，并不是所有函数在每一个平台上都可用。这个总结中介绍的许多进程管理函数就对 Windows 不适用。
    os 模块的 Python 文档的子标题是“杂类操作系统接口”。这个模块主要包括创建和管理运行进程或文件系统内容（文件和目录）的函数，只有很少涉及其他功能。
进程所有者
    os 提供的第一组函数用于确定和改变进程所有者 id。守护进程或一些特殊系统程序（需要改变权限级别而不是作为 root 运行）的作者最常使用这些函数。
    下面的例子显示了一个进程的实际有效的用户和组信息，然后改变这些有效值。这类似于系统自引导期间一个守护进程作为 root 启动时所要做的工作，以降低权限等级，作为一个不同的用户运行。

import os

TEST_GID = 1000
TEST_UID = 1000

def show_user_info():
	print 'User (actual/effective)  : %d / %d' % (os.getuid(), os.geteuid())
	print 'Group (actual/effective) : %d / %d' % (os.getgid(), os.getegid())
	print 'Actual Groups  :', os.getgroups()
	return

print 'BEFORE CHANGE:'
show_user_info()
print

try:
	os.setegid(TEST_GID)
except OSError:
	print 'ERROR: Could not change effective group.  Rerun as root.'
else:
	print 'CHANGED GROUP:'
	show_user_info()
	print

try:
	os.seteuid(TEST_UID)
except OSError:
	print 'ERROR: Could not change effective user.  Rerun as root.'
else:
	print 'CHANGE USER:'
	show_user_info()
	print

    在 Ubuntu 上，作为 id 为 1000、group 为 1000 的用户运行时，会生成以下输出。

$ python os_process_user_example.py 
BEFORE CHANGE:
User (actual/effective)  : 1000 / 1000
Group (actual/effective) : 1000 / 1000
Actual Groups  : [4, 24, 27, 30, 46, 108, 124, 1000]

CHANGED GROUP:
User (actual/effective)  : 1000 / 1000
Group (actual/effective) : 1000 / 1000
Actual Groups  : [4, 24, 27, 30, 46, 108, 124, 1000]

CHANGE USER:
User (actual/effective)  : 1000 / 1000
Group (actual/effective) : 1000 / 1000
Actual Groups  : [4, 24, 27, 30, 46, 108, 124, 1000]

    这些值不会改变，因为只要不作为 root 运行，进程就不能改变其有效所有者值。试图将有效用户 id 或 组 id 设置为非当前用户的其他值时，会导致一个 OSError。使用 sudo 运行同样的脚本，使它启动时有 root 权限，其结果则会完全不同。

$ sudo python os_process_user_example.py 
BEFORE CHANGE:
User (actual/effective)  : 0 / 0
Group (actual/effective) : 0 / 0
Actual Groups  : [0]

CHANGED GROUP:
User (actual/effective)  : 0 / 0
Group (actual/effective) : 0 / 100
Actual Groups  : [0]

CHANGE USER:
User (actual/effective)  : 0 / 100
Group (actual/effective) : 0 / 100
Actual Groups  : [0]

    在这种情况下，由于它作为 root 启动，脚本可以改变进程的有效用户和组。一旦改变了有效 UID，进程则限于该用户的权限。由于非根用户不能改变其有效组，程序在改变用户之前需要先改变组。
进程环境

