const narwhal = require("../index");

const worker = new narwhal(1, 1);

for (let i = 0; i < 10; i++) {
  let id = worker.id();

  if (typeof id !== "string") {
    throw new TypeError(`Generated ID(${id}) not of type 'String'`);
  }

  if (!/^\d+$/.test(id)) {
    throw new Error(`Generated ID(${id}) is not numeric.`);
  }

  if (id.length < 16 || id.length > 19) {
    throw new Error(`Generated ID(${id}) is not 64 bits.`);
  }
}

console.log("\nNarwhal Test: Generated 10 IDs with no errors.\n");
