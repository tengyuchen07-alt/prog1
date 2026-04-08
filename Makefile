# 你的原本設定
CXX = g++
CXXFLAGS = -Wall -lrt
TARGET = main

all: $(TARGET)

$(TARGET): prog1.cpp
	$(CXX) prog1.cpp -o $(TARGET) $(CXXFLAGS)

# 預設執行
run: $(TARGET)
	./$(TARGET) 3 8

# === 👇 新增多組測試案例 ===

# 測試組 1：觀察正常對戰
test1: $(TARGET)
	./$(TARGET) 1 2

# 測試組 2：觀察不同種子
test2: $(TARGET)
	./$(TARGET) 7 4

# 測試組 3：極端測試 (邊界值 10)
test3: $(TARGET)
	./$(TARGET) 10 10

# 自動跑完所有測試的指令
test-all: $(TARGET)
	@echo "=== 開始測試組 1 ==="
	./$(TARGET) 1 2
	@echo "\n=== 開始測試組 2 ==="
	./$(TARGET) 7 4
	@echo "\n=== 開始測試組 3 ==="
	./$(TARGET) 10 10

clean:
	rm -f $(TARGET)