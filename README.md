# 🐚 Minishell  

## 📌 Description  
Minishell is a lightweight shell built in C, inspired by Bash. It supports command execution, built-in commands, I/O redirections, pipes, environment variable expansion, and signal handling. The project follows **42 Norm** and focuses on system programming concepts like process management and file descriptors.  

## 🚀 Features  
✅ Command execution using `execve()`  
✅ Built-in commands: `cd`, `echo`, `pwd`, `export`, `unset`, `env`, `exit`  
✅ Input/output redirections (`<`, `>`, `>>`, `<<`)  
✅ Pipes (`|`) for command chaining  
✅ Environment variable expansion (`$VAR`, `$?`)  
✅ Signal handling (`Ctrl-C`, `Ctrl-D`, `Ctrl-\`)  

## 🛠️ Installation  
Clone the repository and compile the project:  
```bash
git clone https://github.com/AbdoCooder/minishell.git
cd Minishell
make
./minishell
```

## 📜 Usage  
Start the shell:  
```bash
./minishell
```
Examples:  
```bash
echo "Hello, world!"
ls -l | grep minishell
export MY_VAR="42"
echo $MY_VAR
cat file.txt > output.txt
```

## ⚙️ Requirements  
- **Language**: C (compiled with `gcc -Wall -Wextra -Werror`)  
- **Libraries**: `readline`, `signal`, `unistd.h`, `stdlib.h`  
- **OS**: Linux / macOS (Unix-based systems)  

## 📖 Learning Outcomes  
🔹 Process creation and management (`fork()`, `execve()`, `waitpid()`)  
🔹 File descriptors and redirections (`dup2()`, `pipe()`)  
🔹 Signal handling (`sigaction()`, `kill()`)  
🔹 Memory management (avoiding leaks with `malloc()` and `free()`)  

## 📄 License  
This project is licensed under the **GNU General Public License v3 (GPL-3.0)**.  
You are free to use, modify, and distribute it, but **any modifications must also be open-source** under the same license.  

For full details, check the [LICENSE](https://www.gnu.org/licenses/gpl-3.0.en.html) file.  

## 👨‍💻 Authors  
- [**ABDELKADER BENAJIBA**](https://github.com/AbdoCooder)  /  [**OTHMANE ZARWAL**](https://github.com/YAGAME-YAGAME) -
