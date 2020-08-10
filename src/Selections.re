let selectNextWord =
    (currentLine: Vscode.TextLine.t, currentSelection: Vscode.Selection.t) => {
  let {active: oldActive}: Vscode.Selection.t = currentSelection;
  let {line}: Vscode.Position.t = oldActive;
  let {text: lineText}: Vscode.TextLine.t = currentLine;

  let newActive =
    lineText
    |> TextLookup.findNextWordStart(~startIndex=oldActive.character)
    |> Result.map(~f=i => Vscode.Position.make(~line, ~character=i));

  newActive
  |> Result.map(~f=active =>
       Vscode.Selection.make(~anchor=oldActive, ~active)
     );
};
