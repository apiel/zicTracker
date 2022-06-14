const { SerialPort } = require("serialport");
const { readFileSync } = require("fs");

const port = new SerialPort({
  path: "/dev/ttyUSB0",
  baudRate: 115200,
  autoOpen: false,
});

port.on("data", function (data) {
  console.log("Data:", data.toString());
});

port.open(function (err) {
  if (err) {
    return console.log("Error opening port: ", err.message);
  }

  // Because there's no callback to write, write errors will be emitted on the port:
  port.write("cd /mnt/SDCARD/App/zic\n");

  const content = readFileSync("../zicServerMiyoo-1.2").toString("base64");

  port.write(`echo "" > zicServerMiyoo-1.2.64\n`);

  content.split(/(.{100})/).forEach((chunk) => {
    port.write(`echo "${chunk}" >> zicServerMiyoo-1.2.64\n`);
  });

  port.write(`base64 -d zicServerMiyoo-1.2.64 > zicServerMiyoo-1.2\n`);
});
