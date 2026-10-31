all: fishing

fishing:
	test -f fishingc && rm fishingc || true
	test -f fishingc.exe && rm fishingc.exe || true
	cd ./audio-player && make fclean
	cd ./audio-player && make
	cp ./audio-player/libs/libsimpleaudio.a .
	gcc -Wall -Wextra main.c utils.c menu.c map.c fish.c profile.c score.c custom.c -o fishingc -L. -lsimpleaudio -lmpg123 -lao -lpthread -lm

brew:
	test -f fishingc && rm fishingc || true
	test -f fishingc.exe && rm fishingc.exe || true
	cd ./audio-player && make fclean
	cd ./audio-player && C_INCLUDE_PATH=/opt/homebrew/include make
	cp ./audio-player/libs/libsimpleaudio.a .
	gcc -Wall -Wextra main.c utils.c menu.c map.c fish.c profile.c score.c custom.c -o fishingc -L. -lsimpleaudio -L/opt/homebrew/lib -lmpg123 -lao -lpthread -lm