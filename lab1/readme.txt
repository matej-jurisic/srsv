Detekcija akcija jedne tipke (desni Ctrl) preko /dev/input.
main1.c - radno cekanje, main2.c - prekidi (SIGIO, SIGALRM).

Akcije (odluka 1 s nakon zadnjeg otpustanja, osim jako dugog stiska):
- jedan ili vise kratkih stisaka (< 1 s), ispisuje se broj stisaka
- dugi stisak (>= 1 s)
- jako dugi stisak (>= 5 s), javlja se dok je tipka jos stisnuta

Pokretanje (isto za main2):
gcc main1.c -o main1
sudo ./main1 /dev/input/by-id/<tipkovnica>-event-kbd
