let handleInsertMode = (_editor, _input: Vscode.textCommandArgs) => ();
let handleSearchMode = (_editor, _input: Vscode.textCommandArgs) => ();

let handleGotoExtendMode = (input: Vscode.textCommandArgs) => {
  switch (input.text) {
  | "h" => Movements.selectToLineStart()
  | "j" => Movements.selectToFileEnd()
  | "k" => Movements.selectToFileStart()
  | "l" => Movements.selectToLineEnd()
  | _ => ()
  };
  Mode.setMode(Mode.Normal);
};

let handleGotoMode = (input: Vscode.textCommandArgs) => {
  switch (input.text) {
  | "h" => Movements.gotoLineStart()
  | "j" => Movements.gotoFileEnd()
  | "k" => Movements.gotoFileStart()
  | "l" => Movements.gotoLineEnd()
  | _ => ()
  };
  Mode.setMode(Mode.Normal);
};

let handleNormalMode = (input: Vscode.textCommandArgs) =>
  switch (input.text) {
  | "w" => Movements.selectNextWord()
  | "W" => Movements.extendNextWord()
  | "b" => Movements.selectPreviousWord()
  | "B" => Movements.extendPreviousWord()
  | "h" => Movements.moveCharacterLeft()
  | "H" => Movements.extendCharacterLeft()
  | "j" => Movements.moveLineDown()
  | "J" => Movements.extendLineDown()
  | "k" => Movements.moveLineUp()
  | "K" => Movements.extendLineUp()
  | "l" => Movements.moveCharacterRight()
  | "L" => Movements.extendCharacterRight()
  | "x" => Movements.selectCurrentLine()
  | "X" => Movements.expandLineSelection()
  | "g" => Mode.setMode(Mode.Goto)
  | "G" => Mode.setMode(Mode.GotoExtend)
  | "d" => Edits.deleteSelections()
  | "p" => Edits.paste()
  | "P" => Edits.clearSelectionsAndPaste()
  | "c" =>
    Edits.deleteSelections();
    Mode.setMode(Mode.Insert);
  | _ => ()
  };

let onType = (args: Vscode.textCommandArgs) => {
  args->Js.log;

  Vscode.Window.activeTextEditor()
  |> Option.tap(~f=e =>
       switch (Mode.getMode()) {
       | Normal => args |> handleNormalMode
       | Insert => args |> handleInsertMode(e)
       | Search => args |> handleSearchMode(e)
       | Goto => args |> handleGotoMode
       | GotoExtend => args |> handleGotoExtendMode
       }
     );
};

let activate = context => {
  context->Vscode.overrideTypeCommand(onType);
  // Vscode.setCursorStyle(Vscode.TextEditor.Block);
};
