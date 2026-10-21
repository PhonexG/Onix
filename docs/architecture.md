# Onix Architecture & Components

## Core Subsystem (`onix::core`)

The `onix::core` module provides the fundamental data structures and text mutation algorithms. It is completely decoupled from UI/TUI rendering concerns and third-party runtime frameworks, ensuring clean portability across POSIX systems (including NixOS and Chimera Linux).

```
                      +-------------------+
                      |    TextBuffer     |  <--- Public Facade
                      +---------+---------+
                                |
             +------------------+------------------+
             |                                     |
             v                                     v
   +-------------------+                 +-------------------+
   |    PieceTable     |                 |     LineIndex     |
   | (Text storage,    |                 | (Spatial index,   |
   |  splicing, diffs) |                 |  binary search)   |
   +-------------------+                 +-------------------+
```

---

### 1. `PieceTable`
Handles text storage and non-destructive modifications via immutable original and append-only buffers.

- Header: `include/onix/core/PieceTable.hpp`
- Source: `src/core/PieceTable.cpp`

#### Internal Layout:
- `original_buffer_` (`std::string`): Immutable initial content (e.g., loaded from a file or startup parameter).
- `add_buffer_` (`std::string`): Append-only storage for newly typed or inserted content.
- `pieces_` (`std::vector<Piece>`): Sequence of descriptors pointing to spans within the buffers:
  - `BufferType buffer`: `Original` or `Add`.
  - `std::size_t start`: Starting byte index within the referenced buffer.
  - `std::size_t length`: Byte span length.

#### Core API:
- `insert(offset, text)`: Locates the piece crossing `offset`, splits it, and inserts a new `Piece` referring to `add_buffer_`.
- `remove(offset, length)`: Slices or removes piece spans within the target byte range.
- `get_text()`: Reconstructs contiguous document text.
- `size()`, `empty()`: $O(1)$ size queries backed by `total_size_`.

---

### 2. `LineIndex`
Maintains spatial line boundaries, allowing efficient two-way translation between absolute byte offsets and 2D editor coordinates.

- Header: `include/onix/core/LineIndex.hpp`
- Source: `src/core/LineIndex.cpp`

#### Data Types:
```cpp
struct Position {
    std::size_t line{0};   // 0-indexed line number
    std::size_t column{0}; // 0-indexed byte offset within the line

    auto operator<=>(const Position&) const = default;
};
```

#### Internal Layout:
- `line_starts_` (`std::vector<std::size_t>`): Monotonically increasing vector of byte offsets where lines begin. `line_starts_[0]` is always `0`.

#### Core API:
- `line_count()`: Total lines in the document ($O(1)$).
- `line_to_offset(line)`: Byte offset corresponding to the start of the given line ($O(1)$). Throws `std::out_of_range` on invalid indices.
- `offset_to_position(offset)`: Binary search (`std::upper_bound`) resolving any arbitrary byte offset into `Position{line, column}` in $O(\log N)$ time.
- `build_from(text)`: Single-pass scan recording newline offsets. Fully handles both LF (`\n`) and CRLF (`\r\n`) conventions.

---

### 3. `TextBuffer`
Public facade unifying storage and spatial querying.

- Header: `include/onix/core/TextBuffer.hpp`
- Source: `src/core/TextBuffer.cpp`

#### Core API:
- Coordinates operations between `PieceTable` and `LineIndex`.
- `insert_at(Position pos, text)`: Position-based text insertion.
- `get_line(line)`: Line extraction for viewport drawing.
- Synchronizes line index state after buffer modifications.
