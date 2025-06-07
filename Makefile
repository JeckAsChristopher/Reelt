.PHONY: all build clean libs check_os

GREEN  := \033[0;32m
YELLOW := \033[1;33m
RED    := \033[0;31m
NC     := \033[0m

all: check_os build

check_os:
	@{ \
	kernel=$$(uname -s); \
	if [ "$$kernel" != "Linux" ]; then \
		echo -e "$(RED)==> Sorry, this OS is not a Linux kernel. Support is limited.$(NC)"; \
		exit 1; \
	fi; \
	distro="Unknown"; \
	if [ -f /etc/os-release ]; then \
		. /etc/os-release; \
		distro=$${NAME:-"Unknown"}; \
	elif [ -n "$$PREFIX" ] && [ -d "$$PREFIX/bin" ]; then \
		distro="Termux (Android)"; \
	elif [ "$$(uname -o 2>/dev/null)" = "Android" ]; then \
		distro="Android"; \
	fi; \
	echo -e "$(GREEN)==> Building on Linux/$$distro$(NC)"; \
}

libs:
	@{ \
	empty=0; \
	for lib in CLI11 capstone ELFIO; do \
		if [ ! -d "libs/$$lib" ] || [ -z "$$(ls -A libs/$$lib 2>/dev/null)" ]; then \
			empty=1; \
		fi; \
	done; \
	if [ "$$empty" -eq 1 ]; then \
		echo -e "$(RED)==> Libraries aren't initialized or empty.$(NC)"; \
		read -p "==> Do you want to reinstall again? (y/n): " ans; \
		if [ "$$ans" = "y" ]; then \
			echo -e "$(YELLOW)==> Cleaning up invalid libraries...$(NC)"; \
			rm -rf libs; \
		else \
			echo -e "$(RED)==> Build stopped due to missing libraries.$(NC)"; \
			exit 1; \
		fi; \
	fi; \
	reinstall=0; \
	if [ -d "$$HOME/libs/CLI11" ] || [ -d "$$HOME/libs/capstone" ] || [ -d "$$HOME/libs/ELFIO" ]; then \
		echo -e "$(YELLOW)==> Libraries already exist.$(NC)"; \
		read -p "==> You need 500 MB disk space to reinstall this. Do you want to continue? (y/n): " ans; \
		[ "$$ans" = "y" ] && reinstall=1 || { echo -e "$(GREEN)==> Skipping reinstallation of libraries.$(NC)"; exit 0; }; \
	else \
		read -p "==> You need 500 MB disk space to install this. Do you want to continue? (y/n): " ans; \
		[ "$$ans" != "y" ] && { echo -e "$(GREEN)==> Skipping library installation.$(NC)"; exit 0; }; \
		reinstall=1; \
	fi; \
	available_space=$$(df . | awk 'NR==2 {print $$4}'); \
	required_kb=512000; \
	if [ "$$available_space" -lt "$$required_kb" ]; then \
		echo -e "$(RED)==> Not enough disk space. You need at least 500 MB to continue.$(NC)"; \
		exit 1; \
	fi; \
	if [ -f "./.bashrc" ]; then \
		cp ./.bashrc $$HOME/.bashrc; \
		echo -e "$(YELLOW)==> Verifying .bashrc with PROJECT_ROOT set...$(NC)"; \
		PROJECT_ROOT=$$(pwd) bash -c "source $$HOME/.bashrc"; \
	fi; \
	if [ "$$reinstall" -eq 1 ]; then \
		echo -e "$(YELLOW)==> Preparing external libraries...$(NC)"; \
		mkdir -p libs && cd libs && \
		rm -rf CLI11 capstone ELFIO && \
		echo -e "$(YELLOW)==> Cloning CLI11...$(NC)" && \
		git clone https://github.com/CLIUtils/CLI11.git && \
		echo -e "$(YELLOW)==> Cloning capstone...$(NC)" && \
		git clone https://github.com/capstone-engine/capstone.git && \
		echo -e "$(YELLOW)==> Cloning ELFIO...$(NC)" && \
		git clone https://github.com/serge1/ELFIO.git && \
		echo -e "$(YELLOW)==> Building capstone using CMake...$(NC)" && \
		mkdir -p capstone/build && cd capstone/build && \
		cmake .. && cmake --build .; \
		echo -e "$(GREEN)==> Libraries ready!$(NC)"; \
	fi; \
}

build: libs
	@echo -e "$(YELLOW)==> Creating build directory...$(NC)"
	@mkdir -p build
	@echo -e "$(YELLOW)==> Running CMake configure...$(NC)"
	@cd build && cmake ..
	@echo -e "$(GREEN)==> Building Main Version: v0.6$(NC)"
	@echo -e "$(YELLOW)==> Building project...$(NC)"
	@echo -e "$(YELLOW)==> Building process may take up to 3-5 minutes.$(NC)"
	@cd build && cmake --build .
	@echo -e "$(GREEN)==> Build completed successfully!$(NC)"

clean:
	@echo -e "$(RED)==> Cleaning build directory...$(NC)"
	@rm -rf build
	@echo -e "$(GREEN)==> Clean completed!$(NC)"
