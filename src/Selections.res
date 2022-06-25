let onlyPrimarySelection = (~setSelections, selections: array<Vscode.Selection.t>) =>
  Belt.Option.map(Belt.Array.get(selections, 0), s => [s] |> setSelections)

let onlyActiveCursor = (~setSelections, selections: array<Vscode.Selection.t>): unit =>
  selections
  |> Array.map((s: Vscode.Selection.t) => Vscode.Selection.make(~anchor=s.active, ~active=s.active))
  |> setSelections

let findCharacterInLineFromActivePosition = (~active: Vscode.Position.t, ~character, text) =>
  String.index_from_opt(text, active.character, character)

let selectToCharacter = (~character, ~getTextLine, selection) => {
  let {active}: Vscode.Selection.t = selection

  let newCharacter =
    active.line
    |> getTextLine
    |> Belt.Option.getWithDefault(_, "")
    |> findCharacterInLineFromActivePosition(~active, ~character)
    |> Belt.Option.getWithDefault(_, active.character)

  let newActive = Vscode.Position.make(~line=active.line, ~character=newCharacter)
  Vscode.Selection.make(~anchor=active, ~active=newActive)
}

let extendSelectionToCharacter = (~character, ~getTextLine, selection) => {
  let {active}: Vscode.Selection.t = selection

  let newCharacter =
    active.line
    |> getTextLine
    |> Belt.Option.getWithDefault(_, "")
    |> findCharacterInLineFromActivePosition(~active, ~character)
    |> Belt.Option.getWithDefault(_, active.character)

  let newActive = Vscode.Position.make(~line=active.line, ~character=newCharacter)

  Vscode.Selection.make(~anchor=selection.anchor, ~active=newActive)
}

let selectOnCharacter = (~character, ~getTextLine, selection) => {
  let {active}: Vscode.Selection.t = selection

  let newCharacter =
    active.line
    |> getTextLine
    |> Belt.Option.getWithDefault(_, "")
    |> findCharacterInLineFromActivePosition(~active, ~character)
    |> Belt.Option.map(_, i => i + 1)
    |> Belt.Option.getWithDefault(_, active.character)

  let newActive = Vscode.Position.make(~line=active.line, ~character=newCharacter)
  Vscode.Selection.make(~anchor=active, ~active=newActive)
}

let extendSelectionOnCharacter = (~character, ~getTextLine, selection) => {
  let {active}: Vscode.Selection.t = selection

  let newCharacter =
    active.line
    |> getTextLine
    |> Belt.Option.getWithDefault(_, "")
    |> findCharacterInLineFromActivePosition(~active, ~character)
    |> Belt.Option.map(_, i => i + 1)
    |> Belt.Option.getWithDefault(_, active.character)

  let newActive = Vscode.Position.make(~line=active.line, ~character=newCharacter)

  Vscode.Selection.make(~anchor=selection.anchor, ~active=newActive)
}

let selectFromActiveToCharacter = (~setSelections, ~getTextLine, ~character, selections): unit =>
  selections |> Array.map(s => s |> selectToCharacter(~character, ~getTextLine)) |> setSelections

let extendSelectionFromActiveToCharacter = (
  ~setSelections,
  ~getTextLine,
  ~character,
  selections,
): unit =>
  selections
  |> Array.map(s => s |> extendSelectionToCharacter(~character, ~getTextLine))
  |> setSelections

let selectFromActiveOnCharacter = (~setSelections, ~getTextLine, ~character, selections): unit =>
  selections |> Array.map(s => s |> selectOnCharacter(~character, ~getTextLine)) |> setSelections

let extendSelectionFromActiveOnCharacter = (
  ~setSelections,
  ~getTextLine,
  ~character,
  selections,
): unit =>
  selections
  |> Array.map(s => s |> extendSelectionOnCharacter(~character, ~getTextLine))
  |> setSelections
