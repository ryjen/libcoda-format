# Format string grammar

`coda::format` uses zero-based positional specifiers enclosed in braces.

## Canonical form

The documented form is:

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

- `index` is a non-negative decimal integer.
- `width` is a signed decimal integer. A negative width requests left alignment.
- `type` is a single formatting character such as `f`, `e`, `x`, `X`, `o`, `O`, or `n`.
- `argument` is type-specific. Numeric precision arguments must be complete decimal integers; trailing characters are invalid.

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

Double opening or closing braces escape a literal brace, for example:

```text
{{0}}
```

renders as `{0}`.

## Invalid input

Parsing rejects malformed numeric fields rather than accepting a numeric prefix. Examples such as `{0junk}`, `{0,8junk}`, and `{0:f2junk}` are invalid.
