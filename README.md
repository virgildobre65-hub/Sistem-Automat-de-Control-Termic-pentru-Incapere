# Sistem Automat de Control Termic și Iluminare pentru Încăpere

Acest repository este dedicat dezvoltării unui sistem inteligent automatizat, conceput pentru monitorizarea și gestionarea eficientă a temperaturii și a iluminatului ambiental într-o încăpere.

Proiectul utilizează un microcontroler (ATmega328P / Arduino Nano) care prelucrează în timp real datele de la senzorii de temperatură ambientală și luminozitate pentru a asigura un confort optim. 
Sistemul implementează o mașină de stări finită (FSM) care permite utilizatorului să comute și să configureze dinamic parametrii de funcționare ai mai multor periferice hardware folosind un singur potențiometru și un buton cu debounce software profesional. Datorită logicii integrate, intervenția utilizatorului este minimă: dacă temperatura depășește pragul stabilit, se activează ventilatorul via PWM; dacă temperatura scade sub o limită sigură, se declanșează o alarmă optică; dacă lumina scade sub pragul setat, sistemul activează automat corpurile de iluminat.

Repository-ul conține codul sursă (C), documentația și resursele necesare pentru înțelegerea, reproducerea și extinderea acestui sistem.

## Specificații

 - *Arhitectură Bare-Metal*: Manipulare directă a regiștrilor (register manipulation) pentru control total asupra hardware-ului, eficiență maximă a resurselor și latență minimă.
 - *BSP (Board Support Package)*: Mapare hardware specifică pentru placa Arduino Nano, decuplând logica aplicației de nivelul fizic.
 - *Mașină de Stări Finită (FSM)*: Comutare fluidă între regimurile de configurare (Ventilator, Alarmă de temperatură minimă, Prag de iluminat ambiental).
 - *Interfață Serială (CLI)*: Monitorizare în timp real prin USART la 9600 baud, cu curățare automată a ecranului terminalului și indicarea vizuală a regimului de reglaj activ curent.
 - *Drivere*: Arhitectură modulară, documentată și reutilizabilă pentru perifericele microcontrolerului:
    - *GPIO*: Inițializare, Write și Toggle (control digital pentru liniile de Iluminat, LED Alarmă și citire buton pe pinul `PD2`).
    - *ADC*: Conversie pe 10-biți pe mai multe canale pentru eșantionarea precisă a senzorului de temperatură (Termistor NTC), senzorului de lumină (LDR) și a potențiometrului de reglaj.
    - *PWM*: Control hardware cu frecvență fixă (25kHz) pe Port B pentru ajustarea dinamică a factorului de umplere aplicat ventilatorului.
    - *Timer*: System Tick generat via Timer hardware (Timer0) pentru gestionarea neblocantă a timpului (`Millis()`), esențială pentru algoritmul de debounce și citirile periodice ale senzorilor.
    - *EEPROM*: Salvarea pragurilor setate de utilizator pentru păstrarea configurărilor la căderile de tensiune.
 - *Sistem de Build Robust*: Utilizarea Makefile pentru automatizarea proceselor de compilare, link-editare și scriere (flash) via avrdude.
 - *Unit Testing pe Host*: Rularea testelor unitare direct pe calculator prin tehnici de register mocking, validând logica de comutare a stărilor fără a necesita hardware-ul fizic.

## Structura Regimurilor de Lucru

Prin apăsarea butonului hardware conectat la pinul `PD2`, sistemul ciclează prin următoarele stări de configurare (comutarea fiind protejată de un algoritm de debounce de 50ms):

1. **MODE_SET_FAN (Reglaj Ventilator):** Mișcarea fizică a potențiometrului ajustează pragul de temperatură superioară pentru activarea ventilatorului (Interval: 18°C - 35°C).
2. **MODE_SET_ALARM (Reglaj Alarmă):** Mișcarea fizică a potențiometrului ajustează pragul de temperatură inferioară sub care se declanșează alerta vizuală prin toggle intermitent (Interval: 10°C - 35°C).
3. **MODE_SET_LIGHT (Reglaj Iluminat):** Mișcarea fizică a potențiometrului modifică valoarea brută ADC pentru pragul de declanșare a celor două linii de iluminat ambiental (Interval: 0 - 1023).

## Configurație Pini Hardware (Pinout)

| Componentă Hardware | Pin Arduino Nano | Registru / Canal Hardware | Mod Operare |
| :--- | :--- | :--- | :--- |
| **Buton Selectare Mod** | D2 | Port D (PD2) / Pull-up activat | INPUT |
| **Potențiometru Reglaj** | A1 | ADC Channel 1 | INPUT |
| **Senzor Temperatură (Termistor)** | A0 | ADC Channel 0 | INPUT |
| **Senzor Lumină (LDR)** | A2 | ADC Channel 2 | INPUT |
| **LED Alarmă Temperatură** | D9 | Port B | OUTPUT |
| **Ieșire PWM Ventilator** | D10 | Port B (Bit 3) | OUTPUT |
| **Linie Iluminat 1** | D5 | Port D | OUTPUT |
| **Linie Iluminat 2** | D6 | Port D | OUTPUT |

## Roadmap

- [x] Structura de bază și Makefile
- [x] GPIO driver (suport INPUT_PULLUP și Toggle hardware)
- [x] ADC driver (citire multi-canal: Termistor, LDR, Potențiometru)
- [x] Timer driver (bază de timp stabilă pentru `Millis()`)
- [x] PWM driver (control ventilator la frecvență înaltă)
- [x] USART driver (interfață CLI și monitorizare)
- [x] EEPROM driver
- [x] Implementare mașină de stări (FSM) pentru selectarea regimurilor de control
- [x] Algoritm de Debounce Profesional pentru protecția butonului
- [x] Logica de control independent pentru sistemul de iluminat automatizat

## Structura Proiectului

```text
├── bsp/            
├── drivers/        
│   ├── adc/
│   └──alarm/
│   └──buton/
│   ├── eeprom/
│   └──fam/
│   ├── gpio/
│   ├── interrupt/
│   ├── pwm/
│   └──thermistor/
│   ├── timer/
│   └── usart/
├── src/            
├── test/           
│   ├── mocks/      
│   ├── framework/  
│   └── test_*.cpp  
├── utils/          
└── Makefile
