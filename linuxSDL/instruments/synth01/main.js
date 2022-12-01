function processLine(line) {
  log("internal state.value", state.value);
  yoyo();
  return "hello" + line;
}

function renderer() {
  log("renderer");
  render("wowowowow");
}
// yoyo('bla bla');

var x = 1 + 2;
log("hello:", x);
123;
