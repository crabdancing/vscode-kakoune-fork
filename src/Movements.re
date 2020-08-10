let rec getNextNonEmptyLine =
        (currentLineIndex, document: Vscode.TextDocument.t) =>
  if (currentLineIndex < document.lineCount) {
    let textLine = document.lineAt(currentLineIndex);
    textLine.text |> String.isEmpty
      ? document |> getNextNonEmptyLine(currentLineIndex + 1)
      : Ok(currentLineIndex);
  } else {
    Error(TextLookup.LookupError.Overflow);
  };

let selectNextWord = (editor: Vscode.TextEditor.t) => {
  let currentSelection = editor |> Vscode.TextEditor.getSelection;
  let textLine = editor.document.lineAt(currentSelection.active.line);

  currentSelection
  |> Selections.selectNextWord(textLine)
  |> (
    s =>
      switch (s) {
      | Ok(s) => Ok(s)
      | Error(_) =>
        editor.document
        |> getNextNonEmptyLine(textLine.lineNumber + 1)
        |> Result.map(~f=newLineIndex =>
             editor.document.lineAt(newLineIndex)
           )
        |> Result.andThen(~f=(newTextLine: Vscode.TextLine.t) => {
             let newSelectionPos =
               Vscode.Position.make(
                 ~character=0,
                 ~line=newTextLine.lineNumber,
               );
             let newSelection =
               Vscode.Selection.make(
                 ~anchor=newSelectionPos,
                 ~active=newSelectionPos,
               );
             newSelection |> Selections.selectNextWord(newTextLine);
           })
      }
  )
  |> Result.tap(~f=s => editor->Vscode.TextEditor.setSelection(s));
};

let selectPreviousWord = (_editor: Vscode.TextEditor.t) => ();

let selectCharacterLeft = (_editor: Vscode.TextEditor.t) => ();
let selectCharacterRight = (_editor: Vscode.TextEditor.t) => ();
let selectCharacterDown = (_editor: Vscode.TextEditor.t) => ();
let selectCharacterUp = (_editor: Vscode.TextEditor.t) => ();

let selectCurrentLine = (_editor: Vscode.TextEditor.t) => ();

let gotoLineStart = (_editor: Vscode.TextEditor.t) => ();
let gotoLineEnd = (_editor: Vscode.TextEditor.t) => ();
let gotoFileStart = (_editor: Vscode.TextEditor.t) => ();
let gotoFileEnd = (_editor: Vscode.TextEditor.t) => ();
