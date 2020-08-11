let handleInsertMode = (_editor, _input: Vscode.textCommandArgs) => ();
let handleSearchMode = (_editor, _input: Vscode.textCommandArgs) => ();

let handleGotoMode = (editor, input: Vscode.textCommandArgs) => {
  switch (input.text) {
  | "h" => editor |> Movements.gotoLineStart
  | "j" => editor |> Movements.gotoFileEnd
  | "k" => editor |> Movements.gotoFileStart
  | "l" => editor |> Movements.gotoLineEnd
  | _ => ()
  };
  Mode.setMode(Mode.Normal);
};

let handleNormalMode = (editor, input: Vscode.textCommandArgs) =>
  switch (input.text) {
  | "w" => editor |> Movements.selectNextWord
  | "b" => editor |> Movements.selectPreviousWord
  | "h" => Movements.selectCharacterLeft()
  | "j" =>
    editor |> Vscode.TextEditor.getSelection |> Movements.selectCharacterDown
  | "k" =>
    editor |> Vscode.TextEditor.getSelection |> Movements.selectCharacterUp()
  | "l" => Movements.selectCharacterRight()
  | "x" => editor |> Movements.selectCurrentLine
  | "g" => Mode.setMode(Mode.Goto)
  | _ => ()
  };

let onType = (args: Vscode.textCommandArgs) => {
  args->Js.log;

  Vscode.Window.activeTextEditor()
  |> Option.tap(~f=e =>
       switch (Mode.getMode()) {
       | Normal => args |> handleNormalMode(e) |> ignore
       | Insert => args |> handleInsertMode(e) |> ignore
       | Search => args |> handleSearchMode(e) |> ignore
       | Goto => args |> handleGotoMode(e) |> ignore
       }
     );
};

let activate = context => {
  context->Vscode.overrideTypeCommand(onType);
  // Vscode.setCursorStyle(Vscode.TextEditor.Block);
};
