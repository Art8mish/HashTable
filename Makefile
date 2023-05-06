CC = g++

TARGET = hash_tbl

CFLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations \
         -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual	      \
         -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op                 \
         -Wmissing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192         \
         -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_SIDE				          \

PREF_TBL_SRC = ./src/
PREF_LST_SRC = ./list_func/src/

PREF_OBJ = ./obj/

SRC_TBL = $(wildcard $(PREF_TBL_SRC)*.cpp)
SRC_LST = $(wildcard $(PREF_LST_SRC)*.cpp)
SRC_ASM = $(wildcard $(PREF_TBL_SRC)*.asm)

OBJ_TBL = $(patsubst $(PREF_TBL_SRC)%.cpp, $(PREF_OBJ)%.o, $(SRC_TBL))
OBJ_LST = $(patsubst $(PREF_LST_SRC)%.cpp, $(PREF_OBJ)%.o, $(SRC_LST))
OBJ_ASM = $(patsubst $(PREF_TBL_SRC)%.asm, $(PREF_OBJ)%.o, $(SRC_ASM))

OBJ = $(OBJ_TBL) $(OBJ_LST) $(OBJ_ASM) 

all : $(TARGET)

$(TARGET) : $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

$(PREF_OBJ)%.o : $(PREF_TBL_SRC)%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(PREF_OBJ)%.o : $(PREF_LST_SRC)%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(PREF_OBJ)%.o : $(PREF_TBL_SRC)%.asm
	nasm -f elf64 -F dwarf -o $@ $<

clean :
	rm $(TARGET).exe $(PREF_OBJ)*.o
