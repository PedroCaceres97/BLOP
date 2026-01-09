:: gcc -O3 -g -I.. log.c  -o log.exe
:: gcc -O3 -g -I.. ansi.c -o ansi.exe
:: gcc -O3 -g -I.. list.c -o list.exe
:: gcc -O3 -g -I.. pool.c -o pool.exe
:: gcc -O3 -g -I.. vector.c -o vector.exe
gcc -O3 -g -I.. -IC:/Dev/Libs/cJSON-1.7.19 -IC:/Dev/Libs/curl-8.17.0_5-win64-mingw/include -LC:/Dev/Libs/curl-8.17.0_5-win64-mingw/lib openai.c C:/Dev/Libs/cJSON-1.7.19/cJSON/cJSON.c -lcurl -o openai.exe