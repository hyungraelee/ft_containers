CC = clang++
CFLAGS = -Wall -Wextra -Werror -std=c++98

INC_DIR = ./includes
OWN_TEST_DIR = ./tester/OwnTester
OWN_TEST_OBJS_DIR = ./tester/OwnTester/objs
OWN_TEST_LOG_DIR = ./tester/OwnTester/log
MLI_TEST_DIR = ./tester/mli42Tester
STD_NAMESPACE = std
FT_NAMESPACE = ft
CONT = vector
TIME = time
RM = rm -f

ifeq ($(TESTED_NAMESPACE),)
TESTED_NAMESPACE = ft
endif

all :
	@make start
	@make own
	@make mli

start :
	@$(OWN_TEST_DIR)/mytest.sh

own :
	@make vector
	@make stack
	@make map

mli :
	@cd $(MLI_TEST_DIR) && ./do.sh vector stack map

bonus :
	@make set
	@cd $(MLI_TEST_DIR) && ./do.sh set

vector :
	@make own_test CONT=vector

stack :
	@make own_test CONT=stack

map :
	@make own_test CONT=map

set :
	@make own_test CONT=set

own_test :
	@mkdir -p $(OWN_TEST_LOG_DIR)
	@$(CC) $(CFLAGS) $(OWN_TEST_DIR)/$(CONT).cpp -o $(CONT) -I$(INC_DIR) -DTESTED_NAMESPACE=$(FT_NAMESPACE)
	@./$(CONT) > $(OWN_TEST_LOG_DIR)/$(FT_NAMESPACE)_$(CONT)
	@$(CC) $(CFLAGS) $(OWN_TEST_DIR)/$(CONT).cpp -o $(CONT) -I$(INC_DIR) -DTESTED_NAMESPACE=$(STD_NAMESPACE)
	@./$(CONT) > $(OWN_TEST_LOG_DIR)/$(STD_NAMESPACE)_$(CONT)
	@diff $(OWN_TEST_LOG_DIR)/$(STD_NAMESPACE)_$(CONT) $(OWN_TEST_LOG_DIR)/$(FT_NAMESPACE)_$(CONT)
	@rm $(CONT)
	@$(OWN_TEST_DIR)/compare.sh $(CONT)

time :
	@make time_unit CONT=vector
	@make time_unit CONT=stack
	@make time_unit CONT=map
	@make time_unit CONT=set

time_unit :
	@$(CC) $(CFLAGS) $(OWN_TEST_DIR)/$(CONT).cpp -o $(CONT) -I$(INC_DIR) -DTESTED_NAMESPACE=$(FT_NAMESPACE)
	@printf "\n=====\t$(CONT)\t====="
	@printf "\ntime 'FT'"
	@$(TIME) ./$(CONT) > $(OWN_TEST_LOG_DIR)/$(FT_NAMESPACE)_$(CONT)
	@$(CC) $(CFLAGS) $(OWN_TEST_DIR)/$(CONT).cpp -o $(CONT) -I$(INC_DIR) -DTESTED_NAMESPACE=$(STD_NAMESPACE)
	@printf "time 'STD'"
	@$(TIME) ./$(CONT) > $(OWN_TEST_LOG_DIR)/$(STD_NAMESPACE)_$(CONT)
	@rm $(CONT)

clean :
	@$(RM) -r $(OWN_TEST_LOG_DIR)

fclean : clean

re : fclean all

.PHONY : all start own mli bonus own_test time time_unit clean fclean re
