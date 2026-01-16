# epuzzle

<div align="right">

[**Русский**](README.md) | **English**

</div>

[![CI Checks (PR & Push)](https://github.com/mz-pilot/epuzzle/actions/workflows/ci_pr.yml/badge.svg)](https://github.com/mz-pilot/epuzzle/actions/workflows/ci_pr.yml)
[![Testing](https://img.shields.io/badge/Testing-Google_Test-4285F4?logo=googletest)](https://google.github.io/googletest/)
[![clang-tidy](https://img.shields.io/badge/clang--tidy-enabled-blue?logo=llvm)](https://clang.llvm.org/extra/clang-tidy/)

[![Platforms](https://img.shields.io/badge/Platforms-Windows%20%7C%20Linux-blue)](https://github.com/mz-pilot/epuzzle/actions)
[![C++ Standard](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![CMake](https://img.shields.io/badge/CMake-≥3.24-064F8C.svg)](https://cmake.org)
[![License: MIT](https://img.shields.io/badge/License-MIT-lightgrey.svg)](https://github.com/mz-pilot/epuzzle/blob/main/LICENSE)

[![GitHub release (latest by date)](https://img.shields.io/github/v/release/mz-pilot/epuzzle)](https://github.com/mz-pilot/epuzzle/releases)


Modern C++20 library and console utility for solving logic puzzles (such as [Einstein's Riddle](https://en.wikipedia.org/wiki/Zebra_Puzzle) and similar constraint satisfaction problems). 

The project demonstrates Modern C++ 20 practices, multithreading, HPC, cross-platform CMake build (Windows, Linux), unit tests via Google Test, clang-tidy, sanitizers, and CI/CD pipelines on GitHub Actions.

<details>

<summary><h3> Tech stack & Practices (expand)</h3></summary>

### Architecture, patterns, idioms, and type safety

* Pragmatic layered architecture separated into DTO, Model, and algorithms.
* Adherence to `SOLID` principles in design.
* Polymorphism: Solver, `std::variant` + `std::visit`, etc.
* Strong Typing: IndexedVector + TypedIndex.
* Invariant checks: `compiletime`, `runtime`, informative exceptions.
* GoF patterns: Strategy, Factory Method, Iterator, etc.
* RAII: `std::unique_ptr`, `std::lock_guard/std::unique_lock`. 
* Move semantics optimization.

### C++ 20

* Concurrency & Thread Cancellation: `std::jthread` and `std::stop_token` (cooperative cancellation in ParallelExecutor).
* Concepts & Constraints: `std::concepts`, `std::regular`, `std::totally_ordered` (in TypedIndex), etc.
* Attributes: `[[likely]]` - `[[unlikely]]` (branch prediction in hot loops).
* Other core: explicit template lambdas (PuzzleDefinition validation), designated initialization, user-defined literals, Spaceship operator, etc.
* Standard library: `std::ranges`, `std::views`, `std::format`, `std::source_location`, etc.

### Performance and Multithreading

* Indexed `cache friendly` data for `O(1)` access: dense packing (IndexedVector + TypedIndex, data locality).
* Prefiltering mode for bruteforce.
* `Parallel mode`: custom ParallelExecutor, `chunked queue`, `graceful shutdown`, `threads exceptions handling`.
* False Sharing Prevention: alignas `L1 Cache-line size` (in AtomicProgressTracker).
* Lock-free elements: `std::atomic` with `memory_order_relaxed` in appropriate cases.
* Profiling with `Intel VTune` was used.

### Testing and Quality

* Parameterized test suites on `Google Test` (different algorithms on shared data).
* Full coverage with synthetic puzzles 2x2, 2x3, 3x2, 3x3.
* Tests with real puzzles.
* Tests for operation cancellation and graceful shutdown.
* Automatic code analysis with `clang-tidy` and sanitizers.
* `CI/CD` with cross-platform testing (see below).

### Infrastructure

* `Cross-platform Build`: unified build system based on `Modern CMake` (`Windows`, `Linux`) with automatic packaging via `CPack` (`deb`, `tar.gz`, `zip`).
* CI/CD: `GitHub Actions workflow` with full CI/CD cycle and `build matrix` (Windows/MSVC, Linux/GCC/Clang).
* Versioning via `Git tags`, `automatic releases` and their `Smoke Test`.

</details>

## Table of Contents

* [Quick Start](#quick-start)
  * [System Requirements](#system-requirements)
  * [Installation](#installation)
  * [Creating Your Own Puzzle](#creating-your-own-puzzle)
* [Full User Guide](#full-user-guide)
  * [Working with Puzzles](#working-with-puzzles)
  * [Working with the Program](#working-with-the-program)
* [For Developers](#for-developers)
  * [Project Architecture](#project-architecture)
  * [Build and Development](#build-and-development)
  * [Roadmap](#roadmap)
* [Contact](#сontact)

## Quick Start

### System Requirements
The project is regularly tested using GitHub Actions and manually.

| OS          | Version     | Status            |
|-------------|-------------|-------------------|
| **Windows** | 10 (Home)   | ✅ Supported      |
| **Windows** | Server 2025 | ✅ Supported      |
| **Ubuntu**  | 24.04       | ✅ Supported      |
| **Debian**  | 13.2        | ✅ Supported      |

### Installation

<details>
<summary><h5> Installation on Windows (expand)</h5></summary>

#### Windows
1. **Download** the latest release `epuzzle-<version>-Windows-x64.zip` from the [Releases](https://github.com/mz-pilot/epuzzle/releases) page.
2. **Unzip** the archive to any convenient folder (e.g., `C:\epuzzle`).
3. **Open command prompt** in the extracted folder:
```cmd
cd C:\epuzzle
epuzzle.exe --help
```
4. **Run the program to solve the test puzzle**:
```cmd
epuzzle.exe --file puzzle_examples\einsteins.toml
```

</details>

<details>
<summary><h5> Installation on Linux (expand)</h5></summary>

#### Linux (Ubuntu/Debian)
**Option A — via .deb package (recommended):**
```bash
# Download the latest .deb package from the Releases page
sudo apt update
sudo apt install ./epuzzle-*.deb

# Run the program to solve the test puzzle
epuzzle --file /usr/share/epuzzle/puzzle_examples/einsteins.toml
```
**Option B — via .tar.gz archive:**
```bash
# Download and extract the archive
tar -xzf epuzzle-*-Linux-x64.tar.gz
cd epuzzle-*-Linux-x64

# Run the program to solve the test puzzle
./bin/epuzzle --file share/epuzzle/puzzle_examples/einsteins.toml
# Or install manually to the system
```

</details>

### Creating Your Own Puzzle

1. **Copy the example** to your working folder:

```bash
# Windows (in command prompt)
copy puzzle_examples\einsteins.toml my_puzzle.toml     

# Linux/macOS
cp /usr/share/epuzzle/puzzle_examples/einsteins.toml ./my_puzzle.toml
```

2. **Edit** the `my_puzzle.toml` file in any text editor.

3. **Run the program to solve your puzzle**:
```bash
epuzzle --file my_puzzle.toml
```

<div align="right"> 

[⬆ Top](#table-of-contents)

</div>


## Full User Guide

### Working with Puzzles

The distribution includes a **`puzzle_examples`** folder with examples of ready-made puzzles (`.toml` files). Each corresponds to a `.txt` file with a natural language description of the puzzle.

The easiest way to create your own puzzle is to copy one of the example puzzles (`einsteins.toml`, `five_ships.toml` or other) to your working folder and rewrite it for your own puzzle.

Puzzle examples are provided with comments (comments start with the '#' character). 

<details>
<summary><h4> More details on the TOML puzzle format (expand)</h4></summary>

The puzzle file uses [TOML](https://toml.io/) syntax and has the following structure:
```toml
# Mandatory format version
epuzzle_format_version = 1

# Attributes (all strings must be unique within an attribute)

# Mandatory attribute - person
person         = ["Englishman", "Spaniard", "Ukrainian", "Norwegian", "Japanese"]

# Optional attributes
house_color    = ["red", "green", "white", "yellow", "blue"]
house_position = ["1", "2", "3", "4", "5"]
pet            = ["dog", "snails", "fox", "horse", "zebra"]
# ... you can add any attributes

# Constraints
# ... you can add many constraints
```
**Important:** All attributes must have the same number of elements (5 in the example above).

#### Constraints

**Type "fact"** - links two values:
```toml
[[constraints]]
type = "fact"
first = { person = "Englishman" }
second = { house_color = "red" }
```

Negation is indicated by the prefix `!` before the value:
```toml
[[constraints]]
type = "fact"
first = { color = "green" }
second = { hobby = "!books" }
# Reads: "The one who has green color does NOT read books"
```

**Type "comparison"** - compares positions of values:
```toml
[[constraints]]
type = "comparison"
first = { person = "Norwegian" }
compare_by = "house_position"  # attribute for comparing positions
relation = "adjacent"          # relation type
second = { house_color = "blue" }
```

Available relations:
- `"immediate_left"` — immediately to the left
- `"immediate_right"` — immediately to the right  
- `"adjacent"` — adjacent (on either side)
- `"before"` — somewhere to the left
- `"after"` — somewhere to the right

</details>


### Working with the Program
The program has a CLI (Command Line Interface) and is controlled by command-line arguments.

<details>
<summary><h4> More details on CLI Arguments (expand)</h4></summary>

```bash
# Basic syntax
epuzzle --file <FILE> [--method <METHOD>] [--prefilter <ON/OFF>] [--execpolicy <POLICY>]

# All available options
epuzzle --help
```

**Mandatory arguments:**
- `-f, --file <FILE>` — path to the puzzle data file (.toml)

**Optional arguments:**
- `-m, --method <METHOD>` — solution method: `BruteForce` (default) or `Deductive` (in development)
- `-p, --prefilter <MODE>` — prefiltering (only for BruteForce): `Enabled` (default) or `Disabled`
- `-e, --execpolicy <POLICY>` — execution policy (only for BruteForce): `Parallel` (default) or `Sequential`
- `-v, --version` — show program version
- `-h, --help` — show help

**Usage examples:**
```bash
# Quick check (default mode)
epuzzle --file puzzle_examples/einsteins.toml

# Parallel brute force
epuzzle --file riddle.toml --method BruteForce --execpolicy Parallel

# Single-threaded brute force without prefiltering
epuzzle -f test.toml -m BruteForce -p Disabled -e Sequential
```

#### Description of operating modes

##### Solution Methods (`--method`)
- *BruteForce* — full exhaustion of all possible variants. Guarantees finding all solutions.
- *Deductive* — logical inference (in development).

##### Prefiltering (`--prefilter`, only for BruteForce)
- *Enabled* (default) — excludes obviously incorrect variants at the preparation stage (speeds up work by 10-100 times).
- *Disabled* — full exhaustion without optimizations (use for correctness verification).

##### Execution Policy (`--execpolicy`, only for BruteForce)
- *Parallel* (default) — multithreaded processing (uses all available CPU cores).
- *Sequential* — single-threaded processing (useful for debugging).

</details>


<details>
<summary><h4> Results Interpretation (expand)</h4></summary>

The program outputs:
1. **Information about the loaded puzzle** — list of attributes and constraints
2. **Solution progress** — completion percentage
3. **Found solutions** — each solution is presented as a table

**Solution output format:**
```bash
Solutions found: 1
Solution #1:
  person          Englishman  Spaniard          Ukrainian       Norwegian   Japanese
  beverage        milk        orange juice      tea             water       coffee
  cigarettes      Old Gold    Lucky Strike      Chesterfield    Kool        Parliament
  house_color     red         white             blue            yellow      green
  house_position  3           4                 2               1           5
  pet             snails      dog               horse           fox         zebra
```
Values in a column are attributes belonging to one person.

</details>

<details>
<summary><h4> Troubleshooting / Possible Issues (expand)</h4></summary>

#### TOML format errors
The program displays the approximate location of the error (line, symbol). Check:
- Correctness of TOML syntax (you can use an [online validator](https://www.toml-lint.com/))
- Compliance with the format version (`epuzzle_format_version = 1`)
- Uniqueness of values within each attribute

#### Large puzzles
The BruteForce method may require significant memory and time for a large number of combinations. Recommendations:
- Use `--prefilter Enabled` (enabled by default)
- For very large tasks, it is preferable to use Deductive methods

#### File not found
Ensure the file path is specified correctly. On Windows, use double quotes if the path contains spaces:
```cmd
epuzzle.exe --file "C:\My Puzzles\riddle.toml"
```

</details>

<div align="right"> 

[⬆ Top](#table-of-contents)

</div>

## For Developers

### Project Architecture

The `epuzzle` project is built on a pragmatic layered architecture separated into DTO (Data Transfer Objects - PuzzleDefinition, PuzzleSolution), Model (PuzzleModel, SolutionModel), and algorithms:

![Diagram](docs/images/epuzzle_arch.png)

<details>
<summary><h4> Basic Interfaces (expand)</h4></summary>

#### Solver Interface
The core logic is built around the abstract `Solver` interface, allowing new solution algorithms to be easily added:
```cpp
namespace epuzzle
{
    // The main interface for client code.
    class Solver
    {
    public:
        static std::unique_ptr<Solver> create(SolverConfig, PuzzleDefinition);
        virtual ~Solver() = default;

        // Parameter object. The callback executes in solve()'s calling thread. Return false to cancel the operation.
        struct SolveOptions
        {
            std::chrono::milliseconds progressInterval = std::chrono::milliseconds(1000);
            std::function<bool(std::uint64_t total, std::uint64_t current)> progressCallback = [](auto, auto) { return true; };
        };
        virtual std::vector<PuzzleSolution> solve(const SolveOptions&) = 0;
    };
}
```

#### Solver Configuration (SolverConfig)
Configuration uses modern C++ types for flexible parameter description:
```cpp
struct SolverConfig 
{
    enum class SolvingMethod : std::uint8_t { BruteForce, Deductive };

    struct BruteForceConfig 
    {
        enum class ExecPolicy : std::uint8_t { Sequential, Parallel };
        bool prefilter = true;
        ExecPolicy execution = ExecPolicy::Parallel;
    };

    SolvingMethod solvingMethod = SolvingMethod::BruteForce;
    std::optional<BruteForceConfig> bruteForce;
};
```
**Principle:** Configuration is validated at the solver creation stage, ensuring correct state before calculations begin.

#### Extensibility: steps to add a new solver:

1. **Create a class** inheriting from `Solver`, see e.g. `DeductiveSolver`:
```cpp
namespace epuzzle::details
{
    class DeductiveSolver final : public Solver
    {
    public:
        explicit DeductiveSolver(PuzzleModel&&);
        std::vector<PuzzleSolution> solve(const SolveOptions&) override;
    };
}
```
2. **Add a corresponding method** to `SolverConfig::SolvingMethod`
3. **Extend the factory method** `Solver::create()`
4. **Add to parameterized tests** (see section below).

**Current status:** The project already contains a stub for `DeductiveSolver`, demonstrating the ready-made extension path.

</details>


<details>
<summary><h4> Parameterized Tests (expand)</h4></summary>

The testing system uses **Google Test** with an emphasis on parameterized test suites, providing maximum coverage and simplifying the verification of new algorithms.

#### Testing Strategy
```cpp
// Tests are not tied to a specific solver, e.g., SolverTests for BruteForce and for Deductive:
    INSTANTIATE_TEST_SUITE_P(
        SolverBruteForce,
        SolverTests,
        testing::Values(SolverConfig{ Method::BruteForce, BFConfig{.prefilter = true, .execution = ExecPolicy::Sequential} }));

    INSTANTIATE_TEST_SUITE_P(
        SolverDeductive,
        SolverTests,
        testing::Values(SolverConfig{ Method::Deductive, {} }));
```

#### Three-level verification system:
1. **Synthetic puzzles** (2x2, 3x3, etc.)
   - Dozens of tests with a known number of solutions
   - Verification of basic logic and edge cases
   - Building "trust" in the reference solver (`BruteForce`)

2. **Real puzzles** (Einstein's Riddle, etc.)
   - Comparison with known solutions
   - Verification of correctness with real data

3. **New solvers**
   - Automatic verification on the entire array of tests
   - Comparison of results with the reference `BruteForce` solver

**Approach advantages:**
- A new algorithm is immediately checked on multiple test cases
- Easy addition of tests for specific cases
- Ability to compare performance of different algorithms

As a result, we get an excellent ecosystem for working on algorithms.

</details>


<details>
<summary><h4> Multithreaded Architecture (expand)</h4></summary>

**Data flow in the parallel solver (BruteForce, Parallel mode):**
```text
Main Thread
    ↓
Creates ParallelExecutor with N threads (equal to CPU core count)
    ↓
Each worker thread:
    1. Requests new chunk from SpaceSplitter
    2. Checks each candidate in the chunk (hot loop!)
    3. Saves matching solutions locally
    4. Updates AtomicProgressTracker
    ↓
Main thread periodically polls progress
    and calls progressCallback
    ↓
On completion: results are merged, exceptions are handled
```

</details>


<details>
<summary><h4> Strong Typing (expand)</h4></summary>

The project actively uses C++20 features to prevent a whole class of errors at compile time.

#### Strong types for indices (TypedIndex)
Instead of "naked" indices, wrappers are used to prevent logical errors:
```cpp
// Index category - part of the type
using AttributeTypeID = utils::TypedIndex<struct AttributeTypeID_tag>;
using AttributeValueID = utils::TypedIndex<struct AttributeValueID_tag>;
using PersonID = utils::TypedIndex<struct PersonID_tag>;
```
**Implementation features:**
- Full compatibility with STL (`std::regular`, `std::totally_ordered`)
- Support for `<=>`
- `constexpr` semantics for compile-time calculations
- Safe creation from iterators via `fromDistance()`

#### Safe Containers (IndexedVector)
Wrapper over `std::vector` with index type checking:
```cpp
using AttributeAssignment = utils::IndexedVector<AttributeValueID, PersonID>; // Single combination values of single attribute

Assignment assignment;
assignment.emplace_back(PersonID{}); // Ok
// assignment.emplace_back(AttributeTypeID{}); // compiler error!

assignment[AttributeValueID{}] = PersonID{}; // Ok
// assignment[PersonID{}] = PersonID{}; // compiler error!
```

#### Constraints System Extension

The architecture supports adding new constraint types via static typing. Basic steps:
1. Add a new type to `std::variant` in the `PuzzleDefinition::constraints` definition
2. Follow compilation errors to implement the necessary processing logic
3. Add corresponding tests

The system guarantees correctness thanks to checks at the compilation stage.

</details>

<div align="right"> 

[⬆ Top](#table-of-contents)

</div>


### Build and Development

The project uses a standard workflow based on Modern CMake, providing simple building and testing on any supported OS. You can clone the repository and experiment with the code using familiar tools.

<details>
<summary><h4> Windows (Visual Studio 2022) (expand)</h4></summary>

1. **Install**:
   - [Visual Studio 2022](https://visualstudio.microsoft.com/)
   - Component "Desktop development with C++"
   - Component "C++ CMake tools for Windows"
2. **Clone the repository**:
```bash
git clone https://github.com/mz-pilot/epuzzle.git
cd epuzzle
```
3. **Open folder** in Visual Studio: `File → Open → Folder...`
4. **Select configuration** in the top panel:
   - `windows-x64` (will configure automatically via `CMakePresets.json`); wait for configuration to finish (monitor in Output window)
5. **Build the project**: `Build → Build All`
6. **Run tests**: via menu `Test → Test Explorer`

#### Cross-platform development with WSL2 (Windows)
1. **Install WSL2** and `Ubuntu` distribution:
```powershell
wsl --install -d Ubuntu
```
2. **In Visual Studio** connect to WSL:
   - Open the project
   - In the configuration dropdown select "Manage Configurations"
   - Add a configuration for Linux
3. **Debug** code running in WSL directly from Visual Studio

<small>Note: for similar use of `Debian` in Visual Studio with WSL2, you may need to manually launch its terminal after installation and install `rsync` with `sudo apt install -y rsync` </small>

</details>


<details>
<summary><h4> Linux (expand)</h4></summary>

1. **Install dependencies**:
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install build-essential cmake ninja-build git clang clang-tidy clang-format clangd
```
2. **Clone and build**:
```bash
git clone https://github.com/mz-pilot/epuzzle.git
cd epuzzle
cmake --preset=linux-release
cmake --build --preset=lin-release
ctest --preset=test-lin-release
```

</details>

<details>
<summary><h4> Alternative IDEs (expand)</h4></summary>

- **Qt Creator (Linux/Windows)** - project opens in Qt Creator as "CMake Project":
```
    1. File -> Open File or Project...
    2. Select the project's root `CMakeLists.txt`.
    3. In Configure Project menu select the desired Kit, e.g., Desktop GCC.
    4. Click Configure.
    5. Use `Ctrl+R` to build and run or `F5` to debug.
```
- **VS Code** — with CMake Tools and C++ extensions
- **CLion** — works great with CMakePresets
- **Terminal + favorite editor** — use CMake presets

</details>

#### Build Specifics
- **Release builds include debug info** - (`/DEBUG` in MSVC, `-g` in GCC/Clang) for easier diagnostics
- **Frame pointer omission disabled** - (`/Oy-` in MSVC, `-fno-omit-frame-pointer` in GCC/Clang) - also for easier diagnostics
- **Static runtime linking** on Windows — `epuzzle.exe` does not depend on MSVC Redist


#### Running code analysis with clang-tidy and sanitizers
Performed in two ways:
- **Automatically on GitHub** - within `CI Checks (PR & Push)`.
- **Manually locally** - on the developer machine, you can use the `linux-tidy` preset in Linux/WSL:
  - in IDE
  - or in terminal:
  ```bash
  cd epuzzle
  cmake --preset=linux-tidy
  cmake --build --preset=lin-tidy  # clang-tidy
  ctest --preset=test-lin-tidy     # sanitizers
  ```

Note: `WarningsAsErrors` strict setting is used for the linter.

#### Running Tests

```bash
ctest --preset=test-lin-release  # Linux
ctest --preset=test-win-release  # Windows

# With verbose output
ctest -V --preset=test-lin-release
```

<details>
<summary><h4> Known issue with Google Test adapter in Visual Studio (expand)</h4></summary>

**Known issue with Google Test adapter in Visual Studio:**
When using the Google Test adapter for Visual Studio, tests marked as `GTEST_SKIP()` may erroneously show as failed in Test Explorer. This is a [known issue](https://developercommunity.visualstudio.com/t/googletest-skipped-tests-are-reported-as-failed-in/884416) of Microsoft.

**Recommended workarounds:**
* Use WSL2 — the issue does not appear in the Linux environment
* Use CTest to run tests for this project
* Create your own Playlist for Google Test Adapter, excluding skipped tests

**Why `GTEST_SKIP()` is used:** To temporarily disable tests for functionality not yet implemented (e.g., `DeductiveSolver`).

</details>

#### Debugging
- **Visual Studio** — use the built-in debugger; breakpoints work for both Windows and WSL
- **Qt Creator**, **VS Code**, **CLion** - you can use the debugger in the usual way
- **Via command line** - go to the folder with the built executable and run the debugger:
```bash
# Linux
cd out/build/linux-debug/bin/apps
gdb ./epuzzle

# Windows (if Debugging Tools for Windows is installed)
cd out/build/windows-x64/bin/apps
cdb epuzzle.exe
```

<div align="right"> 

[⬆ Top](#table-of-contents)

</div>


### Roadmap

#### Version 1.1 (in development)
- [ ] Implementation of `DeductiveSolver` (logical inference)
- [ ] Addition of new constraint types to expand the class of solvable tasks

#### Development Strategy
- **BruteForce solver** will remain the reference for verifying the correctness of new algorithms
- **Deductive solvers** will be developed for efficient solving of large puzzles (6x6 and larger)
- **Testing of new algorithms** will be conducted including comparison of results with BruteForce

<div align="right"> 

[⬆ Top](#table-of-contents)

</div>

---

## Contact

Contact the author:  [mzexe@mail.ru](mailto:mzexe@mail.ru)

The project was created as a demonstration of professional skills in modern C++ development. Open for study, use, and forks. Thank you for your interest in the project!

<div align="right"> 

[⬆ Top](#table-of-contents)

</div>
