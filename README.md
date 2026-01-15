# epuzzle

[![CI Checks (PR & Push)](https://github.com/mz-pilot/epuzzle/actions/workflows/ci_pr.yml/badge.svg)](https://github.com/mz-pilot/epuzzle/actions/workflows/ci_pr.yml)
[![Testing](https://img.shields.io/badge/Testing-Google_Test-4285F4?logo=googletest)](https://google.github.io/googletest/)
[![clang-tidy](https://img.shields.io/badge/clang--tidy-enabled-blue?logo=llvm)](https://clang.llvm.org/extra/clang-tidy/)

[![Platforms](https://img.shields.io/badge/Platforms-Windows%20%7C%20Linux-blue)](https://github.com/mz-pilot/epuzzle/actions)
[![C++ Standard](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![CMake](https://img.shields.io/badge/CMake-≥3.24-064F8C.svg)](https://cmake.org)
[![License: MIT](https://img.shields.io/badge/License-MIT-lightgrey.svg)](https://github.com/mz-pilot/epuzzle/blob/main/LICENSE)

[![GitHub release (latest by date)](https://img.shields.io/github/v/release/mz-pilot/epuzzle)](https://github.com/mz-pilot/epuzzle/releases)


Modern C++20 библиотека и консольная утилита для решения логических головоломок (таких как [Загадка Эйнштейна](https://ru.wikipedia.org/wiki/Загадка_Эйнштейна) ([Zebra Puzzle](https://en.wikipedia.org/wiki/Zebra_Puzzle)) и аналогичных задач на удовлетворение ограничений). 

Проект демонстрирует практики Modern C++ 20, многопоточность, HPC, кроссплатформенную сборку CMake (Windows, Linux), модульные тесты на google test и CI/CD pipelines на GitHub Actions.

<details>

<summary><h3> Развернуть >> Technical Highlights </h3></summary>

### Архитектура, паттерны, идиомы и безопасность типов

* Прагматичная слоистая архитектура с разделением на DTO, Model и алгоритмы.
* Опора на `SOLID` при проектировании.
* Polymorphism: Solver, `std::variant` + `std::visit`, и др.
* Strong Typing: IndexedVector + TypedIndex.
* Проверки инвариантов: `compiletime`, `runtime`, информативные исключения.
* Паттерны GoF: стратегия, фабричный метод, итератор, посредник и др.
* RAII: `std::unique_ptr`, `std::lock_guard/std::unique_lock`. 
* Move semantics optimization.

### C++ 20

* Concurrency & Thread Cancellation: `std::jthread` и `std::stop_token` (кооперативная отмена в ParallelExecutor).
* Concepts & Constraints: `std::concepts`, `std::regular`, `std::totally_ordered` (в TypedIndex) и др.
* Attributes: `[[likely]]` - `[[unlikely]]` (предсказания переходов в горячих циклах).
* Other core: explicit template lambdas (валидация PuzzleDefinition), designated initialization, user-defined literals, Spaceship operator и др.
* Standard library: `std::ranges`, `std::views`, `std::format`, `std::source_location` и др.

### Производительность и многопоточность

* Indexed `cache friendly` данные для `O(1)` доступа: плотная упаковка (IndexedVector + TypedIndex, data locality).
* Prefiltering mode for bruteforce.
* `Parallel mode`: собственный ParallelExecutor, `chunked queue`, `graceful shutdown`, `threads exceptions handling`.
* False Sharing Prevention: alignas `L1 Cache-line size` (в AtomicProgressTracker).
* Lock-free элементы: `std::atomic` с `memory_order_relaxed` в уместных кейсах.
* Использовалось профилирование `Intel VTune`

### Тестирование и качество

* Параметризованные тестовые наборы на `GoogleTest` (разные алгоритмы на общих данных).
* Полное покрытие синтетическими задачами 2x2, 2x3, 3x2, 3x3.
* Тесты с реальными головоломками.
* Тесты на отмену операции и graceful shutdown.
* Автоматический анализ кода линтером `clang-tidy` и санитайзерами.
* `CI/CD` с кроссплатформенным тестированием (см. далее).

### Инфраструктура

* `Cross-platform Build`: единая система сборки на `Modern CMake` (`Windows`, `Linux`) с автоматическим пакетированием `CPack` (`deb`, `tar.gz`, `zip`).
* CI/CD: `GitHub Actions workflow` с полным циклом CI/CD и `матрицей сборки` (Windows/MSVC, Linux/GCC).
* Версионирование через `Git tags`, `автоматические релизы` и их `Smoke Test`.

</details>

## Содержание

* [Быстрый старт](#быстрый-старт)
  * [Системные требования](#системные-требования)
  * [Установка](#установка)
  * [Создание своего пазла](#создание-своего-пазла)
* [Полное руководство пользователя](#полное-руководство-пользователя)
  * [Работа с пазлами](#работа-с-пазлами)
  * [Работа с программой](#работа-с-программой)
* [Для разработчиков](#для-разработчиков)
  * [Архитектура проекта](#архитектура-проекта)
  * [Сборка и разработка](#сборка-и-разработка)
  * [Roadmap](#roadmap)

## Быстрый старт

### Системные требования
Проект регулярно тестируется с помощью GitHub Actions и вручную.

| ОС          | Версия      | Статус            |
|-------------|-------------|-------------------|
| **Windows** | 10 (Home)   | ✅ Поддерживается |
| **Windows** | Server 2025 | ✅ Поддерживается |
| **Ubuntu**  | 24.04       | ✅ Поддерживается |
| **Debian**  | 13.2        | ✅ Поддерживается |

### Установка

<details>
<summary><h5> Развернуть >> Установка в Windows</h5></summary>

#### Windows
1. **Скачайте** последний релиз `epuzzle-<версия>-Windows-x64.zip` со страницы [Releases](https://github.com/mz-pilot/epuzzle/releases)
2. **Распакуйте** архив в любую удобную папку (например, `C:\epuzzle`)
3. **Откройте командную строку** в распакованной папке:
```cmd
cd C:\epuzzle
epuzzle.exe --help
```
4. **Запустите решение тестового пазла**:
```cmd
epuzzle.exe --file puzzle_examples\einsteins.toml
```

</details>

<details>
<summary><h5> Развернуть >> Установка в Linux</h5></summary>

#### Linux (Ubuntu/Debian)
**Вариант A — через .deb пакет (рекомендуется):**
```bash
# Скачайте последний .deb пакет со страницы Releases
sudo apt update
sudo apt install ./epuzzle-*.deb

# Запустите решение тестового пазла
epuzzle --file /usr/share/epuzzle/puzzle_examples/einsteins.toml
```
**Вариант B — через .tar.gz архив:**
```bash
# Скачайте и распакуйте архив
tar -xzf epuzzle-*-Linux-x64.tar.gz
cd epuzzle-*-Linux-x64

# Запустите решение тестового пазла
./bin/epuzzle --file share/epuzzle/puzzle_examples/einsteins.toml
# Или установите в систему вручную
```

</details>

### Создание своего пазла

1. **Скопируйте пример** в рабочую папку:

```bash
# Windows (в командной строке)
copy puzzle_examples\einsteins.toml my_puzzle.toml    

# Linux/macOS
cp /usr/share/epuzzle/puzzle_examples/einsteins.toml ./my_puzzle.toml
```

2. **Отредактируйте** файл `my_puzzle.toml` в любом текстовом редакторе

3. **Запустите решение пазла**:
```bash
epuzzle --file my_puzzle.toml
```

<div align="right"> 

[⬆ Наверх](#содержание)

</div>


## Полное руководство пользователя

### Работа с пазлами

В дистрибутив включена папка **`puzzle_examples`** c примерами готовых пазлов (файлы `.toml`). Каждому соответствует `.txt` файл с описанием головоломки на естественном языке.

Самый простой способ составить свой собственный пазл - скопировать один из пазлов-примеров (`einsteins.toml` или `four_girls.toml`) в рабочую папку и переписать его под свою собственную головоломку.

Примеры пазлов снабжены комментариями (комментарий начинается с символа '#'). 

<details>
<summary><h4> Развернуть >> Подробнее про TOML-формат пазла</h4></summary>

Файл пазла использует синтаксис [TOML](https://toml.io/) и имеет следующую структуру:
```toml
# Обязательная версия формата
epuzzle_format_version = 1

# Атрибуты (все строки должны быть уникальными в пределах атрибута)

# Обязательный атрибут - персонажи
person         = ["Англичанин", "Испанец", "Украинец", "Норвежец", "Японец"]

# Опциональные атрибуты
house_color    = ["красный", "зелёный", "белый", "жёлтый", "синий"]
house_position = ["1", "2", "3", "4", "5"]
pet            = ["собака", "улитки", "лиса", "лошадь", "зебра"]
# ... можно добавлять любые атрибуты

# Условия (constraints)
# ... можно добавлять много условий
```
**Важно:** Все атрибуты должны иметь одинаковое количество элементов (5 в примере выше).

#### Условия (Constraints)

**Тип "fact" (факт)** - cвязывает два значения:
```toml
[[constraints]]
type = "fact"
first = { person = "Англичанин" }
second = { house_color = "красный" }
```

Отрицание указывается префиксом `!` перед значением:
```toml
[[constraints]]
type = "fact"
first = { color = "green" }
second = { hobby = "!books" }
# Читается: "Тот, у кого зеленый цвет, НЕ читает книги"
```

**Тип "comparison" (сравнение)** - cравнивает позиции значений:
```toml
[[constraints]]
type = "comparison"
first = { person = "Норвежец" }
compare_by = "house_position"  # атрибут для сравнения позиций
relation = "adjacent"          # тип отношения
second = { house_color = "синий" }
```

Доступные отношения (relation):
- `"immediate_left"` — непосредственно слева
- `"immediate_right"` — непосредственно справа  
- `"adjacent"` — рядом (с любой стороны)
- `"before"` — где-то левее
- `"after"` — где-то правее

</details>


### Работа с программой
Программа имеет CLI (Command Line Interface) и управляется аргументами командной строки.

<details>
<summary><h4> Развернуть >> Подробнее про Аргументы CLI</h4></summary>

```bash
# Основной синтаксис
epuzzle --file <ФАЙЛ> [--method <МЕТОД>] [--prefilter <ВКЛ/ВЫКЛ>] [--execpolicy <ПОЛИТИКА>]

# Все доступные опции
epuzzle --help
```

**Обязательные аргументы:**
- `-f, --file <ФАЙЛ>` — путь к файлу с данными пазла (.toml)

**Опциональные аргументы:**
- `-m, --method <МЕТОД>` — метод решения: `BruteForce` (по умолчанию) или `Deductive` (в разработке)
- `-p, --prefilter <РЕЖИМ>` — предфильтрация (только для BruteForce): `Enabled` (по умолчанию) или `Disabled`
- `-e, --execpolicy <ПОЛИТИКА>` — политика выполнения (только для BruteForce): `Parallel` (по умолчанию) или `Sequential`
- `-v, --version` — показать версию программы
- `-h, --help` — показать справку

**Примеры использования:**
```bash
# Быстрая проверка (режим по умолчанию)
epuzzle --file puzzle_examples/einsteins.toml

# Параллельный перебор
epuzzle --file загадка.toml --method BruteForce --execpolicy Parallel

# Однопоточный перебор без предфильтрации
epuzzle -f test.toml -m BruteForce -p Disabled -e Sequential
```

#### Описание режимов работы

##### Методы решения (`--method`)
- *BruteForce* — полный перебор всех возможных вариантов. Гарантирует нахождение всех решений.
- *Deductive* — логический вывод (в разработке).

##### Предфильтрация (`--prefilter`, только для BruteForce)
- *Enabled* (по умолчанию) — исключает заведомо неверные варианты на этапе подготовки (ускоряет работу в 10-100 раз).
- *Disabled* — полный перебор без оптимизаций (используйте для проверки корректности).

##### Политика выполнения (`--execpolicy`, только для BruteForce)
- *Parallel* (по умолчанию) — многопоточная обработка (использует все доступные ядра CPU).
- *Sequential* — однопоточная обработка (удобно для отладки).

</details>


<details>
<summary><h4> Развернуть >> Интерпретация результатов</h4></summary>

Программа выводит:
1. **Информацию о загруженном пазле** — список атрибутов и условий
2. **Прогресс решения** — процент выполнения
3. **Найденные решения** — каждое решение представлено в виде таблицы

**Формат вывода решений:**
```bash
Solutions found: 1
Solution #1:
  person          Англичанин  Испанец           Украинец      Норвежец  Японец
  beverage        молоко      апельсиновый сок  чай           вода      кофе
  cigarettes      Old Gold    Lucky Strike      Chesterfield  Kool      Parliament
  house_color     красный     белый             синий         жёлтый    зелёный
  house_position  3           4                 2             1         5
  pet             улитки      собака            лошадь        лиса      зебра
```
Каждый столбец соответствует одной позиции (например, дому в загадке Эйнштейна). Значения в столбце — атрибуты, принадлежащие одному персонажу.

</details>

<details>
<summary><h4> Развернуть >> Возможные проблемы и решения</h4></summary>

#### Ошибки формата TOML
Программа выводит примерное местоположение ошибки (строка, символ). Проверьте:
- Корректность синтаксиса TOML (можно использовать [онлайн-валидатор](https://www.toml-lint.com/))
- Соответствие версии формата (`epuzzle_format_version = 1`)
- Уникальность значений в пределах каждого атрибута

#### Большие пазлы
BruteForce метод может потребовать значительного объема памяти и времени при большом количестве комбинаций. Рекомендации:
- Используйте `--prefilter Enabled` (включен по умолчанию)
- Для очень больших задач предпочтительнее использовать Deductive методы решения

#### Файл не найден
Убедитесь, что путь к файлу указан правильно. В Windows используйте двойные кавычки, если путь содержит пробелы:
       epuzzle.exe --file "C:\My Puzzles\загадка.toml"

</details>

<div align="right"> 

[⬆ Наверх](#содержание)

</div>

## Для разработчиков

### Архитектура проекта

Проект `epuzzle` построен на прагматичной слоистой архитектуре с разделением на DTO (Data Transfer Objects - PuzzleDefinition, PuzzleSolution), Model (PuzzleModel, SolutionModel) и алгоритмы:

![Схема](docs/images/epuzzle_arch.png)

<details>
<summary><h4> Развернуть >> Базовые интерфейсы</h4></summary>

#### Интерфейс решателя (Solver)
Ядро логики построено вокруг абстрактного интерфейса `Solver`, что позволяет легко добавлять новые алгоритмы решения:
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

#### Конфигурация решателя (SolverConfig)
Конфигурация использует современные типы C++ для гибкого описания параметров:
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
**Принцип:** Конфигурация валидируется на этапе создания решателя, что гарантирует корректность состояния до начала вычислений.

#### Расширяемость: шаги для добавления нового решателя:

1. **Создать класс**, наследующий от `Solver`, см. например `DeductiveSolver`:
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
2. **Соответствующий метод** в `SolverConfig::SolvingMethod`
3. **Расширить фабричный метод** `Solver::create()`
4. **Добавить в параметризованные тесты** (см. раздел ниже).

**Текущий статус:** Проект уже содержит заглушку для `DeductiveSolver`, что демонстрирует готовый путь расширения.

</details>


<details>
<summary><h4> Развернуть >> Параметризованные тесты</h4></summary>

Система тестирования использует **Google Test** с акцентом на параметризованные тестовые наборы, это даёт максимальное покрытие и упрощает проверку новых алгоритмов.

#### Стратегия тестирования
```cpp
// Тесты не привязаны к конкретному решателю, например SolverTests для BruteForce и для Deductive:
    INSTANTIATE_TEST_SUITE_P(
        SolverBruteForce,
        SolverTests,
        testing::Values(SolverConfig{ Method::BruteForce, BFConfig{.prefilter = true, .execution = ExecPolicy::Sequential} }));

    INSTANTIATE_TEST_SUITE_P(
        SolverDeductive,
        SolverTests,
        testing::Values(SolverConfig{ Method::Deductive, {} }));
```

#### Трехуровневая система проверок:
1. **Синтетические пазлы** (2x2, 3x3 и т.д.)
   - Десятки тестов с заведомо известным количеством решений
   - Проверка базовой логики и граничных случаев
   - Формирование "доверия" к эталонному решателю (`BruteForce`)

2. **Реальные пазлы** (Загадка Эйнштейна и др.)
   - Сравнение с известными решениями
   - Проверка корректности работы с реальными данными

3. **Новые решатели**
   - Автоматическая проверка на всём массиве тестов
   - Сравнение результатов с эталонным `BruteForce` решателем

**Преимущества подхода:**
- Новый алгоритм сразу проверяется на множестве тестовых случаев
- Легкое добавление тестов для специфичных случаев
- Возможность сравнения производительности разных алгоритмов

В результате мы получаем прекрасную экосистему для работы над алгоритмами.

</details>


<details>
<summary><h4> Развернуть >> Многопоточная архитектура</h4></summary>

**Поток данных в параллельном решателе (BruteForce, режим Parallel):**
```text
Главный поток
    ↓
Создает ParallelExecutor с N потоками (по числу ядер CPU)
    ↓
Каждый рабочий поток:
    1. Запрашивает у SpaceSplitter новый чанк
    2. Проверяет каждый кандидат в чанке (hot loop!)
    3. Сохраняет подошедшие решения локально
    4. Обновляет AtomicProgressTracker
    ↓
Главный поток периодически опрашивает прогресс
    и вызывает progressCallback
    ↓
После завершения: результаты объединяются, исключения обрабатываются
```

</details>


<details>
<summary><h4> Развернуть >> Строгая типизация</h4></summary>

Проект активно использует возможности C++20 для предотвращения целого класса ошибок на этапе компиляции.

#### Сильные типы для индексов (TypedIndex)
Вместо "голых" индексов используются обертки, предотвращающие логические ошибки:
```cpp
// Категория индекса - часть типа
using AttributeTypeID = utils::TypedIndex<struct AttributeTypeID_tag>;
using AttributeValueID = utils::TypedIndex<struct AttributeValueID_tag>;
using PersonID = utils::TypedIndex<struct PersonID_tag>;
```
**Особенности реализации:**
- Полная совместимость со STL (`std::regular`, `std::totally_ordered`)
- Поддержка `<=>`
- `constexpr` семантика для вычислений на этапе компиляции
- Безопасное создание из итераторов через `fromDistance()`

#### Безопасные контейнеры (IndexedVector)
Обертка над `std::vector` с проверкой типов индексов:
```cpp
using AttributeAssignment = utils::IndexedVector<AttributeValueID, PersonID>; // Single combination values of single attribute

Assignment assignment;
assignment.emplace_back(PersonID{}); // Ok
// assignment.emplace_back(AttributeTypeID{}); // compiler error!

assignment[AttributeValueID{}] = PersonID{}; // Ok
// assignment[PersonID{}] = PersonID{}; // compiler error!
```

#### Расширение системы constraints

Архитектура поддерживает добавление новых типов условий через статическую типизацию. Основные шаги:
1. Добавить новый тип в `std::variant` в определении `PuzzleDefinition::constraints`
2. Следовать ошибкам компиляции для реализации необходимой логики обработки
3. Добавить соответствующие тесты

Система гарантирует корректность благодаря проверкам на этапе компиляции.

</details>

<div align="right"> 

[⬆ Наверх](#содержание)

</div>


### Сборка и разработка

Проект использует стандартный workflow на базе Modern CMake, что обеспечивает простую сборку и запуск тестов на любой поддерживаемой ОС. Вы можете клонировать репозиторий и экспериментировать с кодом, используя привычные инструменты.

<details>
<summary><h4> Развернуть >> Windows (Visual Studio 2022)</h4></summary>

1. **Установите**:
   - [Visual Studio 2022](https://visualstudio.microsoft.com/)
   - Компонент "Разработка классических приложений на C++"
   - Компонент "Инструменты CMake для Windows"
2. **Клонируйте репозиторий**:
```bash
git clone https://github.com/mz-pilot/epuzzle.git
cd epuzzle
```
3. **Откройте папку** в Visual Studio: `File → Open → Folder...` (Файл → Открыть → Папка...)
4. **Выберите конфигурацию** в верхней панели:
   - `windows-x64` (автоматически настроится через `CMakePresets.json`); дождитесь окончания конфигурирования (можно следить в Output window)
5. **Соберите проект**: `Build → Build All` (Сборка → Собрать все)
6. **Запустите тесты**: через меню `Test → Test Explorer` (Тест → Обозреватель тестов)

#### Кросс-платформенная разработка с WSL2 (Windows)
1. **Установите WSL2** и дистрибутив `Ubuntu`:
```powershell
wsl --install -d Ubuntu
```
2. **В Visual Studio** подключитесь к WSL:
   - Откройте проект
   - В выпадающем списке конфигураций выберите "Управление конфигурациями"
   - Добавьте конфигурацию для Linux
3. **Отлаживайте** код, выполняющийся в WSL, прямо из Visual Studio

<small>Примечание: для аналогичного использования `Debian` в Visual Studio c WSL2 может потребоваться после установки дистрибутива Debian вручную запустить его терминал и установить `rsync` командой `sudo apt install -y rsync` </small>

</details>


<details>
<summary><h4> Развернуть >> Linux</h4></summary>

1. **Установите зависимости**:
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install build-essential cmake ninja-build git clang clang-tidy clang-format clangd
```
2. **Клонируйте и соберите**:
```bash
git clone https://github.com/mz-pilot/epuzzle.git
cd epuzzle
cmake --preset=linux-release
cmake --build --preset=lin-release
ctest --preset=test-lin-release
```

</details>

<details>
<summary><h4> Развернуть >> Альтернативные среды разработки</h4></summary>

- **Qt Creator (Linux/Windows)** - проект открывается в Qt Creator как "CMake Project":
```
    1. Файл -> Открыть файл или проект...
    2. Выберите корневой `CMakeLists.txt` проекта.
    3. В меню Configure Project выберите нужный комплект (Kit), например Desktop GCC.
    4. Нажмите Configure.
    5. Используйте `Ctrl+R` для сборки и запуска или `F5` для отладки.
```
- **VS Code** — с расширениями CMake Tools и C++
- **CLion** — отлично работает с CMakePresets
- **Терминал + любимый редактор** — используйте пресеты CMake

</details>

#### Особенности сборки проекта
- **Релизные сборки включают отладочную информацию** - (`/DEBUG` в MSVC, `-g` в GCC/Clang) для упрощения диагностики
- **Запрет удаления stack pointer (регистр EBP/RBP)** - (`/Oy-` в MSVC, `-fno-omit-frame-pointer` в GCC/Clang) - также для упрощения диагностики
- **Автоматическое копирование примеров** — при сборке `epuzzle` папка `puzzle_examples` копируется рядом с исполняемым файлом
- **Статическая линковка рантайма** на Windows — `epuzzle.exe` не зависит от MSVC Redist


#### Запуск анализа кода линтером clang-tidy и санитайзерами
Осуществляется двумя способами:
- **Автоматически на GitHub** - в рамках `CI Checks (PR & Push)`.
- **Вручную локально** - на машине разработчика можно в Linux/WSL использовать пресет `linux-tidy`:
  - в IDE
  - либо в терминале:
  ```bash
  cd epuzzle
  cmake --preset=linux-tidy
  cmake --build --preset=lin-tidy  # clang-tidy
  ctest --preset=test-lin-tidy     # sanitizers
  ```

Примечание: для линтера используется строгая настройка `WarningsAsErrors`.

#### Запуск тестов

```bash
ctest --preset=test-lin-release  # Linux
ctest --preset=test-win-release  # Windows

# С подробным выводом
ctest -V --preset=test-lin-release
```

<details>
<summary><h4> Развернуть >> Известная проблема с адаптером Google Test в Visual Studio</h4></summary>

**Известная проблема с адаптером Google Test в Visual Studio:**
При использовании адаптера Google Test для Visual Studio тесты, помеченные как `GTEST_SKIP()`, могут ошибочно показываться как проваленные в Test Explorer. Это [известная проблема](https://developercommunity.visualstudio.com/t/googletest-skipped-tests-are-reported-as-failed-in/884416) Microsoft.

**Рекомендуемые обходные пути:**
* Использовать WSL2 — в Linux-среде проблема не проявляется
* Использовать CTest для запуска тестов данного проекта
* Создать свой Playlist для Google Test Adapter, не включая в него skipped тесты

**Почему используется `GTEST_SKIP()`:** Для временного отключения тестов для еще не реализованного функционала (например, `DeductiveSolver`).

</details>

#### Отладка
- **Visual Studio** — используйте встроенный отладчик, точки останова работают как для Windows, так и для WSL
- **Qt Creator**, **VS Code**, **CLion** - можете использовать отладчик привычным способом
- **Через командную строку** - перейдите в папку с собранным исполняемым файлом и запустите отладчик:
```bash
# Linux
cd out/build/linux-debug/bin/apps
gdb ./epuzzle

# Windows (если установлен Debugging Tools for Windows)
cd out/build/windows-x64/bin/apps
cdb epuzzle.exe
```

<div align="right"> 

[⬆ Наверх](#содержание)

</div>


### Roadmap

#### Версия 1.1 (в разработке)
- [ ] Реализация `DeductiveSolver` (логический вывод)
- [ ] Добавление новых типов constraints для расширения класса решаемых задач

#### Стратегия развития
- **BruteForce решатель** останется эталоном для проверки корректности новых алгоритмов
- **Deductive решатели** будут разрабатываться для эффективного решения больших пазлов (6x6 и более)
- **Тестирование новых алгоритмов** будет проводиться в том числе со сравнением результатов с BruteForce

---

Проект создан как демонстрация профессиональных навыков в современной C++ разработке. Открыт для изучения, использования и форков. Благодарю за внимание к проекту!

<div align="right"> 

[⬆ Наверх](#содержание)

</div>
