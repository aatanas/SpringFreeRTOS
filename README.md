# SpringFreeRTOS
Raspoređivač nad FreeRTOS koji omogućava raspoređivanje taskova po Spring algoritmu. 

Raspoređivač se implementira kao task visokog prioriteta koji raspoređuje ostale taskove nad postojećim FreeRTOS raspoređivačem.

Spring algoritam podržava:

- Sve osnovne heurističke funkcije:
  - a
  - C
  - d
  - d + W*C

- Rad sa resursima:
  - T<sub>est</sub>
  - d + W * T<sub>est</sub>

- Ograničenje prethođenja:
  - E

Poslovi se zadaju sistemu na izvršavanje u grupama (batch). 
Spring raspoređivač će pri startovanju svake grupe kao parametar primiti niz heurističkih funkcija koje treba da koristi pri raspoređivanju.
Ako sistem može da rasporedi grupu po nekoj funkciji, on to i učini, a u
suprotnom prijavljuje da to nije moguće. 
Raspored nije dobar ako bilo koji posao probija svoj rok.

Ako raspored jeste izvodiv, sistem izvršava poslove u tom redosledu, i u
tekstualnoj datoteci zapisuje svaku zamenu konteksta, tj. u kojem tačno tick-u je započet koji od poslova.

Sistem obezbeđuje detaljno logovanje izvršavanja.
Davanje ulaza sistemu je moguće kroz konfiguracionu datoteku, kao i sa konzole.

Pri zadavanju ulaza, korisnik navodi sledeće elemente:

- Resursi:
  - Nabrojati koji resursi postoje u sistemu, i za svaki od njih koje je najranije  
    vreme kada može da se koristi.

- Podaci o poslovima:
  - Startno vreme.
  - Vreme izvršavanja - pretpostavljamo da korisnik zna za svaki task tačno  
    koliko on traje, kao i da neće pogrešiti pri unosu.
  - Deadline.
  - Funkcija posla, data kao stringovno ime zadatka - pretpostavka je da su  
    funkcije fiksirane u sistemu i da se neće dinamički menjati.
  - Niz resursa koje ovaj posao koristi.
- Izbor niza heurističkih funkcija koje će biti korišćene za izvršavanje.
- Ograničenje prethođenja.

Svi vremenski parametri se zadaju u tick-ovima.






