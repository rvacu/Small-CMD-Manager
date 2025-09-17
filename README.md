
# 🧭 Small CMD Manager

> A minimal, shell that lets you create, inspect, and execute queued "List Nodes" of commands. Built for linked lists, string parsing, file I/O, and process execution in C.

![C](https://img.shields.io/badge/lang-C-blue)
![Build](https://img.shields.io/badge/build-Makefile-brightgreen)
![OS](https://img.shields.io/badge/OS-Linux-lightgrey)

---

## ✨ Description

**Small CMD Manager** is an interactive command-line program that shows a prompt (`SCMD$`) and accepts a small set of built-in commands.  
You can enqueue commands as **List Nodes**, open a file's contents into a node, and **execute** a node's command with its arguments.  
A rolling **history** (up to 100 items) helps you recall and re-run previous inputs.

---

## 📦 Prerequisites (with suggested versions)

> These are common, well-supported versions. Newer versions are fine, too.

- **GCC** >= 9 (or any recent GNU C compiler)
- **make** >= 4.0
- **Valgrind** >= 3.19 (recommended to check for leaks)
- **Linux environment** (e.g., server, mint)

---

## 🗂 Project Layout

```
.
├─ inc/          # headers (exec.h, listnode.h)
├─ obj/          # object files (exec.o provided)
├─ src/          # sources (mngr.c)
├─ Makefile      # build rules (produces the 'mngr' binary)
```

---

## ⚙️ Installation & Building

> Make sure the provided `obj/exec.o` and headers in `inc/` are in place (they're required for linking).

```bash
# Clean previous build artifacts (optional)
make clean

# Build
make

# Run
./mngr
```

If you want to verify memory safety during typical use:
```bash
valgrind --leak-check=full ./mngr
```

---

## 🖥️ How It Works

At the `SCMD$` prompt you can use these commands:

- `quit` - Deallocate resources and exit.
- `cd <dir>` - Change directory. Prints an error if the path is invalid or if too many args are given.
- `history` - Print prior commands (oldest at index 0). Blank lines are **not** recorded.
  - `history -c` - Clear the history.
  - `history <index>` - Re-execute the command at `<index>` and record that re-execution.
- `new CMD [args...]` - Create a **List Node** whose command is `CMD` and whose `arguments` are `CMD` followed by any args you supply. (If no args are given, prints: "too few arguments provided".)
- `list` - Print all List Nodes. Shows `List Node <id>`, its command, and its file contents (line by line) if any.
- `open <id> <filename>` - Read the file and store its **entire contents** in node `<id>`. Errors if bad id, wrong arg count, or file can't be opened.
- `execute <id>` - Run the node's command (using the provided executor). If the command's exit status is non-zero, prints the corresponding error text.

**Notes & Rules**

- Maximum **input line length**: **10,000** characters.
- History holds **up to 100** entries (oldest drops first).
- Each node may have **at most 127 arguments** (including the command itself).
- The last entry in the node's `arguments` array is `NULL` (C-style argv).
- Prompts are flushed with `fflush(stdout)` so they appear immediately.

---

## 🔡 Example Session

```text
$ ./mngr
SCMD$ new cat example.txt
SCMD$ list
List Node 0
    Command: cat
    File Contents:
SCMD$ open 0 example.txt
SCMD$ list
List Node 0
    Command: cat
    File Contents:
        <first line of example.txt>
        <second line of example.txt>
SCMD$ execute 0         # runs: cat example.txt
SCMD$ history
0: new cat example.txt
1: list
2: open 0 example.txt
3: list
4: execute 0
5: history
SCMD$ history 0         # re-run "new cat example.txt"
SCMD$ history -c        # clear history
SCMD$ quit
```

> Tip: You can `cd path/to/dir` before running `open` to simplify file paths.

---

## 🔧 Troubleshooting

- **"error: too few arguments provided"** - `new` was called without a command.
- **"error: incorrect number of arguments"** - `open` or `execute` did not receive exactly the required arguments.
- **"error: Id does not exist"** - The `<id>` you used for `open`/`execute` isn't present.
- **"error: file cannot be opened"** - Path is wrong or you lack read permissions.
- **"error: too many arguments"** - A node has > 127 arguments (including the command).
- **"error: <message>" after execute** - The executed process returned a non-zero exit code; the error string is shown.

---

## 🚧 Limits

This project is intentionally **minimal** and does **not** aim to be a full shell:

- No pipes (`|`), redirection (`>`, `<`, `>>`), globbing (`* ?`), quoting, or variable expansion.
- No job control, backgrounding, or concurrency.
- `open` loads **entire** file contents into memory (not streamed).
- Input parsing treats **whitespace** as simple delimiters; special characters are not interpreted.
- History stores **non-blank** lines only, capped at **100** entries.
- Commands are executed **per node** via a provided executor; only the **seven built-ins** listed above are recognized by the shell itself.
- Maximum per-node arguments: **127** (including the command).

---

## 🙌 Acknowledgements

- Linked list design, error formats, and executor interface can fail due to design format.
  (Any unseen errors please create a new issue).
  
- Build rules come from the provided `Makefile` (binary name: **mngr**).
