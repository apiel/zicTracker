const { SerialPort } = require("serialport");
const { readFileSync } = require("fs");

const port = new SerialPort({
  path: "/dev/ttyUSB0",
  baudRate: 115200,
  autoOpen: false,
});

port.on("data", function (data) {
  // console.log("Data:", data.toString());
  process.stdout.write(data);
});

port.open(function (err) {
  if (err) {
    return console.log("Error opening port: ", err.message);
  }

  console.log('Connection opened.');
  // Because there's no callback to write, write errors will be emitted on the port:
  port.write("cd /mnt/SDCARD/App/zic\n");

  const content = readFileSync("../zicTrackerMiyoo-1.2").toString("base64");

  port.write(`rm zicTrackerMiyoo-1.2.64\n`);
  port.write(`touch zicTrackerMiyoo-1.2.64\n`);

  const chunks = content.split(/(.{200})/);
  chunks.forEach((chunk, i) => {
    port.write(`echo "${chunk}" >> zicTrackerMiyoo-1.2.64\n`);
    // console.log(`${i}/${chunks.length}`);
    // process.stdout.write('.');
  });

  port.write(`rm zicTrackerMiyoo-1.2\n`);
  port.write(`base64 -d zicTrackerMiyoo-1.2.64 > zicTrackerMiyoo-1.2\n`);
  port.write(`ls\n`);
  // port.close();
});
