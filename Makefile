NAME    := scop
BUILD   := build

all: $(NAME)

$(NAME):
	@cmake -B $(BUILD) -DCMAKE_BUILD_TYPE=Release
	@cmake --build $(BUILD) -j
	@cp $(BUILD)/$(NAME) .

clean:
	@cmake --build $(BUILD) --target clean 2>/dev/null || true

fclean:
	@rm -rf $(BUILD) $(NAME)

re: fclean
	@$(MAKE) all

.PHONY: all $(NAME) clean fclean re
