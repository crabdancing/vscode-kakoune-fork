type t =
  | Normal
  | Insert
  | MaybeExitInsert
  | Search
  | Goto
  | GotoExtend;

let mode = ref(Normal);

let setMode = newMode => mode := newMode;

let getMode = () => mode^;
