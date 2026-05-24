# 🗄️ Smart Standing Desk

Этот проект предназначен для полной интеграции подстолья **Volundr X2DM PRO** в Home Assistant при помощи **ESP32-S3** и ESPHome. 

Управление реализовано аппаратно через подачу импульсов на емкостные датчики (сенсорные кнопки) на плате контроллера стола. Текущая высота считывается напрямую с пинов `DAT` и `CLK` контроллера дисплея **ET6226M**.

## ✨ Ключевые особенности

- **Авто-высота:** Достаточно выбрать нужную высоту ползунком в Home Assistant. Если мотор немного переехал или недоехал цель, скрипт автоматически выполнит микро-шаги (по 0.1 см) в обратном направлении, пока высота не станет идеальной.
- **Компенсация инерции:** Скрипт учитывает "выбег" мотора (плавную остановку) и отпускает виртуальную кнопку ровно за **0.4 см** до цели, сводя необходимость корректировок к минимуму.
- **Обход спящего режима (Wake-up logic):** Контроллер стола засыпает при бездействии. Логика делает предварительный "клик" (100 мс) для пробуждения пульта, ждет 150 мс, и только потом отправляет основную команду движения.
## 🛠 Оборудование и схема подключения

Для реализации данного проекта вам понадобится:
- Плата ESP32-S3 (в проекте используется `esp32-s3-devkitc-1`)
- 10 проводов
- 6 конденсаторов емкостью 30 пФ
- 2 резистора номиналом 4.7 кОм
- Паяльник, флюс, припой

### Наглядная схема подключения:
<img width="1181" height="914" alt="Wiring diagram" src="https://github.com/user-attachments/assets/6da906cc-00fd-4271-917e-c7c09a78c476" />

### Реализация на плате:
<img width="1373" height="794" alt="Screenshot 2026-05-24 at 1 09 10 AM" src="https://github.com/user-attachments/assets/44d8cfa6-9764-4263-a5e7-f70cfd09e5e6" />

*Пины подключения к ESP32:*
- `GPIO 9` — кнопка M
- `GPIO 10` — кнопка UP (Вверх)
- `GPIO 11` — кнопка DOWN (Вниз)
- `GPIO 12` — кнопка 1
- `GPIO 13` — кнопка 2
- `GPIO 14` — кнопка 3

Для установки прошивки использовался esphome версии 2023.12.9 !

В репозитории есть директория с фотографиями, там можно рассмотреть плату с разных сторон.

## English version

# 🗄️ Smart Standing Desk

This project is designed for the full integration of the **Volundr X2DM PRO** standing desk frame into Home Assistant using an **ESP32-S3** and ESPHome.

Control is implemented at the hardware level by sending pulses to the capacitive sensors (touch buttons) on the desk's controller board. The current height is read directly from the `DAT` and `CLK` pins of the **ET6226M** display controller.

## ✨ Key Features

- **Auto-height:** Simply select the desired height using the slider in Home Assistant. If the motor slightly overshoots or undershoots the target, the script will automatically perform micro-steps (0.1 cm each) in the opposite direction until the height is perfect.
- **Inertia Compensation:** The script accounts for the motor's momentum (smooth stop) and releases the virtual button exactly **0.4 cm** before the target, minimizing the need for subsequent corrections.
- **Wake-up Logic:** The desk controller goes to sleep during inactivity. The logic performs a preliminary "click" (100 ms) to wake up the control panel, waits 150 ms, and only then sends the main movement command.

## 🛠 Hardware and Wiring Diagram

To implement this project, you will need:
- ESP32-S3 board (the project uses `esp32-s3-devkitc-1`)
- 10 wires
- 6 capacitors (30 pF)
- 2 resistors (4.7 kOhm)
- Soldering iron, flux, solder

### Visual Wiring Diagram:
<img width="1181" height="914" alt="Wiring diagram" src="https://github.com/user-attachments/assets/6da906cc-00fd-4271-917e-c7c09a78c476" />

### Implementation on the Board:
<img width="1373" height="794" alt="Screenshot 2026-05-24 at 1 09 10 AM" src="https://github.com/user-attachments/assets/44d8cfa6-9764-4263-a5e7-f70cfd09e5e6" />

*ESP32 Pinout:*
- `GPIO 9` — M button
- `GPIO 10` — UP button
- `GPIO 11` — DOWN button
- `GPIO 12` — 1 button
- `GPIO 13` — 2 button
- `GPIO 14` — 3 button

ESPHome version 2023.12.9 was used to install the firmware!

There is a directory with photos in the repository where you can examine the board from different angles.


Tags: esphome, esp32, esp32-s3, home-assistant, standing-desk, smart-desk, sit-stand-desk, volundr, volundr-x2dm-pro, et6226m, home-automation, smart-home, diy, diy-electronics, hardware-hacking, iot, internet-of-things, desk-setup, workspace, automation, maker, reverse-engineering, capacitive-sensors, capacitive-touch, умный-стол, умный-дом, регулируемый-стол, стол-с-электроприводом, автоматизация, сделай-сам
