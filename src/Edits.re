let deleteSelections = () => Vscode.Commands.cut();

let paste = () => Vscode.Commands.paste();
let clearSelectionsAndPaste = () => {
  Vscode.Commands.cancelSelection();
  paste();
};

let undo = () => Vscode.Commands.undo();
let redo = () => Vscode.Commands.redo();
