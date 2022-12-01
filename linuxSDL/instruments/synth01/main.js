var count = 0;
var content = "wowowowow";

function renderer() {
  render(content + count);
}

function update(keys) {
  count++;
  // log(JSON.stringify(keys));
  content = "updated";
  return 1;
}
