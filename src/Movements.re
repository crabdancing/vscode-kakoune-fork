let moveCharacterLeft = () => Vscode.Commands.moveCharacterLeft();
let extendCharacterLeft = () => Vscode.Commands.selectCharacterLeft();

let moveCharacterRight = () => Vscode.Commands.moveCharacterRight();
let extendCharacterRight = () => Vscode.Commands.selectCharacterRight();

let moveLineDown = () => Vscode.Commands.moveLineDown();
let extendLineDown = () => Vscode.Commands.selectLineDown();

let moveLineUp = () => Vscode.Commands.moveLineUp();
let extendLineUp = () => Vscode.Commands.selectLineUp();

let selectNextWord = (editor: Vscode.TextEditor.t) => {
  // Make sure every selection has no character selected and the cursor
  // is moved to the active position.
  editor
  |> Vscode.TextEditor.getSelections
  |> Array.map(~f=(s: Vscode.Selection.t) => {
       s.start
       |> Vscode.Position.isEqual(s.end_)
       || s.active
       |> Vscode.Position.isBefore(s.anchor)
         ? Vscode.Selection.make(~anchor=s.start, ~active=s.start)
         : Vscode.Selection.make(~anchor=s.end_, ~active=s.end_)
     })
  |> Vscode.TextEditor.setSelections(editor);
  Vscode.Commands.selectWordStartRight();
};
let extendNextWord = () => Vscode.Commands.selectWordStartRight();

let selectPreviousWord = (editor: Vscode.TextEditor.t) => {
  // Make sure every selection has no character selected and the cursor
  // is moved to the active position.
  editor
  |> Vscode.TextEditor.getSelections
  |> Array.map(~f=(s: Vscode.Selection.t) => {
       s.start
       |> Vscode.Position.isEqual(s.end_)
       || s.anchor
       |> Vscode.Position.isBefore(s.active)
         ? Vscode.Selection.make(~anchor=s.end_, ~active=s.end_)
         : Vscode.Selection.make(~anchor=s.start, ~active=s.start)
     })
  |> Vscode.TextEditor.setSelections(editor);
  Vscode.Commands.selectWordStartLeft();
};
let extendPreviousWord = () => Vscode.Commands.selectWordStartLeft();

let expandLineSelection = () => Vscode.Commands.expandLineSelection();

let selectCurrentLine = () => {
  Vscode.Commands.cancelSelection();
  expandLineSelection();
};

let gotoLineStart = () => Vscode.Commands.moveCursorLineStart();
let gotoLineEnd = () => Vscode.Commands.moveCursorLineEnd();
let gotoFileStart = () => Vscode.Commands.moveCursorTop();
let gotoFileEnd = () => Vscode.Commands.moveCursorBottom();

let gotoLineHome = () => Vscode.Commands.moveCursorHome();

let selectToLineStart = () => Vscode.Commands.selectToLineStart();
let selectToLineEnd = () => Vscode.Commands.selectToLineEnd();
let selectToFileStart = () => Vscode.Commands.selectToTop();
let selectToFileEnd = () => Vscode.Commands.selectToBottom();

let moveCursorToSelectionStart = (editor: Vscode.TextEditor.t) =>
  editor
  |> Vscode.TextEditor.getSelections
  |> Array.map(~f=(s: Vscode.Selection.t) =>
       Vscode.Selection.make(~anchor=s.start, ~active=s.start)
     )
  |> Vscode.TextEditor.setSelections(editor);

let moveCursorToSelectionEnd = (editor: Vscode.TextEditor.t) =>
  editor
  |> Vscode.TextEditor.getSelections
  |> Array.map(~f=(s: Vscode.Selection.t) =>
       s.start |> Vscode.Position.isEqual(s.end_)
         ? {
           let character = s.end_.character + 1;
           let newPos = Vscode.Position.make(~line=s.end_.line, ~character);
           Vscode.Selection.make(~anchor=newPos, ~active=newPos);
         }
         : Vscode.Selection.make(~anchor=s.end_, ~active=s.end_)
     )
  |> Vscode.TextEditor.setSelections(editor);
