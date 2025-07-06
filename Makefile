CXX = g++
CXXFLAGS = -Iinclude -Wall -O2
LDFLAGS = -Llib -Wl,--allow-multiple-definition -Wl,--whole-archive -ltinyfiledialogs -Wl,--no-whole-archive -lraylib -lopengl32 -lgdi32 -lwinmm -luser32 -lole32 -lcomdlg32 -Wl,-subsystem,windows
TARGET = game.exe
SRCS = main.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

clean:
	del $(TARGET)

run: $(TARGET)
	./$(TARGET)