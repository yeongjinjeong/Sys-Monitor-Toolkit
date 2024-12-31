CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./Process/include
LDFLAGS = -pthread

SRC_DIR = Process/src
INC_DIR = Process/include
BUILD_DIR = build
BIN_DIR = bin

# 소스 파일 목록
SRCS = $(wildcard $(SRC_DIR)/*.cpp) \
       main.cpp

# Object 파일 목록
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

# 실행 파일
TARGET = $(BIN_DIR)/system_manager

# 기본 타겟
all: directories $(TARGET)

# 디렉토리 생성
directories:
	@mkdir -p $(BUILD_DIR)/$(SRC_DIR)
	@mkdir -p $(BIN_DIR)

# 실행 파일 생성
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Object 파일 생성 규칙
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 청소
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all directories clean
