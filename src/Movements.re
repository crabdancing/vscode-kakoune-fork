let selectNextWord = (editor: Vscode.TextEditor.t) => {
  let currentSelection = editor |> Vscode.TextEditor.getSelection;

  currentSelection
  |> Selections.selectNextWord(
       Vscode.TextDocument.getTextLine(_, editor.document),
     )
  |> Option.tap(~f=s => editor->Vscode.TextEditor.setSelection(s));
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
