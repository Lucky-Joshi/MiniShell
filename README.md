## 🐚 Lucky MiniShell

> A lightweight, single-file Windows terminal emulator written in **pure C++**.
> Minimal, colorful, and powerful — all inside `main.cpp`.

---

### 📦 Features

* 🔤 Custom shell prompt: `Lucky@minishell$`
* 🎤 Full command parsing with background execution (`&`)
* ⟳ Command chaining with `&&`
* 🧠 In-memory command history with ↑ / ↓ navigation
* 🌈 ANSI-colored prompt and help messages (PowerShell / Terminal supported)

---

### ⚙️ Built-In Commands

| Command        | Description                |
| -------------- | -------------------------- |
| `pwd`          | Print working directory    |
| `cd <dir>`     | Change directory           |
| `mkdir <name>` | Create a directory         |
| `rmdir <name>` | Remove a directory         |
| `ls` / `dir`   | List contents of directory |
| `cat <file>`   | Show contents of a file    |
| `touch <file>` | Create an empty file       |
| `echo <msg>`   | Print message              |
| `clear`        | Clear the screen           |
| `date`         | Show system date and time  |
| `whoami`       | Show current user          |
| `history`      | Show command history       |
| `help`         | List all built-in commands |
| `exit`         | Exit the shell             |

---

### 🧐 Extra Features

* ⟳ Command history scroll with ↑ / ↓ arrow keys
* 🔗 Command chaining:
  Example: `cd test && dir && echo Done`
* ⚙️ Background task support with `&`:
  Example: `notepad &`
* ✅ UTF-8 support for extended characters

---

### 🚀 Getting Started

#### 🔧 Requirements

* Windows OS (CMD or PowerShell)
* [MinGW](https://www.mingw-w64.org/) or compatible C++17+ compiler

#### 🛠️ Compile & Run

```bash
g++ main.cpp -o minishell.exe
.\minishell.exe
```

---

### 📸 Sample Session

```plaintext
Lucky@minishell$ pwd
C:\Users\Lucky\MiniShell

Lucky@minishell$ mkdir testdir && cd testdir && echo Hello > msg.txt

Lucky@minishell$ ls
msg.txt

Lucky@minishell$ cat msg.txt
Hello

Lucky@minishell$ date
2025-07-24 17:15:52

Lucky@minishell$ history
1: pwd
2: mkdir testdir && cd testdir && echo Hello > msg.txt
3: ls
4: cat msg.txt

Lucky@minishell$ exit
Goodbye! 👋
```

---

### 🔪 Tested On

* ✅ Windows 10 CMD
* ✅ PowerShell 7+
* ✅ Windows Terminal (best with ANSI colors)

---

### 📁 File Structure

```
.
├── main.cpp      # Entire shell logic
└── README.md     # You're reading it!
```

---

### 💡 Ideas for Future

* [ ] Command auto-completion
* [ ] Command output redirection (`>`, `>>`)
* [ ] Environment variable support (`%PATH%`)
* [ ] Configurable shell prompt
* [ ] Tab completion for filenames

---

### 🧑‍💻 Author

**Lucky Joshi**
Student, Developer, Programmer
GitHub: *\[https://github.com/Lucky-Joshi]*

---

### 📜 License

This project is released under the **MIT License** — use it freely, modify it boldly.
