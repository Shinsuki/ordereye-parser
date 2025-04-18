CC = gcc
CFLAGS = -g -Icli_utils -Iparse_utils
SRCS = ordereye-parser.c cli_utils/cli_utils.c parse_utils/parse_utils.c
OUT = ordereye-parser

all: $(OUT)

$(OUT): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(OUT)

clean:
	rm -f $(OUT)

