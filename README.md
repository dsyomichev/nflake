# nflake

Network ID generation service based on Twitter Snowflakes.

## Usage

```javascript

const NFlake = require("nflake")

const worker = new NFlake(1, 1)

console.log(worker.id())

```