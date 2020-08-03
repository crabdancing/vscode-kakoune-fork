let activate = context => {
  Vscode.overrideTypeCommand(context, _ => ());
  Vscode.setCursorStyle(Vscode.TextEditor.Block);

  Vscode.registerWindowChangeEventHandler(_ => ());
  Vscode.registerTextDocumentContentChangeEventHandler(_ => ());
};
