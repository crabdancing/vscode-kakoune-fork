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
  | "b" => Movements.selectPreviousWord()
  | "h" => Movements.selectCharacterLeft()
  | "j" => Movements.selectCharacterDown()
  | "k" => Movements.selectCharacterUp()
  | "l" => Movements.selectCharacterRight()
  | "x" => Movements.selectCurrentLine()
  | "X" => Movements.expandLineSelection()
  | "g" => Mode.setMode(Mode.Goto)
  | "G" => Mode.setMode(Mode.GotoExtend)
  | _ => ()
  };

let onType = (args: Vscode.textCommandArgs) => {
  args->Js.log;

  Vscode.Window.activeTextEditor()
  |> Option.tap(~f=e =>
       switch (Mode.getMode()) {
       | Normal => args |> handleNormalMode |> ignore
       | Insert => args |> handleInsertMode(e) |> ignore
       | Search => args |> handleSearchMode(e) |> ignore
       | Goto => args |> handleGotoMode |> ignore
       | GotoExtend => args |> handleGotoExtendMode |> ignore
       }
     );
};

let activate = context => {
  context->Vscode.overrideTypeCommand(onType);
  // Vscode.setCursorStyle(Vscode.TextEditor.Block);
};
