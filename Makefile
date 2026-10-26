fishing:
	test -f fishingrc && rm fishingc || true
	cd ./audio-player && make fclean
	cd ./audio-player && make
	gcc -Wall -Wextra main.c utils.c menu.c map.c fish.c profile.c -o fishingc -L. -lsimpleaudio -lmpg123 -lao -lpthread -lm
