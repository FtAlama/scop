NAME    := scop
BUILD   := build

all: $(NAME)

$(NAME):
	@echo "Default mode : Debug"
	@cmake -B $(BUILD) -DCMAKE_BUILD_TYPE=Debug
	@cmake --build $(BUILD) -j
	@cp $(BUILD)/$(NAME) .

release:
	@cmake -B $(BUILD) -DCMAKE_BUILD_TYPE=Release
	@cmake --build $(BUILD) -j
	@cp $(BUILD)/$(NAME) .

clean:
	@cmake --build $(BUILD) --target clean 2>/dev/null || true

fclean:
	@rm -rf $(BUILD) $(NAME)

re: fclean
	@$(MAKE) all

.PHONY: all $(NAME) clean fclean re releasdebuge
