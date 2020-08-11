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
 * Finds the index of the end of the word currently under the cursor location
 * as specified by `startIndex`.
 *
 * Returns Ok(int) when found, Error(LookupError.Underflow) otherwise.
 */
let findWordEnd = (~startIndex, text) => {
  // The index of the first next character that has a different type from the one
  // currently under the cursor is the end of the word.
  text
  |> characterTypeAfter(~startIndex)
  |> Result.andThen(~f=characterType =>
       text |> findNextNotOfType(~characterType, ~startIndex)
     )
  |> (
    // If we Overflowed during the search we know that the end of the word is
    // the same as the end of the string, as such we can't actually Overflow.
    endIndex =>
      switch (endIndex) {
      | Error(LookupError.Overflow) => text |> String.length |> Result.ok
      | Error(Underflow) => Error(LookupError.Underflow)
      | Ok(i) => Ok(i)
      }
  );
};

/**
 * Finds the index of the start of the word currently under the cursor location
 * as specified by `startIndex`.
 *
 * Returns Ok(int) when found, Error(LookupError.Underflow) otherwise.
 */
let findWordStart = (~startIndex, text) => {
  // 1. Find the first previous character that doesn't share the currenct character's type.
  // 2. If Underflow and started searching from 0 => Ok(0)
  text
  |> characterTypeBefore(~startIndex)
  |> Result.andThen(~f=characterType =>
       characterType != CharacterType.Whitespace
         ? switch (text |> findPreviousNotOfType(~characterType, ~startIndex)) {
           | Error(LookupError.Underflow) =>
             startIndex > 0 ? Ok(0) : Error(LookupError.Underflow)
           | Error(Overflow) => Error(Overflow)
           | Ok(i) => Ok(i)
           }
         : {
           switch (text |> findPreviousNotOfWhitespace(~startIndex)) {
           | Error(LookupError.Underflow) =>
             startIndex > 0 ? Ok(0) : Error(LookupError.Underflow)
           | Error(Overflow) => Error(Overflow)
           | Ok(i) =>
             text
             |> characterTypeBefore(~startIndex=i)
             |> Result.andThen(~f=characterType =>
                  switch (
                    text
                    |> findPreviousNotOfType(~characterType, ~startIndex=i)
                  ) {
                  | Error(LookupError.Underflow) => Ok(0)
                  | Error(Overflow) => Error(LookupError.Overflow)
                  | Ok(i) => Ok(i)
                  }
                )
           };
         }
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

/**
 * Finds the index of the end of the next word, counted from the word currently
 * under the cursor location specified by `startIndex`.
 *
 * Returns Ok(int) when found, Error(LookupError.t) otherwise.
 */
let findNextWordEnd = (~startIndex, text) => {
  // 1. Find the next character that doesn't share the current character's type.
  // 2. Find the next character that isn't a whitespace.
  // 3. Find the end of the word starting from the index retrieved in 2.
  text
  |> characterTypeAfter(~startIndex)
  |> Result.andThen(~f=characterType =>
       text |> findNextNotOfType(~characterType, ~startIndex)
     )
  |> Result.andThen(~f=startIndex =>
       text |> findNextNotOfWhitespace(~startIndex)
     )
  |> Result.andThen(~f=startIndex => text |> findWordEnd(~startIndex));
};

/**
 * Finds the index of the end of the previous word, counted from the word currently
 * under the cursor location specified by `startIndex`.
 *
 * Returns Ok(int) when found, Error(LookupError.t) otherwise.
 */
let findPreviousWordEnd = (~startIndex, text) => {
  // 1. Find the start of the word currently under the cursor.
  // 2. Find the previous character that isn't a whitespace.
  text
  |> findWordStart(~startIndex)
  |> Result.andThen(~f=wordStartIndex => {
       // If we were already at the start of the word, and that word was at the
       // beginning of the line, we're about to underflow, otherwise we can safely
       // find a previous character that isn't a whitespace.
       wordStartIndex == startIndex && wordStartIndex == 0
         ? Error(LookupError.Underflow)
         : text
           |> findPreviousNotOfWhitespace(~startIndex=wordStartIndex)
           |> Result.unwrap(~default=0)
           |> Result.ok
     });
};
