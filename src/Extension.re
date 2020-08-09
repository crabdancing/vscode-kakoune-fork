let onType = (args: Vscode.textCommandArgs) => {
  args->Js.log;
};

let activate = context => {
  context->Vscode.overrideTypeCommand(onType);
  Vscode.setCursorStyle(Vscode.TextEditor.Block);
};
