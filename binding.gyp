{
	"targets": [{
		"target_name": "SnowflakeWorker",
		"include_dirs" : [
			"src",
			"<!(node -e \"require('nan')\")"
		],
		"sources": [
			"src/index.cc",
			"src/SnowflakeWorker.cc"
		]
	}]
}