# DRUID – *Detection Rogue USB and Illegitimate Devices* (linux v1.0)

DRUID is a tool that analyzes connected USB devices and classifies them by risk level based on a reference database.

---

## 🚀 Usage

```
druid [options]
```

By default, if no argument is passed, the program automatically detects connected USB devices and compares them to the database.

## 📖 Options

To view the full list of options and usage examples :

```
druid --help
```

This will display :

 - Available commands

 - Expected CSV input format

 - Example use cases

 - License details

 - Required dependencies

## 📦 Requirements

Make sure systemd is installed on your system :

**Debian / Ubuntu**:
```
sudo apt-get install systemd
```

**Red Hat**:
```
sudo yum install systemd
```

**Arch Linux**:
```
sudo pacman -S systemd
```

📄 License
 - This project is licensed under the **Creative Commons Attribution-ShareAlike 4.0 International License** (CC BY-SA 4.0 DEED).

👤 Author
 - Developed by **Sacha Lemée**, Junior Cybersecurity Consultant at **Fujitsu Technology Solutions SAS**, during an internship at **EPITECH Paris**.
