TARGET = KeyBoardControl      #工程名
OBJ = $(patsubst %.c,%.o, $(wildcard ./*.c)) #替换当前目录下所有.c文件的文件名

CC = gcc               #编译器
RM = del               #删除命令 windows平台为del linux平台为 rm -rf 强制删除所有指定文件
DEL = *.exe *.o        #要删除的文件
#Linux 平台为 DEL = $(OBJ) $(wildcard ./*.exe)
#DEL = $(OBJ) $(wildcard ./*.exe)

make: clean $(TARGET)  #清除历史编译生成的文件，生成新文件
#编译文件
$(TARGET):$(OBJ)
		$(CC) $^ -o $@

%.o:%.c                        #编译为目标文件 
		$(CC) -c -g $<         

.PHONY:clean           #清除指定文件
clean:
		$(RM) $(DEL)

# 伪目标定义在上侧
show:
		echo $(wildcard ./*.c)
		echo $(patsubst %.c,%.o, $(wildcard ./*.c))
		echo $(DEBUG)
