# Cryonix: A Modern C++ Linux Userland
![cryonix](https://github.com/user-attachments/assets/528d7e42-03b5-4c1b-a390-65f463b3a1de)


# Cryonix - The Ultimate Linux Userland

## ⚡ What is Cryonix?
Cryonix is a **modern, high-performance, and feature-rich** userland for Linux, written entirely in **C++23**. It redefines how you interact with Linux utilities by offering **optimized, beautiful, and incredibly powerful alternatives** to the standard GNU core utilities.

Unlike traditional userlands, Cryonix aims to bring **innovation, speed, and enhanced usability** while maintaining full compatibility with UNIX philosophy.

---
## 🚀 Key Features
✔ **One-to-one compatibility** with GNU userland utilities<br>
✔ **C++23-powered optimizations** for faster and safer execution<br>
✔ **Enhanced user experience** with **colorful output**, **powerful flags**, and **better error handling**<br>
✔ **Easter eggs & fun features** that bring joy to your terminal<br>
✔ **Lightweight and efficient**, built with performance in mind<br>
✔ **Full support for multi-threaded operations** where applicable<br>
✔ **Modular design** - use only the tools you need<br>

---
## 🛠 Built-in Utilities
Cryonix includes a **full suite of Unix commands**, all rewritten from scratch in modern C++.
Here’s a **list of completed utilities**, each built with power and efficiency:

### 🔥 Essential Commands
- [x] `lsd / lsda` - List directory contents
- [x] `copy` - Copy files and directories
- [x] `move` - Move/rename files
- [x] `rm` - Remove files (with a funny warning for `rm -rf /`!)
- [x] `mkdir` - Create directories
- [x] `rmdir` - Remove empty directories
- [x] `modech` - Change file permissions
- [x] `ownerch` - Change file ownership
- [x] `chroot` - Change root directory
- [x] `df` - Show disk space usage with colorized output
- [x] `stat` - Powerful and colorful file statistics
- [x] `uname` - System information with `--fetch` flag (Neofetch-style output!)
- [x] `saymyname` - Display current user
- [x] `uptime` - Show system uptime in an aesthetic way
- [x] `useradd` - Add new user to the system
- [x] `usermod` - Modify existing user in the system
- [x] `groupadd` - Add new user group
- [x] `groupmod` - Modify existing group
- [x] `date` - View current system date and time. ASCII calendar included!

### 🧩 Advanced Commands
- [x] `sed` - A **full one-to-one clone** of the powerful stream editor
- [x] `cut` - Extract sections of lines from input
- [x] `sort` - Sort lines of text files
- [x] `uniq` - Find unique lines in a file
- [x] `tr` - Translate, squeeze, or delete characters
- [x] `tee` - Redirect output to multiple locations
- [x] `shred` - Securely erase files (with a **dark humor easter egg**!)

---
## ⚡ Installation
### 1️⃣ Clone the Repository
```bash
git clone https://github.com/your-repo/cryonix.git
cd cryonix
```

### 2️⃣ Build the Project
```bash
make all
```
> This will build **each command separately** into `bin/`

### 3️⃣ Add Cryonix to Your Path
```bash
export PATH=$PWD/bin:$PATH
```
Now you can use all Cryonix utilities seamlessly!

---
## 🎮 Usage Examples
**Basic Commands:**
```bash
lsda       # List files with human-readable sizes
chmod +x file # Add executable permission
copy file.txt backup/ # Copy file to backup folder
```

**Powerful Flags & Fun Easter Eggs:**
```bash
uname --fetch  # Beautiful system information like Neofetch!
rm -rf /       # Funny warning before destruction
shred --force  # Data loss has never been so... entertaining?
touch --grass  # Dark humor alert!
whoami --heisenberg  # "You god damn right!"
```

---
## 🏆 Why Choose Cryonix?
- **Blazing Fast Execution** 🚀
- **More Intuitive and Fun to Use** 🎉
- **Colorful and Modern Output** 🎨
- **Written with Cutting-Edge C++23** 🛠
- **Works as a Drop-In Replacement for GNU Utilities** 💎

---
## 🔧 Contributing
Want to improve Cryonix? Join the revolution!
1. Fork the repository 🍴
2. Create a new branch 🛠
3. Submit a Pull Request 🎉

---
## 📜 License
Cryonix is **open-source** and licensed under the GPL License. Feel free to modify and use it as you like!

---
## ⭐ Acknowledgments
Special thanks to **Linux**, **GNU**, and **C++** communities for their inspiration. Cryonix is here to redefine how userlands work!

🚀 **Level up your Linux experience with Cryonix!**


