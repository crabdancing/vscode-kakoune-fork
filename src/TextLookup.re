module LookupError = {
  type t =
    | Underflow
    | Overflow;
};

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

let characterTypeBefore = (~startIndex, text) =>
  if (startIndex > 0 && startIndex <= (text |> String.length)) {
    text |> characterTypeAfter(~startIndex=startIndex - 1);
  } else if (startIndex <= 0) {
    Error(LookupError.Underflow);
  } else {
    Error(LookupError.Overflow);
  };

let rec findNextNotOfType = (~characterType, ~startIndex, text) =>
  text
  |> characterTypeAfter(~startIndex)
  |> Result.andThen(~f=t =>
       t == characterType
         ? text
           |> findNextNotOfType(~characterType=t, ~startIndex=startIndex + 1)
         : Ok(startIndex)
     );

let findNextNotOfWhitespace = (~startIndex, text) =>
  text
  |> findNextNotOfType(~characterType=CharacterType.Whitespace, ~startIndex);

let rec findPreviousNotOfType = (~characterType, ~startIndex, text) =>
  text
  |> characterTypeBefore(~startIndex)
  |> Result.andThen(~f=t =>
       t == characterType
         ? text
           |> findPreviousNotOfType(
                ~characterType=t,
                ~startIndex=startIndex - 1,
              )
         : Ok(startIndex)
     );

let findPreviousNotOfWhitespace = (~startIndex, text) =>
  text
  |> findPreviousNotOfType(
       ~characterType=CharacterType.Whitespace,
       ~startIndex,
     );

let findWordEnd = (~startIndex, text) =>
  text
  |> characterTypeAfter(~startIndex)
  |> Result.andThen(~f=characterType =>
       text |> findNextNotOfType(~characterType, ~startIndex)
     )
  |> (
    e =>
      switch (e) {
      | Error(LookupError.Overflow) => text |> String.length |> Result.ok
      | Error(Underflow) => Error(LookupError.Underflow)
      | Ok(i) => Ok(i)
      }
  );

let findWordStart = (~startIndex, text) =>
  text
  |> characterTypeBefore(~startIndex)
  |> Result.andThen(~f=characterType =>
       text
       |> findPreviousNotOfType(~characterType, ~startIndex=startIndex - 1)
     )
  |> Result.map(~f=i => i)
  |> (
    e =>
      switch (e) {
      | Error(LookupError.Underflow) => Ok(0)
      | Ok(i) => Ok(i)
      | Error(Overflow) => Error(LookupError.Overflow)
      }
  );

let findNextWordStart = (~startIndex, text) =>
  text
  |> characterTypeAfter(~startIndex)
  |> Result.andThen(~f=characterType =>
       text |> findNextNotOfType(~characterType, ~startIndex)
     )
  |> Result.andThen(~f=startIndex =>
       text |> findNextNotOfWhitespace(~startIndex)
     );

let findNextWordEnd = (~startIndex, text) =>
  text
  |> characterTypeAfter(~startIndex)
  |> Result.andThen(~f=characterType =>
       text |> findNextNotOfType(~characterType, ~startIndex)
     )
  |> Result.andThen(~f=startIndex =>
       text |> findNextNotOfWhitespace(~startIndex)
     )
  |> Result.andThen(~f=startIndex => text |> findWordEnd(~startIndex));

let findPreviousWordStart = (~startIndex, text) =>
  text
  |> characterTypeBefore(~startIndex)
  |> Result.andThen(~f=characterType =>
       text |> findPreviousNotOfType(~characterType, ~startIndex)
     )
  |> Result.andThen(~f=startIndex =>
       text |> findPreviousNotOfWhitespace(~startIndex)
     )
  |> Result.andThen(~f=startIndex => text |> findWordStart(~startIndex));

let findPreviousWordEnd = (~startIndex, text) =>
  text
  |> characterTypeBefore(~startIndex)
  |> Result.andThen(~f=characterType =>
       text |> findPreviousNotOfType(~characterType, ~startIndex)
     )
  |> Result.andThen(~f=startIndex =>
       text |> findPreviousNotOfWhitespace(~startIndex)
     )
  |> (
    e =>
      switch (e) {
      | Error(LookupError.Underflow) => Ok(0)
      | Ok(i) => Ok(i)
      | Error(Overflow) => Error(LookupError.Overflow)
      }
  );
