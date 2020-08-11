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

let selectCharacterLeft = () => {
  Vscode.Commands.cancelSelection();
  Vscode.Commands.selectCharacterLeft();
};

let selectCharacterRight = () => {
  Vscode.Commands.cancelSelection();
  Vscode.Commands.selectCharacterRight();
};

let selectCharacterDown = (currentSelection: Vscode.Selection.t) =>
  currentSelection.anchor |> Vscode.Position.isBefore(currentSelection.active)
    ? {
      Vscode.Commands.cancelSelection();
      Vscode.Commands.moveLineDown();
      Vscode.Commands.moveCharacterLeft();
      Vscode.Commands.selectCharacterRight();
    }
    : {
      Vscode.Commands.cancelSelection();
      Vscode.Commands.moveLineDown();
      Vscode.Commands.selectCharacterLeft();
    };

let selectCharacterUp = ((), currentSelection: Vscode.Selection.t) =>
  currentSelection.anchor |> Vscode.Position.isBefore(currentSelection.active)
    ? {
      Vscode.Commands.cancelSelection();
      Vscode.Commands.moveLineUp();
      Vscode.Commands.selectCharacterLeft();
    }
    : {
      Vscode.Commands.cancelSelection();
      Vscode.Commands.moveLineUp();
      Vscode.Commands.moveCharacterLeft();
      Vscode.Commands.selectCharacterRight();
    };

let selectCurrentLine = (_editor: Vscode.TextEditor.t) => ();

let gotoLineStart = (_editor: Vscode.TextEditor.t) => ();
let gotoLineEnd = (_editor: Vscode.TextEditor.t) => ();
let gotoFileStart = (_editor: Vscode.TextEditor.t) => ();
let gotoFileEnd = (_editor: Vscode.TextEditor.t) => ();
