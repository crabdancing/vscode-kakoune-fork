module LookupError = {
  type t =
    | NotFound
    | InvalidIndex;
};

let characterTypeAt = (text, index) =>
  index >= 0 && index < text->String.length
    ? Ok(text->String.sub(index, 1)->CharacterType.characterType)
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
  text
  ->characterTypeAt(index)
  ->Belt.Result.flatMap(t => text->findNextNotOfType(t, index))
  ->Belt.Result.map(i => i - 1)
  ->Belt.Result.getWithDefault(text->String.length - 1);

let findWordStart = (text, index) =>
  text
  ->characterTypeAt(index)
  ->Belt.Result.flatMap(t => text->findPreviousNotOfType(t, index))
  ->Belt.Result.map(i => i + 1)
  ->Belt.Result.getWithDefault(0);

let findNextWordStart = (text, index) =>
  text
  ->characterTypeAt(index)
  ->Belt.Result.flatMap(t => text->findNextNotOfType(t, index))
  ->Belt.Result.flatMap(i => text->findNextNotOfWhitespace(i));

let findNextWordEnd = (text, index) =>
  text
  ->characterTypeAt(index)
  ->Belt.Result.flatMap(t => text->findNextNotOfType(t, index))
  ->Belt.Result.flatMap(i => text->findNextNotOfWhitespace(i))
  ->Belt.Result.map(i => text->findWordEnd(i));

let findPreviousWordStart = (text, index) =>
  text
  ->characterTypeAt(index)
  ->Belt.Result.flatMap(t => text->findPreviousNotOfType(t, index))
  ->Belt.Result.flatMap(i => text->findPreviousNotOfWhitespace(i))
  ->Belt.Result.map(i => text->findWordStart(i));

let findPreviousWordEnd = (text, index) =>
  text
  ->characterTypeAt(index)
  ->Belt.Result.flatMap(t => text->findPreviousNotOfType(t, index))
  ->Belt.Result.flatMap(i => text->findPreviousNotOfWhitespace(i));
