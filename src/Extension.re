let selectNextWord =
    (textLine: Vscode.TextLine.t, currentSelection: Vscode.Selection.t) => {
  [
    textLine.text
    ->TextLookup.findNextWordStart(currentSelection.active.character),
    textLine.text
    ->TextLookup.findNextWordEnd(currentSelection.active.character),
  ]
  |> Result.values
  |> Result.map(~f=r => r->Js.log);
};

let onType = (args: Vscode.textCommandArgs) => {
  args->Js.log;

  Vscode.Window.activeTextEditor()
  |> Option.map(~f=e => (e->Vscode.TextEditor.getSelection, e))
  |> Option.map(~f=((s: Vscode.Selection.t, e: Vscode.TextEditor.t)) =>
       selectNextWord(e.document.lineAt(s.active.line), s)
     )
  |> Option.unwrap(~default=Error(TextLookup.LookupError.NotFound))
  |> Result.map(~f=nextWordSelection => nextWordSelection->Js.log)
  |> ignore;
};

let activate = context => {
  context->Vscode.overrideTypeCommand(onType);
  Vscode.setCursorStyle(Vscode.TextEditor.Block);
};
