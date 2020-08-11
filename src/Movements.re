/**
 * Moves the current selection to the next word.
 */
let selectNextWord = (editor: Vscode.TextEditor.t) =>
  editor
  |> Vscode.TextEditor.getSelection
  |> Selections.makeNextWordSelection(
       Vscode.TextDocument.getTextLine(_, editor.document),
     )
  |> Option.tap(~f=s => editor->Vscode.TextEditor.setSelection(s));

let selectPreviousWord = (editor: Vscode.TextEditor.t) =>
  editor
  |> Vscode.TextEditor.getSelection
  |> Selections.makePreviousWordSelection(
       Vscode.TextDocument.getTextLine(_, editor.document),
     )
  |> Option.tap(~f=s => editor->Vscode.TextEditor.setSelection(s));

let selectCharacterLeft = (_editor: Vscode.TextEditor.t) => ();
let selectCharacterRight = (_editor: Vscode.TextEditor.t) => ();
let selectCharacterDown = (_editor: Vscode.TextEditor.t) => ();
let selectCharacterUp = (_editor: Vscode.TextEditor.t) => ();

let selectCurrentLine = (_editor: Vscode.TextEditor.t) => ();

let gotoLineStart = (_editor: Vscode.TextEditor.t) => ();
let gotoLineEnd = (_editor: Vscode.TextEditor.t) => ();
let gotoFileStart = (_editor: Vscode.TextEditor.t) => ();
let gotoFileEnd = (_editor: Vscode.TextEditor.t) => ();
