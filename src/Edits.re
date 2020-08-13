let deleteSelections = (editor: Vscode.TextEditor.t) => {
  // Make sure every selection has at least one character selected.
  editor
  |> Vscode.TextEditor.getSelections
  |> Array.map(~f=(s: Vscode.Selection.t) => {
       s.anchor |> Vscode.Position.isEqual(s.active)
         ? {
           let newActiveCharacter = s.active.character + 1;
           let active =
             Vscode.Position.make(
               ~line=s.active.line,
               ~character=newActiveCharacter,
             );
           Vscode.Selection.make(~anchor=s.anchor, ~active);
         }
         : s
     })
  |> Vscode.TextEditor.setSelections(editor);
  Vscode.Commands.cut();
};

let copy = () => Vscode.Commands.copy();
let paste = () => Vscode.Commands.paste();
let clearSelectionsAndPaste = () => {
  Vscode.Commands.cancelSelection();
  paste();
};

let undo = () => Vscode.Commands.undo();
let redo = () => Vscode.Commands.redo();

let insertLineBelow = () => Vscode.Commands.insertLineBelow();
let insertLineAbove = () => Vscode.Commands.insertLineAbove();

let increaseSelectionIndentation = () => Vscode.Commands.increaseIndentation();
let decreaseSelectionIndentation = () => Vscode.Commands.decreaseIndentation();
