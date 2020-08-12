let moveCharacterLeft = () => Vscode.Commands.moveCharacterLeft();
let extendCharacterLeft = () => Vscode.Commands.selectCharacterLeft();

let moveCharacterRight = () => Vscode.Commands.moveCharacterRight();
let extendCharacterRight = () => Vscode.Commands.selectCharacterRight();

let moveLineDown = () => Vscode.Commands.moveLineDown();
let extendLineDown = () => Vscode.Commands.selectLineDown();

let moveLineUp = () => Vscode.Commands.moveLineUp();
let extendLineUp = () => Vscode.Commands.selectLineUp();

let selectNextWord = () => {
  moveCharacterRight();
  Vscode.Commands.selectWordStartRight();
};
let extendNextWord = () => Vscode.Commands.selectWordStartRight();

let selectPreviousWord = () => {
  moveCharacterLeft();
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
