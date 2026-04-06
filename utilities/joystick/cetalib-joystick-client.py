# 
# Copyright (C) 2026 dBm Signal Dynamics Inc
#
# File:     cetalib-joystick-client.py
# Version:  0.0.1
# Date:     March 30, 2026
#
# Description:
#
# This Python script captures raw HID reports from a Logitech F310 Gamepad and
# streams them as raw bytes over UDP to a XRP, XRP Beta, or CETA IoT Robot running
# the CETALIB "joystick" library using the default IP:Port 192.168.42.1:8888.
# The F310 must be set to "D" mode.
#

import hid
import time
import socket
import signal
import sys

# Logitech F310 IDs
LOGITECH_VID = 0x046d
PID_MODE_D = 0xc216  
PID_MODE_X = 0xc21d

def exit_gracefully(signum, frame):
    print("\n[!] Exiting...")
    sys.exit(0)

# Handle Ctrl+C (works on Windows and macOS)
signal.signal(signal.SIGINT, exit_gracefully)

def find_f310():
    for device in hid.enumerate(LOGITECH_VID):
        if device['product_id'] in [PID_MODE_D, PID_MODE_X]:
            return device['path']
    return None

def main():
    print("--- CETALIB Joystick UDP Publisher v0.0.1 ---")
    ip = "192.168.42.1"
    port = 8888

    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server_address = (ip, port)

    path = find_f310()
    if not path:
        print("F310 not found."); return

    try:
        device = hid.device()
        device.open_path(path)
        device.set_nonblocking(True)
        
        print(f"Streaming raw bytes to {ip}:{port}...")

        while True:
            # Read 8 bytes (Standard F310 Report size in Mode D)
            report = device.read(8)
            
            if report:
                # 1. Convert to immutable 'bytes' type for the socket
                raw_payload = bytes(report)
                
                # 2. Send over UDP
                udp_socket.sendto(raw_payload, server_address)

                # 3. Output to terminal for verification
                # Shows: [128, 128, 128, 128, 8, 0, 0, 0]
                print(f"Raw: {list(raw_payload)}", end='\r')

            time.sleep(0.01) # 100Hz

    except Exception as e:
        print(f"\nError: {e}")
    finally:
        device.close()
        udp_socket.close()

if __name__ == "__main__":
    main()