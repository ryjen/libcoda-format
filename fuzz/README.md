# Fuzzing libcoda-format

`coda_format_afl` reads a format string from standard input and exercises parsing, copy/move construction, argument binding, rendering, and reset behavior.

The harness is deterministic and side-effect free. Inputs are truncated to 4096 bytes and argument application is bounded to 64 specifiers per exercise cycle. `std::invalid_argument` is an expected result for malformed format strings; unexpected crashes, sanitizer findings, or other uncaught failures remain visible.

## Build with a normal compiler

The harness can be built without AFL++ for deterministic smoke testing:

```bash
cmake -S . -B build/fuzz-smoke \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCODA_BUILD_TESTS=OFF \
  -DCODA_BUILD_FUZZERS=ON
cmake --build build/fuzz-smoke --target coda_format_afl
```

Replay every checked-in seed:

```bash
for input in fuzz/corpora/format/*; do
  build/fuzz-smoke/fuzz/coda_format_afl < "$input"
done
```

## Build with AFL++

Install AFL++ so `afl-c++` and `afl-fuzz` are available, then use the LLVM-mode preset:

```bash
cmake --preset afl
cmake --build --preset afl --target coda_format_afl
```

## Run a campaign

```bash
mkdir -p build/afl/findings
afl-fuzz \
  -i fuzz/corpora/format \
  -o build/afl/findings \
  -x fuzz/dictionaries/format.dict \
  -- build/afl/fuzz/coda_format_afl
```

For a bounded smoke campaign:

```bash
afl-fuzz -V 10 \
  -i fuzz/corpora/format \
  -o build/afl/smoke-findings \
  -x fuzz/dictionaries/format.dict \
  -- build/afl/fuzz/coda_format_afl
```

Long-running campaigns should use a fresh findings directory and should not block the normal build/test workflow.

## AddressSanitizer

The `afl-asan` preset enables AFL++ AddressSanitizer instrumentation using `AFL_USE_ASAN=1`:

```bash
cmake --preset afl-asan
cmake --build --preset afl-asan --target coda_format_afl
```

Run it with a separate findings directory:

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

Keep checked-in seeds small and grammar-focused. Add a reproducer to the corpus only after the behavior is understood and the input has durable regression value. Do not commit AFL++ queue or findings directories.

The first harness uses AFL++'s conventional forkserver execution model. Persistent mode is a later throughput optimization and should be evaluated separately from this correctness slice.
