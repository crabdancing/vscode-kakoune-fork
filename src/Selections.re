let selectNextWord =
    (currentLine: Vscode.TextLine.t, currentSelection: Vscode.Selection.t) => {
  let {line}: Vscode.Position.t = currentSelection.active;
  let {text: lineText}: Vscode.TextLine.t = currentLine;

  let newActive =
    lineText
    |> TextLookup.findWordStart(
         ~startIndex=currentSelection.active.character + 1,
       )
    |> Result.map(~f=i => Vscode.Position.make(~line, ~character=i - 1));

  if (newActive |> Result.isError) {
    "newActive is Error" |> Js.log;
  } else {
    "newActive is Ok" |> Js.log;
  };

  "current active character index: "
  ++ (currentSelection.active.character |> string_of_int)
  |> Js.log;

  "current line: '" ++ lineText ++ "'" |> Js.log;

  "current active character: "
  ++ (
    lineText
    |> String.getAt(~index=currentSelection.active.character)
    |> Option.map(~f=c => c |> Char.toString)
    |> Option.unwrap(~default="")
  )
  |> Js.log;

  "after active character: "
  ++ (
    lineText
    |> String.getAt(~index=currentSelection.active.character + 1)
    |> Option.map(~f=c => c |> Char.toString)
    |> Option.unwrap(~default="")
  )
  |> Js.log;

  newActive
  |> Result.map(~f=active =>
       Vscode.Selection.make(~anchor=currentSelection.active, ~active)
     );
};
