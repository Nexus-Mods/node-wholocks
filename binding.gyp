{
    "targets": [
        {
            "target_name": "wholocks",
            "includes": [
                "auto.gypi"
            ],
            "include_dirs": [
            ],
            "libraries": [
            ],
            "cflags!": ["-fno-exceptions"],
            "cflags_cc!": ["-fno-exceptions"],
            "defines": [
                "UNICODE",
                "_UNICODE"
            ],
            "msvs_settings": {
                "VCCLCompilerTool": {
                    "ExceptionHandling": 1
                }
            },
            "conditions":  [
                ["OS=='win'", {
                    "sources": [
                        "src_win/impl.cpp",
                        "src_win/wholocks.cpp"
                    ]
                }]
            ]
        }
    ],
    "includes": [
        "auto-top.gypi"
    ]
}
