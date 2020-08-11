let deleteSelections = () => Vscode.Commands.cut();

let paste = () => Vscode.Commands.paste();
let clearSelectionsAndPaste = () => {
  Vscode.Commands.cancelSelection();
  paste();
};
