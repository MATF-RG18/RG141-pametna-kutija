## RG141-pametna-kutija

**Pametna kutija :package:**

Projekat na kursu Računarska grafika, na Matematičkom fakultetu. 
Korisnik upravlja (plavom) kutijom koja se na komande tastature kreće po poligonu sa namerom uspešnog dolaska do cilja. Igrac mora pametno da gura druge (ruzičaste) kutije koje se nadju na putu do cilja.

Projekat je napisan na programskom jeziku C, korišćenjem grafičkog sistema OpenGL (okruženja za razvoj interaktivnih 2D i 3D grafičkih aplikacija) kao i korišćenjem GLUT biblioteke.

Sve potrebne biblioteke mogu se instalirati komandom: 
```
sudo apt-get install binutils-gold freeglut3 freeglut3-dev
```
za operativni sistem Ubuntu.

Za učitavanje tekstura korišćena je biblioteka SOIL. Instaliranje:
```
sudo apt-get install libsoil-dev
```

**Projekat uključuje:** 
   - crtanje primitiva
   - proveru dubine
   - osvetljenje
   - kretanje pomocu tastature
   - teksture
   - upotrebu korisnicki definisanih matrica kako bi se kreirao poligon
   - animaciju
   - obradu kolizija
   - …

**Pokretanje:**
```
make
```
```
./brainBox
```


**Komande:**
   | Tipka | Događaj |
   | ----- | ------ |
   | :arrow_up: | kretanje napred |
   | :arrow_down: | kretanje nazad |
   | :arrow_left: | kretanje ulevo |
   | :arrow_right: | kretanje udesno |
   | <kbd>R</kbd> | resetovanje programa |
   | <kbd>Esc</kbd> | izlaz iz programa |


Za više informacija posetiti: [wiki](https://github.com/MATF-RG18/RG141-pametna-kutija/wiki) stranu projekta.