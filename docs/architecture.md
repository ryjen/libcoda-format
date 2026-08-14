# libcoda-format architecture

## Purpose

`libcoda-format` is a small C++17 formatting library. The modernization goal is to make its boundaries explicit without turning a compact value-oriented utility into an abstraction framework.

The public facade remains `coda::format`. Internal responsibilities are documented separately so parser, rendering, and state behavior can be tested and extracted incrementally when doing so removes coupling or enables a concrete test/security seam.

## Public API boundary

The canonical public include is:

```cpp
#include <coda/format/format.h>
```

`#include <format.h>` remains supported as a compatibility path during modernization. New code should use the canonical namespaced include.

The existing `coda::format` class name and core behavior are retained. Compatibility-affecting API changes should be deliberate, documented, and covered by regression tests rather than folded into internal refactoring.

## Current responsibilities

| Responsibility | Current location | Contract |
| --- | --- | --- |
| Public facade | `coda::format` | Owns the source format string, specifier state, argument binding, reset, rendering entry points, and string/stream conversion. |
| Parser | `format::initialize`, `format::add_specifier`, file-local numeric parsers | Parses the documented grammar into internal specifiers and rejects malformed input with `std::invalid_argument`. |
| Specifier model | private `specifier` value in `format` | Holds source positions, index, width, type-specific argument, and rendered replacement. |
| Argument binding | `format::args` templates | Binds values sequentially to logical specifier indexes and records rendered replacements. |
| Rendering | `print`, `unescape`, `begin_manip`, `end_manip` | Emits literals/replacements and applies scoped stream formatting rules. |
| State/reset | constructors, assignments, `reset`, `specifiers` | Preserves the current binding cursor across copy/move behavior and rebuilds parser state on reset. |
| Error handling | parser and binding operations | Malformed format input and invalid binding operations use `std::invalid_argument`; fuzzing treats those as expected rejected-input outcomes. |

## Dependency direction

The intended dependency direction is deliberately simple:

```text
consumer
   |
   v
coda::format facade
   |
   +--> parser/specifier model
   |
   +--> argument binding/state
   |
   +--> renderer --> C++ standard streams
```

Internal parsing and rendering must not depend on test frameworks, AFL++, filesystem state, networking, or external services. The fuzz target depends on the public library; the library never depends on fuzz infrastructure.

## Extraction seams

The current class is small enough that extraction is not automatically an improvement. Prefer extraction only when it creates a measurable boundary:

1. **Parser seam** — a pure parser could return a collection of immutable specifier descriptors. This becomes useful when parser tests/fuzzing need to run independently of rendering state.
2. **Renderer seam** — rendering can become a pure operation over source text plus bound specifiers if stream manipulation grows or alternate output sinks become necessary.
3. **Binding/state seam** — keep mutable argument cursor/state in the facade unless a second binding strategy appears. Avoid introducing an interface solely for architectural symmetry.
4. **Error seam** — preserve `std::invalid_argument` compatibility until a richer public error model has a concrete consumer requirement.

This applies Single Responsibility and Dependency Inversion pragmatically: isolate volatile or externally coupled behavior, but do not add interfaces where there is only one stable implementation and no useful substitution boundary.

## Security and robustness invariants

- Treat format strings as untrusted input.
- Numeric grammar tokens must be fully consumed; numeric prefixes followed by junk are rejected.
- Width remains bounded by the current `std::int8_t` storage contract.
- Parser rejection is deterministic and side-effect free.
- Fuzz harnesses bound input size and argument application so pathological inputs cannot create unbounded work in the harness itself.
- Public headers must not inject namespaces or rely on transitive standard-library includes.

## Replication pattern

The modernization pattern to carry into `libcoda-db` and `libcoda-net` is:

1. establish a target-owned CMake boundary;
2. make the public include/API surface explicit and preserve compatibility intentionally;
3. document the component's pure/domain responsibilities separately from adapters or I/O;
4. characterize existing behavior with deterministic tests before tightening semantics;
5. add sanitizers and fuzzing at pure input-processing seams first;
6. only then extract interfaces or adapters where tests, alternate implementations, or platform/service boundaries justify them.

For format-specific syntax, see `format-grammar.md`. For the deterministic AFL++ harness, see `../fuzz/README.md`.
