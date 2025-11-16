# Projekt 4 na PGK

Zrobione:
- Opisać efekty dodatkowe w pliku README
- Jakiś efekt na dolecenie do celu
- Ulepszenie efektu przy zderzeniu z przeszkodą
- dodać przy pomocy fragment shadera wzorki na przeszkodach (mają teraz kształt asteroidy)
- Poprawna implementacja FPS
- Dodatkowy widok rysować w trybie szkieletu z możliwością zmieniania trybu strzałkami
- Dwa widoki (TPS i FPS)
- Kontrola kamery myszką
- Kontrola gracza klawiaturą
- Parametry z lini poleceń
- Rysowanie przeszkód tak żeby można było odróżnić ich pozycje
- TPS powinien być głównym ekranem
- FPS powinien być dodatkowym ekranem
- Obsługa myszy
- poprawna zmiana proporcji widoków wypełniających okno przy zmianie rozmiaru okna (widoki powinny wypełniać jak najwięcej okna)
- na zewnętrznych ścianach sześcianu ograniczających labirynt dodać przy pomocy fragment shadera wzory ułatwiające orientacje obserwatora. (implementowane przez skybox)
- Latające przeszkody
- Kolizje z przeszkodami


## Opis jak działają efekty

### Asteroidy

Na końcowy wygląd asteroid składa się kilka komponentów
1. Mesh, generowany z nałożeniem Perlin noise na mesh kuli 
2. Oświetlenie specular + diffuse (model Phonga) z jednym źródłem
3. Powolny obrót plus latanie góra-dół według wykresu sinusa 

Do spawnowania dużej ilości asteroid i szybkiego przekazywania pamięci do GPU używam instancjonowania oraz buferów SSBO

### Cząsteczki

Podobnie jak przy asteroidach używam tutaj buferów SSBO i instancjonowania, ale przy pewnych kompromisach wysyłam jedynie pozycje i skale cząsteczki, dzieki czemu na dobrej maszynie jestem w stanie z bardzo dobrymi frame time'ami generować ich nawet w okolicach 100.000

Teoretycznie dałoby się bardzo przyśpieszyć gdybym całą logike przeniósł do compute shadera, ale jeszcze tego nie zrobiłem

### Sekwencja koncowa gry

Po dotarciu dostatecznie bisko do gwiazdy, wszystkie obiekty w grze są "wyłączane" a gwiazda robi efekt "wybuchu" 

### Kolizje

Prosty model kolizji gdzie mamy jeden dynamiczny obiekt (gracz), który może kolidować ze statycznymi obiektami (asteroidy). Z kolei obiekty statyczne zakładami że nigdy nie będą ze sobą kolidowały.

Kolizje obliczane są jako model kolizji kuli (statyczne obiekty) z prostpadłościanem (gracz)

### Kolizje z asterodiami

Kolizja z asterodią powoduje nałozenie na gracza efektu "odbicia", usunięcie obiektu asteroidy ze sceny oraz uruchomienie efektu wybuchu cząsteczek

### Skybox
Skybox jest po prostu kulą ze ścianami odwróconymi "do środka" wyskalowaną i śledzącą pozycje gracza, tak żeby sprawiała wrażenie "bardzo dalekiej" 

Pattern w skyboxie generowany jest za pomocą simplex noise 

### Format pliku .3d

Prosty format pliku wzorowany na plikach .obj zaimplementowany po to żeby łatwiej mi się zarządzało meshami