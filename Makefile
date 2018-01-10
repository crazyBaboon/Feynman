OBJS = Source/main.c Source/energy_calculation.c Source/momentum_calculation.c Source/check_collisions.c Source/drawing_and_input_controls.c Source/update_motion.c

CC = clang

CFLAGS=-I.

LINKER_FLAGS = -lpthread -lGL -lGLU -lglut -lm

OBJ_NAME = Feynman_081

hellomake : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
