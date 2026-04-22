# AT Command Parser Makefile

CC = gcc
RAGEL = ragel
DOT = dot

TARGET = a.out
SRC_RL = main.rl
SRC_C = main.c
DOT_FILE = main.dot
GRAPH_PNG = main.png

.PHONY: all clean graph run

all: $(TARGET)

# 从 .rl 生成 .c
$(SRC_C): $(SRC_RL)
	$(RAGEL) $(SRC_RL) -o $(SRC_C)

# 编译生成可执行文件
$(TARGET): $(SRC_C)
	$(CC) $(SRC_C) -o $(TARGET)

# 使用 Graphviz 生成状态机图
graph: $(SRC_RL)
	$(RAGEL) -V $(SRC_RL) > $(DOT_FILE)
	$(DOT) -Tpng $(DOT_FILE) -o $(GRAPH_PNG)
	@echo "Graph generated at $(GRAPH_PNG)"

# 运行程序
run: $(TARGET)
	./$(TARGET)

# 清理生成的文件
clean:
	rm -f $(TARGET) $(SRC_C) $(DOT_FILE) $(GRAPH_PNG)
	@echo "Cleaned up generated files."
