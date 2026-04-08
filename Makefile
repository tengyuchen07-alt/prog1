# Gemini:此檔案可以省去輸入編譯指令的麻煩，直接使用 make run 指令即可編譯+執行程式。
# 定義編譯器
CXX = g++

# 定義編譯選項
CXXFLAGS = -Wall -lrt

# 目標檔名 (我們上一回合改名叫 main 了)
TARGET = main

# 預設執行的規則 (通常放在最上面，輸入 make 會跑這個)
all: $(TARGET)

# 編譯規則
$(TARGET): prog1.cpp
	$(CXX) prog1.cpp -o $(TARGET) $(CXXFLAGS)

# ========== 新增這個神技 ==========
# 這是「先編譯，後執行」的規則
run: $(TARGET)
	./$(TARGET) 1 5
# ==================================

# 清除編譯產生的檔案
clean:
	rm -f $(TARGET)