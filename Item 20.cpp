/**
 * Prefer pass-by-reference-to-const to pass-by-value.
*/

/**
 * Prefer pass-by-reference-to-const over pass-by-value.
 * It’s typically more efficient and it avoids the slicing problem.
 *
 * The rule doesn’t apply to built-in types and STL iterator and function object types.
 * For them, pass-by-value is usually appropriate.
*/