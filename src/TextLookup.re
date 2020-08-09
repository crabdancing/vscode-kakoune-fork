module LookupError = {
  type t =
    | NotFound
    | InvalidIndex;
};

let characterTypeAt = (text, index) =>
  index >= 0 && index < text->String.length
    ? text
      |> String.slice(~from=index, ~to_=index + 1)
      |> CharacterType.characterType
      |> Result.ok
    : Error(LookupError.InvalidIndex);

let rec findNextNotOfType = (text, characterType, index) =>
  switch (text->characterTypeAt(index)) {
  | Error(LookupError.NotFound)
  | Error(LookupError.InvalidIndex) => Error(LookupError.NotFound)
  | Ok(t) =>
    t == characterType
      ? text->findNextNotOfType(characterType, index + 1) : Ok(index)
  };

let findNextNotOfWhitespace = (text, index) =>
  text->findNextNotOfType(CharacterType.Whitespace, index);

let rec findPreviousNotOfType = (text, characterType, index) =>
  switch (text->characterTypeAt(index)) {
  | Error(LookupError.NotFound)
  | Error(LookupError.InvalidIndex) => Error(LookupError.NotFound)
  | Ok(t) =>
    t == characterType
      ? text->findPreviousNotOfType(characterType, index - 1) : Ok(index)
  };

let findPreviousNotOfWhitespace = (text, index) =>
  text->findPreviousNotOfType(CharacterType.Whitespace, index);

let findWordEnd = (text, index) =>
  text->characterTypeAt(index)
  |> Result.andThen(~f=t => text->findNextNotOfType(t, index))
  |> Result.map(~f=i => i - 1)
  |> Result.unwrap(~default=text->String.length - 1);

let findWordStart = (text, index) =>
  text->characterTypeAt(index)
  |> Result.andThen(~f=t => text->findPreviousNotOfType(t, index))
  |> Result.map(~f=i => i + 1)
  |> Result.unwrap(~default=0);

let findNextWordStart = (text, index) =>
  text->characterTypeAt(index)
  |> Result.andThen(~f=t => text->findNextNotOfType(t, index))
  |> Result.andThen(~f=i => text->findNextNotOfWhitespace(i));

let findNextWordEnd = (text, index) =>
  text->characterTypeAt(index)
  |> Result.andThen(~f=t => text->findNextNotOfType(t, index))
  |> Result.andThen(~f=i => text->findNextNotOfWhitespace(i))
  |> Result.map(~f=i => text->findWordEnd(i));

let findPreviousWordStart = (text, index) =>
  text->characterTypeAt(index)
  |> Result.andThen(~f=t => text->findPreviousNotOfType(t, index))
  |> Result.andThen(~f=i => text->findPreviousNotOfWhitespace(i))
  |> Result.map(~f=i => text->findWordStart(i));

let findPreviousWordEnd = (text, index) =>
  text->characterTypeAt(index)
  |> Result.andThen(~f=t => text->findPreviousNotOfType(t, index))
  |> Result.andThen(~f=i => text->findPreviousNotOfWhitespace(i));
