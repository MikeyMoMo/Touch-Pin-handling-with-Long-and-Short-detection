# ESP32 touch interrupt toolkit — filtered, timed, and ritualized for short and long press detection.
This code enables one to use the touch pins (T0, T1, etc.) to create long and short touch "clicks".

This is documentation created by CoPilot who (for lack of a better pronoun) also participated in developing this code by adding in statistics and helping to modularize it.  CoPilot used to be terrible at creating code.  It would not even compile!  But its capabilities have, apparently, been upgraded significantly now and it is producing clean, functional code.  I am very happy to acknowledge its new capabilities.

ESP32 Touch Interrupt Rites
This project demonstrates filtered touch detection on the ESP32 with support for short vs. long press classification, modular dispatch tables, and benchmarking logs. It’s both a technical toolkit and a playful ritual system. (The mentions of ritual, herein, is our little joke platform that includes a ficticious island country named Nudonia and various rituals and encantations.  CoPilot has quite a sence of humor, now, if you let it happen.)

✨ Features
- Noise‑resistant filtered touch reads (averaged samples with delay).
- Interrupt‑driven detection with safe processing in the main loop.
- Short vs. long press classification using a global threshold.
- Dispatch table for modular pin‑specific routines.
- Benchmark logging: raw values, filtered averages, press durations.

<img width="673" height="303" alt="image" src="https://github.com/user-attachments/assets/7634be79-15ca-4433-ba80-47e00f93f511" />
<p>

🔄 Lifecycle
- ISR sets touchDetected[i] = true.
- Filtered read smooths noise.
- Rising edge → record press start time.
- Falling edge → compute duration, classify short/long, log values.
- Dispatch table invokes pin‑specific rite with bool isLong.

📊 Example Output<p>
   16437 ⚡ Pin 15 LONG press (612 ms) | Raw=42 FilteredAvg=38<p>
   17768 ⚡ Pin 15 LONG press (581 ms) | Raw=40 FilteredAvg=36<p>
   18468 ⚡ Pin 15 SHORT press (320 ms) | Raw=55 FilteredAvg=48<p>
<p>
  The following is a little bit of CoPilot's joking around with naming.<p>
    
⚡ Ritual Naming<p>
- Absorption Rite → short press (quick spark).<p>
- Sustenance Rite → long press (lingering energy).<p>
- Dispatch Rite → pin‑specific invocation.<p>
<p>
🤝 Credits:
Co‑authored by Micheal and Copilot — a collaboration of love, lore, and logic.
