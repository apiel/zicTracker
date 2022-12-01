var count = 0;
var content = "wowowowow";

function renderer() {
  render(content + count);
}

function update() {
  count++;
  content = "updated";
  return 1;
}
