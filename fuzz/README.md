# Fuzzing libcoda-format

The first fuzz target is `coda_format_afl`. It reads a format string from standard input and exercises parsing, copy/move construction, argument binding, rendering, and reset behavior.

The harness is deterministic and side-effect free. Inputs are truncated to 4096 bytes and argument application is bounded to 64 specifiers per cycle. `std::invalid_argument` is treated as an expected result for malformed format strings; unexpected crashes, sanitizer findings, or other uncaught failures remain visible to AFL++.

## Build with AFL++

Install AFL++ so `afl-cc`, `afl-c++`, and `afl-fuzz` are available, then configure and build:

```bash
cmake --preset afl
cmake --build --preset afl --target coda_format_afl
```

The preset selects AFL++ LLVM instrumentation through `AFL_CC_COMPILER=LLVM`, disables the Bandit test tree, and enables the fuzz target in addition to the library.

## Run a campaign

```bash
mkdir -p build/afl/findings
afl-fuzz \
  -i fuzz/corpora/format \
  -o build/afl/findings \
  -x fuzz/dictionaries/format.dict \
  -- build/afl/fuzz/coda_format_afl
```

For a bounded local smoke campaign:

```bash
afl-fuzz -V 10 \
  -i fuzz/corpora/format \
  -o build/afl/smoke-findings \
  -x fuzz/dictionaries/format.dict \
  -- build/afl/fuzz/coda_format_afl
```

Long-running campaigns should write findings outside source-controlled directories and should not block the normal build/test workflow. AFL++ supports an explicit `-m` memory limit; tune it against the seed corpus before unattended campaigns rather than choosing a repository-wide arbitrary value.

## AddressSanitizer

The `afl-asan` preset enables AFL++ AddressSanitizer instrumentation through `AFL_USE_ASAN=1`:

```bash
cmake --preset afl-asan
cmake --build --preset afl-asan --target coda_format_afl
```

Run the resulting target with a separate output directory:

```bash
afl-fuzz \
  -i fuzz/corpora/format \
  -o build/afl-asan/findings \
  -x fuzz/dictionaries/format.dict \
  -- build/afl-asan/fuzz/coda_format_afl
```

## Reproduce a crash

A saved AFL++ input can be replayed directly through standard input:

```bash
build/afl/fuzz/coda_format_afl < path/to/crash-input
```

Use the ASAN build when reproducing memory errors:

```bash
build/afl-asan/fuzz/coda_format_afl < path/to/crash-input
```

## Corpus policy

Keep checked-in seeds small and grammar-focused. Add a reproducer to the corpus only after the underlying behavior is understood and the input provides durable regression value. Do not commit AFL++ queue/findings directories.

## Next optimization step

This first harness uses the conventional AFL++ forkserver path because it is simple and reviewable. If campaign throughput becomes the limiting factor, evaluate an `LLVMFuzzerTestOneInput`/persistent-mode harness as a separate change rather than mixing lifecycle optimization into the initial correctness slice.
