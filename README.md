# TextEditor-1.0  
A lightweight, command-driven text editor with file operations, search/replace, and advanced text manipulation.

## Overview  
This project is a command-driven text editor inspired by Vim, featuring essential file operations, search and replace functionality, and advanced text manipulation commands. It has been implemented in multiple phases to ensure robust functionality and error handling.

## Features  

### 1. File Operations  
- `:w filename` - Save the current buffer to a file.  
- `:q` - Quit (with a warning if there are unsaved changes).  
- `:q!` - Force quit without saving.  
- `:wq` - Save and quit.  
- `:e filename` - Open a file for editing.  

### 2. Search and Replace  
- `/pattern` - Search forward for a pattern.  
- `n` - Find the next occurrence.  
- `N` - Find the previous occurrence.  
- `:s/old/new` - Replace the first occurrence of `old` with `new` on the current line.  
- `:s/old/new/g` - Replace all occurrences of `old` with `new` on the current line.  

### 3. Advanced Text Operations  
- `:d N` - Delete line N.  
- `J` - Join the current line with the next line.  
- `>>` - Indent the current line.  
- `<<` - Unindent the current line.  
- Number prefixes for commands:  
  - `3dd` - Delete 3 lines.  
  - `2yy` - Yank (copy) 2 lines.  
  - `5j` - Move down 5 lines.  

### 4. Command Mode Enhancements  
- Command history using up/down arrows.  
- Error messages for invalid commands.  
- Status messages for successful operations.  

### 5. Status Line Display  
- Shows current mode (`NORMAL` or `INSERT`).  
- Displays the filename and a `+` if there are unsaved changes.  
- Displays current line and column position.  

## How to Use  
### Installation  
1. Clone the repository:  
   ```sh
   git clone https://github.com/muteeurrehman28/TextEditor-1.0.git
   ```
2. Navigate to the project directory:  
   ```sh
   cd TextEditor-1.0
   ```
3. Compile the program:  
   ```sh
   g++ -o texteditor main.cpp FileManager.cpp TextEditor.cpp Line.cpp EditorStatus.cpp -std=c++11
   ```
4. Run the editor:  
   ```sh
   ./texteditor
   ```

### Basic Usage  
- Start typing in insert mode (`i` to enter insert mode, `ESC` to exit).
- Use `:w filename` to save the file.
- Use `:q` to quit, or `:q!` to force quit without saving.
- Use `/pattern` to search, `n` to find the next occurrence, and `N` for the previous one.
- Use `:s/old/new/g` to replace all occurrences on the current line.

### Screenshots  
**Editor in action:**  
![Editor Screenshot](screenshot.png)  

## FAQs  
### 1. How do I enter and exit insert mode?  
- Press `i` to enter insert mode.
- Press `ESC` to exit insert mode.

### 2. How can I save my work?  
- Use `:w filename` to save the file with a specific name.
- If the file was previously opened, just use `:w` to save changes.

### 3. What if I want to quit without saving?  
- Use `:q!` to force quit without saving changes.

### 4. How do I search for a word?  
- Type `/pattern` and press `Enter` to search forward.
- Use `n` to find the next match and `N` to find the previous match.

### 5. How can I replace text?  
- Use `:s/old/new/` to replace the first occurrence of `old` with `new` on the current line.
- Use `:s/old/new/g` to replace all occurrences on the current line.

### 6. What if I delete something by mistake?  
- Unfortunately, undo is not yet implemented. Be cautious when deleting lines!

### 7. How do I contribute to this project?  
- Fork the repository, create a new branch, and submit a pull request with your changes.

## Implementation Details  

### File Handling  
```cpp
class FileManager {
private:
    std::string currentFileName;
    bool modified;

public:
    bool loadFile(const std::string& filename);
    bool saveFile(const std::string& filename);
    bool hasUnsavedChanges() const;
    void markAsModified();
    std::string getCurrentFileName() const;
};
```

### Search and Replace  
```cpp
class SearchEngine {
private:
    std::string lastPattern;
    size_t lastMatchLine;
    size_t lastMatchColumn;

public:
    bool search(const std::string& pattern);
    bool findNext();
    bool findPrevious();
    bool replace(const std::string& old, const std::string& newStr, bool global = false);
};
```

### Command Execution  
```cpp
class CommandMode {
private:
    std::vector<std::string> commandHistory;
    size_t historyIndex;
    std::string statusMessage;

public:
    void addToHistory(const std::string& cmd);
    std::string getPreviousCommand();
    std::string getNextCommand();
    void setStatus(const std::string& msg);
};
```

## Contribution  
We welcome contributions! Feel free to fork the repository, submit pull requests, or open issues for any bugs or feature requests.  

## License  
This project is licensed under the MIT License.  

---

