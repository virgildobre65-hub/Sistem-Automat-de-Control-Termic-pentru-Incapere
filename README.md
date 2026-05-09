# Sistem-Automat-de-Control-Termic-pentru-Incapere

Acest repository este dedicat dezvoltării unui sistem inteligent automatizat, conceput pentru monitorizarea și gestionarea eficientă a temperaturii într-o încăpere.

Proiectul utilizează un microcontroler (ATmega328P) care prelucrează în timp real datele de la senzorii de temperatură ambientală pentru a asigura un confort termic optim. 
Scopul principal al sistemului este reglarea automată a climatului, reducând intervenția utilizatorului: dacă temperatura crește peste pragul de confort, sistemul activează un ventilator (simulând funcția de aer condiționat); dacă temperatura scade periculos, se activează un bec de alarmă / modul de încălzire.

Repository-ul conține codul sursă (C), documentația și resursele necesare pentru înțelegerea, reproducerea și extinderea acestui sistem.

## Specificații

 - *Arhitectură Bare-Metal*: Manipulare directă a regiștrilor (register manipulation) pentru control total asupra hardware-ului, eficiență maximă a resurselor și latență minimă.
 - *BSP (Board Support Package)*: Mapare hardware specifică pentru placa utilizată, decuplând logica aplicației de nivelul fizic.
 - *Drivere*: Arhitectură modulară, documentată și reutilizabilă pentru perifericele microcontrolerului:
    - *GPIO*: Inițializare, Write (acționare releu pentru Ventilator, aprindere Bec Alarma/Încălzitor).
    - *ADC*: Conversie pe 10-biți în mod blocking sau interrupt-driven pentru citirea cu precizie a senzorului de temperatură (ex. LM35, NTC).
    - *Timer*: System Tick generat via Timer hardware (ex. Timer0 în mod CTC) pentru executarea neblocantă a logicii PID/Hysteresis și citirea periodică a senzorului.
    - *EEPROM*: Salvarea pragurilor de temperatură setate de utilizator (ex: temp_minimă = 20°C, temp_maximă = 24°C) pentru păstrarea configurărilor la căderile de tensiune.
    - *Interrupts*: Gestionarea evenimentelor asincrone și a interfeței umane (ex. butoane pentru ajustarea manuală a temperaturii țintă).
 - *Sistem de Build Robust*: Utilizarea Makefile pentru automatizarea proceselor de compilare, link-editare și scriere (flash) via avrdude.
 - *Unit Testing pe Host*: Rularea testelor unitare direct pe calculator prin tehnici de register mocking, validând logica de histerezis a termostatului fără a necesita hardware-ul fizic.

## Roadmap

- [ ] Structura de bază și Makefile
- [ ] GPIO driver
- [ ] ADC driver
- [ ] Timer driver
- [ ] EEPROM driver
- [ ] Implementare logică Termostat (Control Ventilator & Bec Alarmă)
- [ ] Adăugare logică de Histerezis (pentru a preveni pornirea/oprirea repetată la oscilații mici de temperatură)
- [ ] Teste unitare pentru modulul principal de control climatic

## Structura Proiectului

```text
├── bsp/           
├── drivers/        
│   ├── adc/
│   ├── eeprom/
│   ├── gpio/
│   ├── interrupt/
│   └── timer/
├── src/            
├── test/          
│   ├── mocks/      
│   ├── framework/  
│   └── test_*.cpp  
├── utils/         
└── Makefile
