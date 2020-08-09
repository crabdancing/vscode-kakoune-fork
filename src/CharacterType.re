type t =
  | Punctuation
  | Character
  | Whitespace;

let characterType = character =>
  if ("\\w"->Js.Re.fromString->Js.Re.test_(character)) {
    Character;
  } else if ("\\s"->Js.Re.fromString->Js.Re.test_(character)) {
    Whitespace;
  } else {
    Punctuation;
  };
