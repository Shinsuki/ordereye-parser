```markdown
# Ordereye Parser
![order](https://github.com/user-attachments/assets/74a59c25-22b9-4358-9657-aa0285e1abdf)

> Part of the **Ordereye** cybersecurity toolkit

**Ordereye Parser** is a powerful and lightweight CLI tool to filter, extract, and enumerate patterns from text files.

It's built for speed and flexibility, providing features beyond traditional tools like `grep`. This parser can extract content after a specific string, between string occurrences, or from a given character position — all while allowing enumeration and custom limits.

---

## Features

- Extract from files based on exact string patterns or character positions
- Enumerate matched lines with `-e`
- Invert match results using `-v` (only in `-l` mode)
- Extremely fast, often outperforming `grep` in simple match scenarios

---

## Usage

```bash
./ordereye-parser [mode] filename [args...]
```

### Modes

#### `-l` — Line Match (like `grep`)
Matches entire lines containing a given string.

```bash
./ordereye-parser -l test.txt "pattern"
```

Optional:
- `-e`: enumerate matched lines
- `-v`: invert match (show lines that **do not** contain the pattern)

#### `-s` — Substring Extraction (after pattern until X occurrences of Y)
Extracts everything **after** the first `pattern` up to the Nth occurrence of another `delimiter`.

```bash
./ordereye-parser -se test.txt "pattern" 1 " " 2
```

- `"pattern"` — string to search
- `1` — which occurrence of the pattern to start after
- `" "` — delimiter to stop at
- `2` — how many times to allow the delimiter to appear before stopping

#### `-p` — Position-Based Extraction
Extracts content starting from a specific character position until a delimiter appears X times.

```bash
./ordereye-parser -pe test.txt 10 ":" 1
```

- `10` — starting position (character index)
- `":"` — stop at this delimiter
- `1` — number of delimiter occurrences allowed before stopping

---

## Examples

```bash
# Simple line match
./ordereye-parser -l test.txt "admin"

# Inverted match
./ordereye-parser -lve test.txt "admin"

# Extract after "username" until 2 spaces
./ordereye-parser -se test.txt "username" 1 " " 2

# Extract from position 5 until second comma
./ordereye-parser -pe test.txt 5 "," 2
```

---

## About

Ordereye Parser is part of the **Ordereye Toolkit**, a collection of cybersecurity tools being developed by [Shinsuki](https://github.com/Shinsuki).

---

## License

MIT
```
