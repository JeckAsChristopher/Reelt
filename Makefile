.PHONY: all build clean

GREEN  := \033[0;32m
YELLOW := \033[1;33m
RED    := \033[0;31m
NC     := \033[0m  # No Color

build:
	@echo -e "$(YELLOW)==> Creating build directory...$(NC)"
	@mkdir -p build
	@echo -e "$(YELLOW)==> Running CMake configure...$(NC)"
	@cd build && cmake .. 
	@echo -e "$(YELLOW)==> Building project...$(NC)"
	@cd build && cmake --build .
	@echo -e "$(GREEN)==> Build completed successfully!$(NC)"

all: build

clean:
	@echo -e "$(RED)==> Cleaning build directory...$(NC)"
	@rm -rf build
	@echo -e "$(GREEN)==> Clean completed!$(NC)"
