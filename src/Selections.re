let rec selectNextWord =
        (
          getTextLine: int => option(Vscode.TextLine.t),
          currentSelection: Vscode.Selection.t,
        ) => {
  let {active: oldActive}: Vscode.Selection.t = currentSelection;
  let {line}: Vscode.Position.t = oldActive;

  line
  |> getTextLine
  |> Option.map(~f=(l: Vscode.TextLine.t) => l.text)
  |> Option.andThen(~f=lineText =>
       lineText
       |> TextLookup.findNextWordStart(~startIndex=oldActive.character)
       |> Result.map(~f=i => Vscode.Position.make(~line, ~character=i))
       |> Result.map(~f=active =>
            Vscode.Selection.make(~anchor=oldActive, ~active)
          )
       |> (
         s =>
           switch (s) {
           | Ok(s) => Some(s)
           | Error(_) =>
             let newSelectionPos =
               Vscode.Position.make(~line=line + 1, ~character=0);
             Vscode.Selection.make(
               ~anchor=newSelectionPos,
               ~active=newSelectionPos,
             )
             |> selectNextWord(getTextLine);
           }
       )
     );
};
