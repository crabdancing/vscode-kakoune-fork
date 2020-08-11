type t =
  | Normal
  | Insert
  | Search
  | Goto
  | GotoExtend;

let mode = ref(Normal);

let setMode = newMode => mode := newMode;

let getMode = () => mode^;
