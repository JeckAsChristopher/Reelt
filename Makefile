.PHONY: all build clean libs

GREEN  := \033[0;32m
YELLOW := \033[1;33m
RED    := \033[0;31m
NC     := \033[0m

all: build

libs:
	@{ \
		if [ -d "libs/CLI11" ] || [ -d "libs/capstone" ] || [ -d "libs/ELFIO" ]; then \
			echo -e "$(YELLOW)==> Libraries already exist.$(NC)"; \
			read -p "==> You need 500 MB disk space to reinstall this. Do you want to continue? (y/n): " ans; \
			if [ "$$ans" != "y" ]; then \
				echo -e "$(GREEN)==> Skipping reinstallation of libraries.$(NC)"; \
				exit 0; \
			fi; \
		else \
			read -p "==> You need 500 MB disk space to install this. Do you want to continue? (y/n): " ans; \
			if [ "$$ans" != "y" ]; then \
				echo -e "$(GREEN)==> Skipping library installation.$(NC)"; \
				exit 0; \
			fi; \
		fi; \
		available_space=$$(df . | awk 'NR==2 {print $$4}'); \
		required_kb=512000; \
		if [ "$$available_space" -lt "$$required_kb" ]; then \
			echo -e "$(RED)==> Not enough disk space. You need at least 500 MB to continue.$(NC)"; \
			exit 1; \
		fi; \
		echo -e "$(YELLOW)==> Preparing external libraries...$(NC)"; \
		mkdir -p libs; \
		cd libs && \
		rm -rf CLI11 capstone ELFIO && \
		echo -e "$(YELLOW)==> Cloning CLI11...$(NC)" && \
		git clone https://github.com/CLIUtils/CLI11.git && \
		echo -e "$(YELLOW)==> Cloning capstone...$(NC)" && \
		git clone https://github.com/capstone-engine/capstone.git && \
		echo -e "$(YELLOW)==> Cloning ELFIO...$(NC)" && \
		git clone https://github.com/serge1/ELFIO.git; \
		echo -e "$(YELLOW)==> Building capstone using CMake...$(NC)"; \
		mkdir -p libs/capstone/build; \
		cd libs/capstone/build && cmake .. && cmake --build .; \
		echo -e "$(GREEN)==> Libraries ready!$(NC)"; \
	}

build: libs
	@echo -e "$(YELLOW)==> Creating build directory...$(NC)"
	@mkdir -p build
	@echo -e "$(YELLOW)==> Running CMake configure...$(NC)"
	@cd build && cmake ..
	@echo -e "$(Green)==> Building Main Version: v0.3"
	@echo -e "$(YELLOW)==> Building project...$(NC)"
	@cd build && cmake --build .
	@echo -e "$(GREEN)==> Build completed successfully!$(NC)"

clean:
	@echo -e "$(RED)==> Cleaning build directory...$(NC)"
	@rm -rf build
	@echo -e "$(GREEN)==> Clean completed!$(NC)"
