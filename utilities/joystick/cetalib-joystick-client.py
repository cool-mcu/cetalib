# 
# Copyright (C) 2026 dBm Signal Dynamics Inc
#
# File:     cetalib-joystick-client.py
# Version:  0.0.2
# Date:     June 15, 2026
#
# Description:
#
# This Python script captures raw HID reports from a Logitech F310 Gamepad and
# streams them as raw bytes over a UDP socket to a XRP, XRP Beta, or CETA IoT Robot running
# sketches that use the CETALIB "joystick" library.
#
# The Robot's Joystick UDP Server Port number is 8888.
#
# Make sure to locate the IP address of the robot before running this script.
# 
# The F310 must be set to "D" mode.
#

import hid
import time
import socket
import signal
import sys
import ipaddress  # Added for strict IPv4 validation

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

def get_valid_ip():
    while True:
        user_input = input("Enter target IPv4 address: ").strip()
        
        if not user_input:
            print("Input cannot be empty. Please try again.\n")
            continue
            
        try:
            # Strict validation: will catch incomplete addresses like "192.168.1"
            ip_obj = ipaddress.IPv4Address(user_input)
            return str(ip_obj)  # Returns the clean, verified IP string
        except ValueError:
            print(f"Error: '{user_input}' is not a valid IPv4 address (must be X.X.X.X format). Please try again.\n")

def main():
    print("--- CETALIB Joystick UDP Publisher v0.0.2 ---")
    
    # Loop until a valid IPv4 address is entered
    ip = get_valid_ip()
    port = 8888

    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server_address = (ip, port)

    path = find_f310()
    if not path:
        print("F310 not found.")
        return

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