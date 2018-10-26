"use strict";

const { SnowflakeWorker } = require("./build/Release/SnowflakeWorker.node");

const worker = new SnowflakeWorker(12, 14);

module.exports = SnowflakeWorker;
