
all: compile

compile:
	mkdir -p ./bin
	gcc -O1 -Wall -Wextra -Wformat=2 -Wno-unused-parameter -o ./bin/realbay-cli src/*.c
	chmod +x ./bin/realbay-cli
	
clean:
	rm -rf ./bin
	mkdir ./bin
