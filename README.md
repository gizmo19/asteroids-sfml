# Dokumentacja Projektu Asteroids SFML

## Autorzy
- Mateusz Hajduczek
- Dawid Nachlik

## Opis Projektu
Asteroids SFML to nowoczesna implementacja klasycznej gry Asteroids, stworzona przy użyciu biblioteki SFML. Projekt wykorzystuje architekturę Scene-Controller-Actor z systemem komunikacji między obiektami (MessageBus), co zapewnia modularność i łatwość rozbudowy.

### Architektura
Projekt podzielono na kilka głównych komponentów:

1. **Core**
   - Główny kontroler zarządzający grą i scenami
   - Obsługa głównej pętli gry
   - Zarządzanie czasem gry

2. **Sceny**
   - MenuScene - scena początkowa
   - GameplayScene - główna scena rozgrywki
   - GameOverScene - ekran końca gry

3. **Aktorzy**
   - Ship - statek gracza
   - Asteroid - asteroidy
   - Bullet - pociski
   - WeaponPickup - bronie

4. **Kontrolery**
   - ShipController - sterowanie statkiem
   - AsteroidController - zarządzanie asteroidami
   - BulletController - logika pocisków
   - WeaponPickupController - zarządzanie power-upami
   - CollisionController - obsługa kolizji
   - ExplosionController - efekty eksplozji

5. **MessageBus**
   - System komunikacji między obiektami
   - Eliminacja bezpośrednich zależności
   - Zwiększenie modularności kodu

## Instrukcja Użytkownika

### Wymagania Systemowe
- CMake 3.16+
- Kompilator zgodny z C++17
- SFML 3.0.0

### Sterowanie
- **W/A/S/D** lub **Strzałki** - Poruszanie statkiem
- **Lewy przycisk myszy** - Celowanie
- **Spacja/LPM** - Strzelanie

### Typy Broni i Ich Rzadkość
1. **Rifle** (Common - 45%)
   - Szybki ogień
   - Pojedyncze pociski
   - Biała poświata

2. **Revolver** (Uncommon - 30%)
   - Średnia szybkostrzelność
   - Mocne pojedyncze pociski
   - Zielona poświata

3. **Shotgun** (Rare - 15%)
   - Rozproszone pociski
   - Wysoka siła rażenia
   - Niebieska poświata

4. **Rocket Launcher** (Epic - 7%)
   - Potężne pociski
   - Efekt odrzutu
   - Pomarańczowa poświata

5. **Flamethrower** (Legendary - 3%)
   - Ciągły ogień
   - Wysoka szybkostrzelność
   - Złota poświata

## Ciekawostki Techniczne

### System Cząsteczek
- Własna implementacja systemu cząsteczek do efektów eksplozji

### System Broni
- Hierarchiczny system rzadkości broni
- Bronie mają unikalne statystyki i efekty wizualne

### Zasoby
- Wykorzystanie darmowych assetów z OpenGameArt.org, Pixabay.com oraz audio.com
- Retro styl grafiki i dźwięków
- Spójna estetyka wizualna

## Podsumowanie i Wnioski

### Zrealizowane Założenia
- ✅ Klasyczna rozgrywka Asteroids
- ✅ Dodanie tekstur do sprite'ów
- ✅ System różnych typów broni
- ✅ Efekty dźwiękowe
- ✅ System punktacji i statystyk

### Wyzwania
- Implementacja architektury Actor-Controller-Scene
- Stworzenie systemu broni
- Optymalizacja wydajności przy dużej liczbie entities w grze

### Wnioski
- Dobrze zaprojektowana architektura pozwoliła na łatwe rozwijanie i stabilność gry

## Możliwości Rozbudowy

1. Dodanie możliwości zmiany ustawień z poziomu gry
2. Implementacja systemu rozbudowy statku
3. Tryb multiplayer
4. Dodanie poziomów

## Licencja
Projekt stworzony do celów edukacyjnych.