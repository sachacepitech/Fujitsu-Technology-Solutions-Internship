# DRUID – *Detection Rogue USB and Illegitimate Devices* (Windows v1.0)

![DRUID logo](ressources/DRUID_green_transparent_no_label_4470x4470.png)

DRUID is a tool that analyzes connected USB devices and classifies them by risk level based on a reference database.

---

## 🚀 Usage

```
druid.exe [options]
```

By default, if no argument is passed, the program automatically detects connected USB devices and compares them to the database.

## 📖 Options

To view the full list of options and usage examples :

```
druid.exe --help
```

This will display :

 - Available commands

 - Expected CSV input format

 - Example use cases

 - License details

 - Required dependencies

## 📦 Requirements

Make sure your development environment supports Windows API development (e.g., Visual Studio with the Windows SDK):

This version is designed for Windows and uses the native Windows APIs :

```
windows.h
```

```
setupapi.h
```

📄 License
 - This project is licensed under the **Creative Commons Attribution-ShareAlike 4.0 International License** (CC BY-SA 4.0 DEED).

👤 Author
 - Developed by **Sacha Lemée**, Junior Cybersecurity Consultant at **Fujitsu Technology Solutions SAS**, during an internship at **EPITECH Paris**.
