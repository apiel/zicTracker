var count = 0;
var content = "wowowowow";

function renderer() {
  render(content + count);
}

function update(keys) {
  if (keys.Up) {
    count++;
  } else if (keys.Down) {
    count--;
  }
  log(JSON.stringify(CONFIG));
  content = "updated";
  return 1;
}
