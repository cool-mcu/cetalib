# CETALIB Joystick UDP Publisher

This Python script captures raw HID reports from a **Logitech F310 Gamepad** and streams them as raw bytes over UDP to a specified target (default: 192.168.42.1:8888). It is meant to be used with robots that are running sketches using the **cetalib joystick library**.

**The F310 must be configured in D-Mode using the switch at the bottom-side of the gamepad:**

<img src="../../assets/f310-feature-1.webp?raw=true">

## 📋 Prerequisites

* Python 3.12+
* Logitech F310 Controller: Ensure the switch on the back is set to "D" (DirectInput).
* C compiler: **hidapi** may require system C compilers (Xcode tools on macOS or Visual Studio Build Tools on Windows).

---

## 🛠️ Setup Instructions

### 1. Install Python on your PC

### 2. Create a Virtual Environment
Isolating dependencies is recommended to keep your system Python clean.

Navigate to the folder containing the "cetalib-joystick-client.py" python script, then enter the following:

Windows 11 (PowerShell/CMD):
* python -m venv venv
* .\venv\Scripts\activate.bat

macOS (Terminal):
* python3 -m venv venv
* source venv/bin/activate

### 3. Install Dependencies
With your virtual environment activated, run:

* pip install --upgrade pip
* pip install -r requirements.txt

Note for macOS Users: If you receive errors regarding the hid library, install the system library via Homebrew:
* brew install hidapi

---

## 🚀 Running the Script

After uploading a joystick sketch to your robot, scan for WiFi networks on your
PC and look for your robot's unique Access Point (AP) SSID. It will have the
following format:

* "XRP_abcd", or
* "XRPBeta_abcd", or
* "CETAIoT_abcd"

Where "abcd" are the lower 2 bytes of the robot radio's MAC address.
Login to the network using the AP passphrase, which is identical to the SSID.

Next, ensure your F310 is plugged in, then execute:

* python cetalib-joystick-client.py

You should see a successful connection and display of raw gamepad data when activating the various switches.

<img src="../../assets/cetalib-joystick-client-macos.jpg?raw=true">

Press "CTRL-C" to stop the script.

---

## 🔍 Troubleshooting

| Issue | Solution |
| :--- | :--- |
| "F310 not found" | Verify the device is connected. Toggle the switch on the back between X and D modes. |
| Permission Denied (macOS) | Go to System Settings > Privacy & Security > Input Monitoring and enable your terminal. |
| UDP Data not received | Verify PC WiFi connectivity to your robot's AP and check your local firewall settings. |
| ModuleNotFoundError | Ensure your virtual environment is active (look for '(venv)' in the prompt). |