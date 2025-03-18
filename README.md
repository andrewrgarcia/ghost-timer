# GhostTimer ⏳👻  
*A lightweight floating countdown timer for Linux, built in C using XOSD.*

## Overview  
GhostTimer is a **C-based countdown timer** that displays a **non-intrusive on-screen timer** using `osd_cat`. It updates smoothly **without flickering** and changes colors as time runs out:

- 🔵 **Cyan** → More than 50% time remaining  
- ⚪ **White** → Between 50% and 25% remaining  
- 🟠 **Orange** → Less than 25% remaining  
- 🔴 **Red** → Less than 10% remaining  
- ⏳ **"Time's Up!"** → Displays in red, then disappears  


## Installation  
### **1️⃣ Install Dependencies**  
GhostTimer requires `xosd-bin` for on-screen display:  
```bash
sudo apt install xosd-bin
```

### **2️⃣ Clone & Build**  
```bash
git clone https://github.com/yourusername/ghost-timer.git
cd ghost-timer
make
```

### **3️⃣ Run**  
```bash
make run
```

## Usage  
- Enter the **hours, minutes, and seconds** when prompted.  
- The **timer floats on-screen** and updates in real-time.  
- When time runs out, it displays **"Time's Up!"** in red, then disappears.  

## Features  
✔ **Written in C for performance**  
✔ **[ALMOST] Non-blinking, real-time updates**  
✔ **Floating on-screen timer (XOSD-based)**  
✔ **Automatic color transitions (White → Orange → Red)**  
✔ **Graceful exit with `Ctrl+C`**  
✔ **Minimal CPU usage**  

## Uninstall  
```bash
sudo make uninstall
```

## Contributing  
Contributions are welcome! Feel free to **fork, submit PRs, or suggest improvements**.

## License  
This project is licensed under the **MIT License**.
