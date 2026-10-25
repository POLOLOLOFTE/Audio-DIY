# Audio-DIY
An audio console where you can play with real time music!

## Descpription:
This is an Audio modificator in real time so you can controll how the audio sounds and give it some DJ style effects and it's open source so you can download the premade code or just code it yourself however you want

> **Note:** This is NOT a toy but neather a professional equipment.

this work with a 'ESP32 DevKit V1 30pin' and another board called codec and it's the board that take the analog input and convert it in digital numbers: 0 and 1

### Reasons:

i made this because i love music and i use to "play" with an old amplificators settings: there was only a filter for low and high frequences sound😭

## Assebly
it's preatty simple you gotta to soder with a sodering iron all of the things: like resistors, capacitors and screw connectors.

i have tryed to make it easy as possible but the controllers for the potenziometers is not soderable trough board so it won't be as easy as the other components

## INPUT and OUTPUT
the board has got 2 inputs:

the usb type c of the esp32 Devkit V1 30pin (for power)

the jack (the one soder on the board)

and it has got one output:

the jack on the other card (Analog Codec)

## Settings
The sound is controlled with 6 switch and 8 potenziometers, and they work all indipendently to others so your code can be basically anything differnt and still works with all the switch and potenziometers

### Gallery

<img width="1316" height="845" alt="Screenshot 2026-10-24 114313" src="https://github.com/user-attachments/assets/d59d8214-df56-44e1-8844-2af36f794658" />

<img width="940" height="660" alt="Screenshot 2026-10-24 114334" src="https://github.com/user-attachments/assets/573e8bfd-898e-4a5c-9896-26505c93c12b" />

<img width="617" height="596" alt="Screenshot 2026-10-24 165023" src="https://github.com/user-attachments/assets/949a267e-25ea-4813-9eca-5e293fa9f0f0" />

<img width="663" height="696" alt="Screenshot 2026-10-23 152326" src="https://github.com/user-attachments/assets/86b87f5b-46ca-43a1-bf6a-1875e77927c7" />

<img width="718" height="446" alt="Screenshot 2026-10-24 165105" src="https://github.com/user-attachments/assets/7dc5b61b-739c-41e9-aaac-5bca2e7180b3" />


# Components that you Need

- [ ] Esp32 DevKit V1 30pin [ Ebay's best offer](https://www.ebay.it/itm/266490362209?chn=ps&norover=1&mkevt=1&mkrid=724-173151-946886-5&mkcid=2&mkscid=101&itemid=266490362209&targetid=2302781041175&device=c&mktype=pla&googleloc=1008982&poi=&campaignid=21278170735&mkgroupid=160806971983&rlsatarget=pla-2302781041175&abcId=9414772&merchantid=6772416&gad_source=1&gad_campaignid=21278170735&gclid=Cj0KCQjwrvHWBhDKARIsABmp7h18D0lcg4mwYWQNuUb_xjlLxH_7VBD56x7ZvB-pxFoKnvR2Bwi4oMIaAjqlEALw_wcB)
- [ ] Analog Codec [ Pcb Artist site(where i found it) ](https://pcbartists.com/product/es8388-module/)
- [ ] 6 220-330homs resistors
- [ ] 2 100nf unpolarized capacitors
- [ ] 2 10uf polarized capacitors
- [ ] some screw connectors (needng them for the buttons/potenziometers) 22 entrace
- [ ] female header pins: 2 1x15; 2 1x10;
- [ ] a female jack without switch
- [ ] the pcb itself
- [ ] some soder and the iron
- [ ] quiet a bit of wire to connect everything
- [ ] 8 potenziometers
- [ ] 6 duble switch
- [ ] 6 Led's, color: what you like
- [ ] 2 ADS1115IDGS (the controller for the potenziometers)




