type t =
  | Punctuation
  | Character
  | Whitespace;

/**
 * Returns the character type of the give 1 character string.
 *
 * Note that the input `character` /must/ be a string of length 1.
 *
 * Character is defined as anything that matches the `\w` regex.
 * Whitespace is defined as anything that matches the `\s` regex.
 * Punctuation is defined as anything that doesn't match the other 2.
 */
let characterType = character =>
  if ("\\w"->Js.Re.fromString->Js.Re.test_(character)) {
    Character;
  } else if ("\\s"->Js.Re.fromString->Js.Re.test_(character)) {
    Whitespace;
  } else {
    Punctuation;
  };
