CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
BUILD    := build

# Գտնում է բոլոր .cpp ֆայլերը
SOURCES  := $(shell find . -name '*.cpp' | sort)
# Ամեն ֆայլի համար ստեղծում է թիրախի անուն, օր. build/01_single_responsibility_good
TARGETS  := $(patsubst ./%,$(BUILD)/%,$(SOURCES:.cpp=))

.PHONY: all run clean

all: $(TARGETS)

$(BUILD)/%: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< -o $@

run: all
	@for bin in $(TARGETS); do \
		echo "=== $$bin ==="; \
		$$bin; \
		echo ""; \
	done

clean:
	rm -rf $(BUILD)
