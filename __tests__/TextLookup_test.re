open Jest;

describe("Text lookup functions", () => {
  open Expect;

  describe("characterTypeAfter", () => {
    let testInput = [
      (("hello world!", 0), Ok(CharacterType.Character)),
      (("hello world!", 5), Ok(CharacterType.Whitespace)),
      (("hello world!", 11), Ok(CharacterType.Punctuation)),
      (("hello world!", 12), Ok(CharacterType.Whitespace)),
      (("hello world!", (-1)), Error(TextLookup.LookupError.Underflow)),
      (("hello world!", 13), Error(Overflow)),
    ];

    testAll(
      "characterTypeAfter", testInput, (((text, startIndex), expected)) =>
      text
      |> TextLookup.characterTypeAfter(~startIndex)
      |> expect
      |> toEqual(expected)
    );
  });

  describe("findNextNotOfType", () => {
    let testInput = [
      (("hello world!", 0, CharacterType.Character), Ok(5)),
      (("hello world!", 5, CharacterType.Whitespace), Ok(6)),
      (("hello world!", 11, CharacterType.Punctuation), Ok(12)),
      (
        ("hello world!", 12, CharacterType.Whitespace),
        Error(TextLookup.LookupError.Overflow),
      ),
      (("hello world!", (-1), CharacterType.Punctuation), Error(Underflow)),
    ];

    testAll(
      "findNextNotOfType",
      testInput,
      (((text, startIndex, characterType), expected)) =>
      text
      |> TextLookup.findNextNotOfType(~characterType, ~startIndex)
      |> expect
      |> toEqual(expected)
    );
  });

  describe("findPreviousNotOfType", () => {
    let testInput = [
      (("hello world!", 5, CharacterType.Whitespace), Ok(5)),
      (("hello world!", 6, CharacterType.Whitespace), Ok(5)),
      (("hello world!", 10, CharacterType.Character), Ok(6)),
      (("hello world!", 11, CharacterType.Punctuation), Ok(11)),
      (("hello world!", 12, CharacterType.Punctuation), Ok(11)),
      (("hello world!", 12, CharacterType.Punctuation), Ok(11)),
      (("hello world!", 12, CharacterType.Whitespace), Ok(12)),
      (
        ("hello world!", 0, CharacterType.Character),
        Error(TextLookup.LookupError.Underflow),
      ),
      (("hello world!", 13, CharacterType.Whitespace), Error(Overflow)),
    ];

    testAll(
      "findPreviousNotOfType",
      testInput,
      (((text, startIndex, characterType), expected)) =>
      text
      |> TextLookup.findPreviousNotOfType(~characterType, ~startIndex)
      |> expect
      |> toEqual(expected)
    );
  });

  describe("findNextWordStart", () => {
    let testInput = [
      (("hello world!", 0), Ok(6)),
      (("hello world!", 7), Ok(11)),
      (("#include <iostream>", 0), Ok(1)),
      (("#include <iostream>", 17), Ok(18)),
      (("#include <iostream>", 18), Ok(19)),
      (("\treturn 0;", 8), Ok(9)),
      (("    return 0;", 11), Ok(12)),
      (("hello world!", 11), Ok(12)),
      (("hello world!", 12), Error(TextLookup.LookupError.Overflow)),
      (("hello world!", (-1)), Error(TextLookup.LookupError.Underflow)),
    ];

    testAll(
      "findNextWordStart", testInput, (((text, startIndex), expected)) =>
      text
      |> TextLookup.findNextWordStart(~startIndex)
      |> expect
      |> toEqual(expected)
    );
  });

  describe("findWordStart", () => {
    let testInput = [
      (("hello world!", 2), Ok(0)),
      (("hello world!", 6), Ok(0)),
      (("hello world!", 8), Ok(6)),
      (("hello world!", 11), Ok(6)),
      (("hello world!", 12), Ok(11)),
      (("hello world!", 0), Error(TextLookup.LookupError.Underflow)),
      (("hello world!", 13), Error(Overflow)),
      (("hello", 2), Ok(0)),
      (("hello", 5), Ok(0)),
      (("hello", 0), Error(Underflow)),
      (("hello", 6), Error(Overflow)),
      (("#include <iostream>", 1), Ok(0)),
      (("#include <iostream>", 8), Ok(1)),
      (("#include <iostream>", 9), Ok(1)),
      (("#include <iostream>", 10), Ok(9)),
      (("#include <iostream>", 17), Ok(10)),
      (("#include <iostream>", 18), Ok(10)),
      (("#include <iostream>", 19), Ok(18)),
      (("#include <iostream>", 20), Error(Overflow)),
      (("    return 0;", 13), Ok(12)),
      (("    return 0;", 12), Ok(11)),
      (("    return 0;", 11), Ok(4)),
      (("    return 0;", 4), Ok(0)),
      (("    return 0;", 2), Ok(0)),
      (("    return 0;", 0), Error(Underflow)),
      (("    return 0;", 14), Error(Overflow)),
    ];

    testAll("findWordStart", testInput, (((text, startIndex), expected)) =>
      text
      |> TextLookup.findWordStart(~startIndex)
      |> expect
      |> toEqual(expected)
    );
  });
});
