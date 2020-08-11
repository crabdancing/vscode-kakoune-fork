module LookupError = {
  type t =
    | Underflow
    | Overflow;
};

/**
 * Checks the character type of the character immediately following the cursor's location
 * as specified by `startIndex`.
 *
 * Returns Ok(CharacterType) for valid indices, Error(LookupError.t) otherwise.
 *
 * Note that the type of the character that immediately follows the last character in the
 * provided string is /assumed/ to be `Whitespace`.
 */
let characterTypeAfter = (~startIndex, text) =>
  if (startIndex >= 0 && startIndex < text->String.length) {
    text
    |> String.slice(~from=startIndex, ~to_=startIndex + 1)
    |> CharacterType.characterType
    |> Result.ok;
  } else if (startIndex < 0) {
    Error(LookupError.Underflow);
  } else if (startIndex == (text |> String.length)) {
    Ok(CharacterType.Whitespace);
  } else {
    Error(Overflow);
  };

/**
 * Checks the character type of the character immediately before the cursor's location
 * as specified by `startIndex`.
 *
 * Returns Ok(CharacterType) for valid indices, Error(LookupError.t) otherwise.
 */
let characterTypeBefore = (~startIndex, text) =>
  if (startIndex > 0 && startIndex <= (text |> String.length)) {
    text |> characterTypeAfter(~startIndex=startIndex - 1);
  } else if (startIndex <= 0) {
    Error(LookupError.Underflow);
  } else {
    Error(LookupError.Overflow);
  };

/**
 * Finds the index of the first next character that does not have the type specified
 * by `characterType`. The search is started from `startIndex`.
 *
 * Returns Ok(int) for valid indices, Error(LookupError.t) otherwise.
 */
let rec findNextNotOfType = (~characterType, ~startIndex, text) =>
  text
  |> characterTypeAfter(~startIndex)
  |> Result.andThen(~f=t =>
       t != characterType
         ? Ok(startIndex)
         : text
           |> findNextNotOfType(~characterType=t, ~startIndex=startIndex + 1)
     );

/**
 * Convenience function.
 *
 * Finds the index of the first next character that is not a `Whitespace`. The search
 * is started from `startIndex`.
 *
 * Returns Ok(int) for valid indices, Error(LookupError.t) otherwise.
 */
let findNextNotOfWhitespace = (~startIndex, text) =>
  text
  |> findNextNotOfType(~characterType=CharacterType.Whitespace, ~startIndex);

/**
 * Finds the index of the first previous character that does not have the type specified
 * by `characterType`. The search is started from `startIndex`.
 *
 * Returns Ok(int) for valid indices, Error(LookupError.t) otherwise.
 */
let rec findPreviousNotOfType = (~characterType, ~startIndex, text) =>
  text
  |> characterTypeBefore(~startIndex)
  |> Result.andThen(~f=t =>
       t != characterType
         ? Ok(startIndex)
         : text
           |> findPreviousNotOfType(
                ~characterType=t,
                ~startIndex=startIndex - 1,
              )
     );

/**
 * Convenience function.
 *
 * Finds the index of the first previous character that is not a `Whitespace`. The
 * search is started from `startIndex`.
 *
 * Returns Ok(int) for valid indices, Error(LookupError.t) otherwise.
 */
let findPreviousNotOfWhitespace = (~startIndex, text) =>
  text
  |> findPreviousNotOfType(
       ~characterType=CharacterType.Whitespace,
       ~startIndex,
     );

/**
 * Finds the index of the start of the word currently under the cursor location
 * as specified by `startIndex`.
 *
 * The current word is defined as the word /ending/ at the current cursor location,
 * i.e. the word is to the left of (before) the current cursor location.
 *
 * Returns Ok(int) when found, Error(LookupError.t) otherwise.
 */
let findWordStart = (~startIndex, text) => {
  /**
   * When looking for the start of a word that is not made up of whitespaces and
   * we underflow there are 2 possibilities:
   *   1. We started searching at the start of the string, in which case we underflow.
   *   2. We've /reached/ the start of the string, in which case we return 0 (start of the string).
   *
   * If we overflow that's an error.
   *
   * If we find a valid index that's the start of the word.
   *
   * Returns Ok(int) or Error(LookupError.t)
   */
  let findStartOfNonWhitespaceWord = (~startIndex) =>
    Result.andThen(~f=characterType => {
      switch (text |> findPreviousNotOfType(~characterType, ~startIndex)) {
      | Error(LookupError.Underflow) =>
        startIndex > 0 ? Ok(0) : Error(LookupError.Underflow)
      | Error(Overflow) => Error(Overflow)
      | Ok(i) => Ok(i)
      }
    });

  /**
   * When looking for the start of a word that is made up of whitespaces and
   * we underflow there are 2 possibilities:
   *   1. We started searching at the start of the string, in which case we underflow.
   *   2. We've /reached/ the start of the string, in which case we return 0 (start of the string).
   *
   * If we overflow that's an error.
   *
   * If we reach a valid non-whitespace character we then find the start of that word and return
   * the result from that search.
   *
   * Returns Ok(int) or Error(LookupError.t)
   */
  let findStartOfWhitespaceWord = (~startIndex) =>
    switch (text |> findPreviousNotOfWhitespace(~startIndex)) {
    | Error(LookupError.Underflow) =>
      startIndex > 0 ? Ok(0) : Error(LookupError.Underflow)
    | Error(Overflow) => Error(Overflow)
    | Ok(i) =>
      text
      |> characterTypeBefore(~startIndex=i)
      |> findStartOfNonWhitespaceWord(~startIndex=i)
    };

  text
  |> characterTypeBefore(~startIndex)
  |> Result.andThen(~f=characterType =>
       characterType != CharacterType.Whitespace
         ? characterType
           |> Result.ok
           |> findStartOfNonWhitespaceWord(~startIndex)
         : findStartOfWhitespaceWord(~startIndex)
     );
};

/**
 * Finds the index of the start of the next word, counted from the word currently
 * under the cursor location specified by `startIndex`.
 *
 * Returns Ok(int) when found, Error(LookupError.t) otherwise.
 */
let findNextWordStart = (~startIndex, text) => {
  // 1. Find the next character that doesn't share the current character's type.
  // 2. Find the next character that isn't a whitespace.
  text
  |> characterTypeAfter(~startIndex)
  |> Result.andThen(~f=characterType =>
       text |> findNextNotOfType(~characterType, ~startIndex)
     )
  |> Result.map(~f=startIndex => {
       // If we overflow when trying to skip over whitespace, we're at the end
       // of the line, and can just return the length of the string.
       // However, this is /not/ the same as overflowing while finding the first
       // character of a different type, which is why this check is here.
       text
       |> findNextNotOfWhitespace(~startIndex)
       |> Result.unwrap(~default=text |> String.length)
     });
};
