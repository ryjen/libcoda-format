# Format string grammar

`coda::format` uses zero-based positional specifiers enclosed in braces.

## Canonical form

```text
{index[,width][:type[argument]]}
```

Examples:

```text
{0}
{0,8}
{0,-8}
{0:f2}
{0,-8:f2}
```

- `index` is a non-negative decimal integer containing digits only.
- `width` is a decimal integer containing digits, optionally prefixed by `-` for left alignment. The current implementation stores width as `int8_t`, so accepted widths are `-128` through `127`.
- `type` is a single formatting character such as `f`, `e`, `x`, `X`, `o`, `O`, or `n`.
- `argument` is type-specific. Numeric precision arguments are non-negative decimal integers containing digits only.

Whitespace, a leading `+`, and trailing characters are not part of numeric tokens.

Specifier indexes may appear out of textual order, but the set of indexes must be contiguous starting at zero.

## Compatibility form

Older tests and callers may use width after the type argument:

```text
{0:f2,12}
```

This form remains accepted for compatibility. New code should use the canonical form:

```text
{0,12:f2}
```

A specifier must not provide width in both positions.

## Escaping

Double opening or closing braces escape a literal brace. For example, `{{0}}` renders as `{0}`.

## Invalid input

Parsing rejects malformed numeric fields rather than accepting a numeric prefix. Examples such as `{0junk}`, `{+0}`, `{0, 8}`, and `{0:f2junk}` are invalid.
