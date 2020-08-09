let selectNextWord =
    (textLine: Vscode.TextLine.t, currentSelection: Vscode.Selection.t) => {
  let nextWordStartPosition =
    textLine.text
    ->TextLookup.findNextWordStart(currentSelection.active.character)
    |> Result.map(~f=i =>
         Vscode.Position.make(
           ~line=currentSelection.active.line,
           ~character=i,
         )
       );

  let nextWordEndPosition =
    textLine.text
    ->TextLookup.findNextWordEnd(currentSelection.active.character)
    |> Result.map(~f=i =>
         Vscode.Position.make(
           ~line=currentSelection.active.line,
           ~character=i,
         )
       );

  Result.both(nextWordStartPosition, nextWordEndPosition)
  |> Result.map(~f=((startPos, endPos)) =>
       Vscode.Selection.make(~anchor=startPos, ~active=endPos)
     );
};

let onType = (args: Vscode.textCommandArgs) => {
  args->Js.log;

  Vscode.Window.activeTextEditor()
  |> Option.map(~f=e => (e->Vscode.TextEditor.getSelection, e))
  |> Option.map(~f=((s: Vscode.Selection.t, e: Vscode.TextEditor.t)) =>
       selectNextWord(e.document.lineAt(s.active.line), s)
     )
  |> Option.unwrap(~default=Error(TextLookup.LookupError.NotFound))
  |> Result.tap(~f=nextWordSelection =>
       Vscode.Window.activeTextEditor()
       |> Option.tap(~f=e =>
            Vscode.TextEditor.setSelection(e, nextWordSelection)
          )
     );
};

let activate = context => {
  context->Vscode.overrideTypeCommand(onType);
  Vscode.setCursorStyle(Vscode.TextEditor.Block);
};
