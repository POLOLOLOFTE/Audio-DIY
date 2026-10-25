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

the rest is not that kind of easy but still simple when you know how it works:

you gotta connect all the buttons, potenziometers, leds;

trough some wire and you have to be near the schematic of the wires if you don't memorize it

and after this work you have to connect the mess (mine is going to be for sure it) to the pcb by the 22 screw connector

> **REMEMBER:** Lable every wire that gose in the pcb! with a number and a letter (for example P2: it's the second potenziometer).

for just not have all the wire and board hanging there you can print on a 3d printer the customizable at pcb-extra/3d/customizable case

and you have included all of the pieces and the 3d model.

for the case you have to go to my other reposty: freedigital amp case and you got the instructions for mounting it. but this board hasn't got any screw so it's mounted with hot glue(i know it's not professional but it work well)
an then there are the buttons (i choose to use some bipolar metal ones: they look sick) and potenziometers that goes in with the screw and the leds: just hot glue them

## INPUT and OUTPUT
the board has got 2 inputs:

the usb type c of the esp32 Devkit V1 30pin (for power)

the jack (the one soder on the board)

and it has got one output:

the jack on the other card (Analog Codec)

## Settings
The sound is controlled with 6 switch and 8 potenziometers, and they work all indipendently to others so your code can be basically anything differnt and still works with all the switch and potenziometers

### Let's talk about buttons to press!
if you use the the default code here are the things that you can do:

if you press any button you arledy strarting to change the sound of the input but the real magic comes with the potenziometers and you take controll of the audio compleatlly

there is a single potenziometers for thoose sounds that controls the power:
- pitch changer
- flangers
- reverb
- echo

but there are two effects that have two potenziometers:

- filter high-low
  the first one control the low frequences (it can boost it or lower it)
  the second control the high frequences (it can boost it or lower it)
  the medium frequences are untouched

- shutter
  the first potenziometer control the time of thoose shut (how much time they stay)
  and the second control the pause between those shuts


## Gallery

<img width="1316" height="845" alt="Screenshot 2026-10-24 114313" src="https://github.com/user-attachments/assets/d59d8214-df56-44e1-8844-2af36f794658" />

<img width="940" height="660" alt="Screenshot 2026-10-24 114334" src="https://github.com/user-attachments/assets/573e8bfd-898e-4a5c-9896-26505c93c12b" />

<img width="617" height="596" alt="Screenshot 2026-10-24 165023" src="https://github.com/user-attachments/assets/949a267e-25ea-4813-9eca-5e293fa9f0f0" />

<img width="663" height="696" alt="Screenshot 2026-10-23 152326" src="https://github.com/user-attachments/assets/86b87f5b-46ca-43a1-bf6a-1875e77927c7" />

<img width="718" height="446" alt="Screenshot 2026-10-24 165105" src="https://github.com/user-attachments/assets/7dc5b61b-739c-41e9-aaac-5bca2e7180b3" />


# Components that you Need



| Piece | Quantity | Notes | Link | USD |
| :--- | :---: | :--- | :--- | :---: |
| **ESP32 DevKit V1 (30pin)** | 1 | Main brain | [eBay Best Offer](https://www.ebay.it/itm/266490362209) | 14.23 |
| **Analog Codec Module** | 1 | Analog codec | [PCB Artists Site](https://pcbartists.com/product/es8388-module/) | 16.79 |
| **ADS1115IDGS** | 2 | potenziometers controller | [Amazon](https://www.amazon.it/-/en/ADS1115IDGSR-ADS1115IDGST-ADS1115IDGS-ADS1115-MSOP10/dp/B0CRQWTRNL/ref=sr_1_1?crid=3A7DLQN7I903N&dib=eyJ2IjoiMSJ9.-TmIbu7YQJw4iThlc6OG7MWgpj6EB_HSH48wNqmVZONnfdk-r3KiK01sLE0pXHxF-g91VWozS64dN5ynV5B_7gTRlVeP80SvG5o6kZ8EeXDfm4AXq1NMQx5EuUOJW4f5hZnC9gTOGtNqXZ1vLr6--8VqCrM2nnEDV2sqoPslPDWQHNZFGwUWPSa1a-AoExBXTplhvIQLp9tFjpdFu_JAK7lKG3X4uck0JFiCnp2s8OXNujH90LfcE37R5i9vtuiafWzRcmp6w7TZ4ZgTYhNOqxK_yItWzCYx4bKpvucx_pc.o1-ewgyV3qJF_BI8BGCa-Ho__qsU65buIypSJFIkgu4&dib_tag=se&keywords=ADS1115IDGS&qid=1792947325&sprefix=ads1115idgs%2Caps%2C146&sr=8-1) | 16.01 |
| **Resistor220Ω - 330Ω** | 6 | resistor for the leds | [Amazon](https://www.amazon.it/-/en/gp/product/B0FP51H3ML/ref=ox_sc_act_title_5?smid=A2X2IS1I8BHRL9&psc=1) | 15.66 |
| **Unpolarized capacitor** | 2 | protection for the controllers | [Amazon](https://www.amazon.it/-/en/gp/product/B07Y7PHY91/ref=ox_sc_act_title_9?smid=A3JM63OQAICKNV&psc=1) | 10.06 |
| **Polarized capacitor** | 2 | 10µF | [Amazon](https://www.amazon.it/TUNOTYUE-Electrolytic-Capacitors-Aluminum-Capacitor/dp/B0G2RY2BSH/ref=sr_1_1_sspa?crid=2CMKG581YDDUB&dib=eyJ2IjoiMSJ9.hzyaSPMoM8UhoWIptWNiW6G1ByCsDW-fX1Y78nOgJteIzrt-ga4Y0Wzr1AeJ893IjKJKRG0Q-MU_greb-UPTFb1oTTV7A_xKjiFIupc53l08Mb1fv9mMQs5gXCBuczoo5EgjF2RbquK0QHhrpx28v6KynAJ9Z2j0xiX9d6wYOMIiQGtZq3MqOp8xTvxdtciF26dqlxtNkBuUVuylpYMfEho5vOQ-aiGJTmhC9xACjvkDxJ2_NxNiHPj-UsCgDH_cm2x82R-BsodixUEZn82E7Y9k0r-zQoXajgedLavJCTI.gBNOIevjQ0IQa8Sm-2RqyOmjishg9t3M5NQw-km7WHU&dib_tag=se&keywords=capacitor+10uf+25v&qid=1792948425&sprefix=capacitor+10uf+25v%2Caps%2C148&sr=8-1-spons&aref=uQbY1q8Pl5&sp_csd=d2lkZ2V0TmFtZT1zcF9hdGY&psc=1) | 7.82 |
| **Screw connector** | 11x2 | connector for the accesorys | [Amazon](https://www.amazon.it/-/en/gp/product/B0FN6VYSFN/ref=ox_sc_act_title_8?smid=A2KWOEY1RHRC64&psc=1) | 12.30 |
| **Header Pin Female 1x10,1x15** | 2+2 | standard conectors 2.54mm | [Amazon](https://www.amazon.it/-/en/gp/product/B09NDMVD2C/ref=ox_sc_act_title_2?smid=A2FXWI8CY4G34J&psc=1) | 13.42 |
| **Jack Female** | 1 | Jack audio Wide without switch | [Amazon](https://www.amazon.it/-/en/gp/product/B0C4SV6GGS/ref=ox_sc_act_title_1?smid=A1TMZHY2SHKLH2&psc=1) | 19.46 |
| **Potenziometrs** | 8 | 10K potenziometers | [Amazon](https://www.amazon.it/-/en/gp/product/B07RHJZ979/ref=ox_sc_act_title_4?smid=AT0FJ7CZCB0G9&psc=1) | 11.18 |
| **Bipolar switches** | 6 | Switches used in racecars or airplanes | [Amazon](https://www.amazon.it/-/en/gp/product/B0GJN7GH6H/ref=ox_sc_act_title_3?smid=A2V9110KNWB97R&psc=1) | 9.25 |
| **LED** | 6 | LED standard 5mm | [Amazon](https://www.amazon.it/-/en/CESFONJER-500pcs-Emitting-Diffuser-Yellow/dp/B07DPJ23YT/ref=sr_1_37?crid=XZEDS0JP4E2B&dib=eyJ2IjoiMSJ9.IWaHpNgDMd8Aqs3IYhj34O66BZcgvMejt1W-sJlgLg5VYvXCjT1i-Qo6sT5UX5z1Vkh7h-rPIJF7w7bXKRT9yNzgOXVT_sMCTEhwaVdQcK7R-QR-ZqL3GEROBvxE-fju-tqfYjbFMVByLecUZt8rBpD-QQpcuPIYgGm_XH2h7ucs__5ha7a-pwbaMqnTLsln-Eea8YbCn4mcby-hXiz8kMpZAvMNrZhaO9aRDUQIA9IG20oFuBItBYIPe4QQ8i_ySTk37-TOvAgFNocJqGzd1c5bd24fr2TU_Xe9J8UhwNw.fy-LiWBKteqsv5HQnHX_2Hs5eu9_gWXX_ik8kk5EbeA&dib_tag=se&keywords=leds&qid=1792947888&sprefix=leds%2Caps%2C207&sr=8-37) | 14 |
| **Main PCB** | 1 | the one in the main folder | you can make it yourself or let it make from: (ex. JLCPCB / PCBWay) | 5.00/10 |
| **Soder** | 1 | For soder everything | you can buy it even localy | Variable |
| **Wire** | A bit for connecting things togheder | You can get it everywere | every type is great | Variable |




