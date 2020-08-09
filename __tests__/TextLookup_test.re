open Jest;

describe("Text lookup functions", () => {
  open Expect;

  describe("characterTypeAt", () => {
    let testInput = [
      (("hello world!", 0), Ok(CharacterType.Character)),
      (("hello world!", 5), Ok(CharacterType.Whitespace)),
      (("hello world!", 11), Ok(CharacterType.Punctuation)),
      (("hello world!", (-1)), Error(TextLookup.LookupError.InvalidIndex)),
      (("hello world!", 12), Error(TextLookup.LookupError.InvalidIndex)),
    ];

    testAll("characterTypeAt", testInput, (((text, index), expected)) => {
      expect(text->TextLookup.characterTypeAt(index)) |> toEqual(expected)
    });
  });

  describe("findNextNotOfType", () => {
    let testInput = [
      (("hello world!", 0, CharacterType.Character), Ok(5)),
      (("hello world!", 5, CharacterType.Whitespace), Ok(6)),
      (
        ("hello world!", 11, CharacterType.Punctuation),
        Error(TextLookup.LookupError.NotFound),
      ),
    ];

    testAll(
      "findNextNotOfType",
      testInput,
      (((text, index, currentType), expected)) => {
      expect(text->TextLookup.findNextNotOfType(currentType, index))
      |> toEqual(expected)
    });
  });

  describe("findPreviousNotOfType", () => {
    let testInput = [
      (
        ("hello world!", 0, CharacterType.Character),
        Error(TextLookup.LookupError.NotFound),
      ),
      (("hello world!", 5, CharacterType.Whitespace), Ok(4)),
      (("hello world!", 11, CharacterType.Punctuation), Ok(10)),
    ];

    testAll(
      "findPreviousNotOfType",
      testInput,
      (((text, index, currentType), expected)) => {
      expect(text->TextLookup.findPreviousNotOfType(currentType, index))
      |> toEqual(expected)
    });
  });

  describe("findNextWordStart", () => {
    let testInput = [
      (("hello world!", 0), Ok(6)),
      (("hello world!", 7), Ok(11)),
      (("hello world!", 11), Error(TextLookup.LookupError.NotFound)),
    ];

    testAll("findNextWordStart", testInput, (((text, index), expected)) => {
      expect(text->TextLookup.findNextWordStart(index)) |> toEqual(expected)
    });
  });

  describe("findNextWordEnd", () => {
    let testInput = [
      (("hello world!", 0), Ok(10)),
      (("hello world!", 7), Ok(11)),
      (("hello world!", 11), Error(TextLookup.LookupError.NotFound)),
    ];

    testAll("findNextWordEnd", testInput, (((text, index), expected)) => {
      expect(text->TextLookup.findNextWordEnd(index)) |> toEqual(expected)
    });
  });

  describe("findPreviousWordStart", () => {
    let testInput = [
      (("hello world!", 11), Ok(6)),
      (("hello world!", 9), Ok(0)),
      (("hello world!", 2), Error(TextLookup.LookupError.NotFound)),
    ];

    testAll("findPreviousWordStart", testInput, (((text, index), expected)) => {
      expect(text->TextLookup.findPreviousWordStart(index))
      |> toEqual(expected)
    });
  });

  describe("findPreviousWordEnd", () => {
    let testInput = [
      (("hello world!", 11), Ok(10)),
      (("hello world!", 9), Ok(4)),
      (("hello world!", 2), Error(TextLookup.LookupError.NotFound)),
    ];

    testAll("findPreviousWordEnd", testInput, (((text, index), expected)) => {
      expect(text->TextLookup.findPreviousWordEnd(index))
      |> toEqual(expected)
    });
  });

  describe("findWordEnd", () => {
    let testInput = [
      (("hello world!", 0), 4),
      (("hello world!", 7), 10),
      (("hello", 2), 4),
    ];

    testAll("findWordEnd", testInput, (((text, index), expected)) => {
      expect(text->TextLookup.findWordEnd(index)) |> toBe(expected)
    });
  });

  describe("findWordStart", () => {
    let testInput = [
      (("hello world!", 0), 0),
      (("hello world!", 2), 0),
      (("hello world!", 8), 6),
      (("hello world!", 6), 6),
      (("hello world!", 11), 11),
      (("hello", 2), 0),
    ];

    testAll("findWordStart", testInput, (((text, index), expected)) => {
      expect(text->TextLookup.findWordStart(index)) |> toBe(expected)
    });
  });
});
