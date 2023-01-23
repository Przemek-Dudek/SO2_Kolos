gcc -pthread client.c -o client -lrt && clear && ./client data_0007.txt test.txt

clear && gcc -pthread main.c -o main -lrt && ./main