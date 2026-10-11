# Historical libcoda component: libcoda-format

**Disposition: ARCHIVE.** The active project is [Seamwork](https://github.com/ryjen/seamwork), a C++23 interface and architecture refinement laboratory rather than an aggregate utility distribution.

## Why
Formatter parsing, fuzzing, sanitizer, and deterministic test evidence are retained, but the formatter is not a Seamwork component.

Prefer standard C++23 formatting; study any demonstrated gap instead of promoting the old formatter.

## Reproducible provenance
- Repository: https://github.com/ryjen/libcoda-format
- Former aggregate pin: [`ed9e69323d8364b5f71bd6aae097637db608f2df`](https://github.com/ryjen/libcoda-format/commit/ed9e69323d8364b5f71bd6aae097637db608f2df)
- Primary migration: https://github.com/ryjen/seamwork/issues/34
- Source/provenance record: https://github.com/ryjen/seamwork/blob/development/docs/migration/seamwork-rebrand.md

Historical source, tests, fuzz corpora, and sanitizer harnesses remain accessible in this repo. They are not currently security maintained or supported as a release. Follow the original LICENSE and preserve attribution before reusing code.

## Maintenance
CI is now manually dispatchable for historical regression reproductions rather than automatically running on each push or pull request. Any new formatting work should be justified through a focused Seamwork C++23 study; do not mechanically rename the legacy `coda::` API.
