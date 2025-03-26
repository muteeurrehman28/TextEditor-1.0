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

## Error Handling  

### File Operations  
- Handle errors like:  
  - File not found.  
  - Permission denied.  
  - Disk full.  
  - File already exists.  

```cpp
void handleFileError(const std::string& operation, const std::string& filename, int errorCode);
```

### Search and Replace  
- Handle cases such as:  
  - Pattern not found.  
  - Invalid pattern.  
  - Empty search string.  

```cpp
void handleSearchError(const std::string& pattern, SearchErrorType error);
```

### Command Mode  
- Detect invalid command syntax, non-existent line numbers, and invalid operations.  

```cpp
void handleCommandError(const std::string& cmd, CommandErrorType error);
```

## Testing  

### File Operation Tests  
```cpp
void testFileOperations() {
    assert(editor.loadFile("test.txt"));
    editor.insertText("Test content");
    assert(editor.saveFile("output.txt"));
    assert(editor.hasUnsavedChanges());
}
```

### Search Tests  
```cpp
void testSearch() {
    editor.loadFile("test.txt");
    assert(editor.search("pattern"));
    assert(editor.findNext());
    assert(editor.replace("old", "new"));
}
```

### Command Tests  
```cpp
void testCommands() {
    editor.executeCommand(":d 5");
    editor.executeCommand("3dd");
    editor.executeCommand(">>");
}
```

## Contribution  
We welcome contributions! Feel free to fork the repository, submit pull requests, or open issues for any bugs or feature requests.  

## License  
This project is licensed under the MIT License.  

---
