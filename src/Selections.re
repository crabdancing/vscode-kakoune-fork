let onlyPrimarySelection =
    (~setSelections, selections: array(Vscode.Selection.t)) =>
  selections |> Array.first |> Option.tap(~f=s => [|s|] |> setSelections);

let onlyActiveCursor =
    (~setSelections, selections: array(Vscode.Selection.t)): unit =>
  selections
  |> Array.map(~f=(s: Vscode.Selection.t) =>
       Vscode.Selection.make(~anchor=s.active, ~active=s.active)
     )
  |> setSelections;

let findCharacterInLineFromActivePosition =
    (~active: Vscode.Position.t, ~character, text) =>
  text
  |> String.slice(~from=active.character, ~to_=text |> String.length)
  |> String.indexOf(_, character)
  |> Option.map(~f=i => i + active.character);

let selectToCharacter = (~character, ~getTextLine, selection) => {
  let {active}: Vscode.Selection.t = selection;

  let newCharacter =
    active.line
    |> getTextLine
    |> Option.unwrap(~default="")
    |> findCharacterInLineFromActivePosition(~active, ~character)
    |> Option.unwrap(~default=active.character);

  let newActive =
    Vscode.Position.make(~line=active.line, ~character=newCharacter);
  Vscode.Selection.make(~anchor=active, ~active=newActive);
};

let extendSelectionToCharacter = (~character, ~getTextLine, selection) => {
  let {active}: Vscode.Selection.t = selection;

  let newCharacter =
    active.line
    |> getTextLine
    |> Option.unwrap(~default="")
    |> findCharacterInLineFromActivePosition(~active, ~character)
    |> Option.unwrap(~default=active.character);

  let newActive =
    Vscode.Position.make(~line=active.line, ~character=newCharacter);

  Vscode.Selection.make(~anchor=selection.anchor, ~active=newActive);
};

let selectOnCharacter = (~character, ~getTextLine, selection) => {
  let {active}: Vscode.Selection.t = selection;

  let newCharacter =
    active.line
    |> getTextLine
    |> Option.unwrap(~default="")
    |> findCharacterInLineFromActivePosition(~active, ~character)
    |> Option.map(~f=i => i + 1)
    |> Option.unwrap(~default=active.character);

  let newActive =
    Vscode.Position.make(~line=active.line, ~character=newCharacter);
  Vscode.Selection.make(~anchor=active, ~active=newActive);
};

let extendSelectionOnCharacter = (~character, ~getTextLine, selection) => {
  let {active}: Vscode.Selection.t = selection;

  let newCharacter =
    active.line
    |> getTextLine
    |> Option.unwrap(~default="")
    |> findCharacterInLineFromActivePosition(~active, ~character)
    |> Option.map(~f=i => i + 1)
    |> Option.unwrap(~default=active.character);

  let newActive =
    Vscode.Position.make(~line=active.line, ~character=newCharacter);

  Vscode.Selection.make(~anchor=selection.anchor, ~active=newActive);
};

let selectFromActiveToCharacter =
    (~setSelections, ~getTextLine, ~character, selections): unit =>
  selections
  |> Array.map(~f=s => s |> selectToCharacter(~character, ~getTextLine))
  |> setSelections;

let extendSelectionFromActiveToCharacter =
    (~setSelections, ~getTextLine, ~character, selections): unit =>
  selections
  |> Array.map(~f=s =>
       s |> extendSelectionToCharacter(~character, ~getTextLine)
     )
  |> setSelections;

let selectFromActiveOnCharacter =
    (~setSelections, ~getTextLine, ~character, selections): unit =>
  selections
  |> Array.map(~f=s => s |> selectOnCharacter(~character, ~getTextLine))
  |> setSelections;

let extendSelectionFromActiveOnCharacter =
    (~setSelections, ~getTextLine, ~character, selections): unit =>
  selections
  |> Array.map(~f=s =>
       s |> extendSelectionOnCharacter(~character, ~getTextLine)
     )
  |> setSelections;
