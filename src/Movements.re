/**
 * Moves the current selection to the next word.
 */
let selectNextWord = () => {
  Vscode.Commands.cancelSelection();
  Vscode.Commands.selectWordStartRight();
};
let extendNextWord = () => Vscode.Commands.selectWordStartRight();

let selectPreviousWord = () => {
  Vscode.Commands.cancelSelection();
  Vscode.Commands.selectWordStartLeft();
};
let extendPreviousWord = () => Vscode.Commands.selectWordStartLeft();

let selectCharacterLeft = () => {
  Vscode.Commands.cancelSelection();
  Vscode.Commands.selectCharacterLeft();
};
let extendCharacterLeft = () => Vscode.Commands.selectCharacterLeft();

let selectCharacterRight = () => {
  Vscode.Commands.cancelSelection();
  Vscode.Commands.selectCharacterRight();
};
let extendCharacterRight = () => Vscode.Commands.selectCharacterRight();

let moveLineDown = () => Vscode.Commands.moveLineDown();
let extendLineDown = () => Vscode.Commands.selectLineDown();

let moveLineUp = () => Vscode.Commands.moveLineUp();
let extendLineUp = () => Vscode.Commands.selectLineUp();

let expandLineSelection = () => Vscode.Commands.expandLineSelection();

let selectCurrentLine = () => {
  Vscode.Commands.cancelSelection();
  expandLineSelection();
};

let gotoLineStart = () => Vscode.Commands.moveCursorLineStart();
let gotoLineEnd = () => Vscode.Commands.moveCursorLineEnd();
let gotoFileStart = () => Vscode.Commands.moveCursorTop();
let gotoFileEnd = () => Vscode.Commands.moveCursorBottom();

let selectToLineStart = () => Vscode.Commands.selectToLineStart();
let selectToLineEnd = () => Vscode.Commands.selectToLineEnd();
let selectToFileStart = () => Vscode.Commands.selectToTop();
let selectToFileEnd = () => Vscode.Commands.selectToBottom();
